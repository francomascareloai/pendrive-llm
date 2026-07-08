# revisar.ps1 - manda um arquivo .c pro MiniCPM5-1B revisar/corrigir
# Uso:  powershell -ExecutionPolicy Bypass -File revisar.ps1 programa.c
#       (ou arraste o .c em cima de revisar.bat)
# Requer: run-minicpm.bat rodando (servidor na porta 8138)
# Versao 1B: temp 0.3 + LOOP DE CORRECAO (revisa -> compila -> se erro, corrige com gcc -> repete ate 3x).

param(
  [Parameter(Mandatory=$true, Position=0)]
  [string]$Arquivo,
  [switch]$Salvar,
  [switch]$Novo,
  [int]$MaxIter = 3
)

$ErrorActionPreference = "Stop"
$Porta = 8138
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"
$cb = '```'
$dirScript = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not $dirScript) { $dirScript = $PSScriptRoot }
if (-not $dirScript) { $dirScript = (Get-Location).Path }

$Arquivo = [System.IO.Path]::GetFullPath($Arquivo)
if (-not (Test-Path -LiteralPath $Arquivo)) {
  Write-Host "[ERRO] Arquivo nao encontrado: $Arquivo" -ForegroundColor Red
  exit 1
}

$codigo = Get-Content -LiteralPath $Arquivo -Raw -Encoding UTF8
if ([string]::IsNullOrWhiteSpace($codigo)) {
  Write-Host "[ERRO] Arquivo vazio ou ilegivel: $Arquivo" -ForegroundColor Red
  exit 1
}

try {
  Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null
} catch {
  Write-Host "[ERRO] Servidor nao esta rodando na porta $Porta." -ForegroundColor Red
  Write-Host "       Abra run-minicpm.bat primeiro (deixa a janela aberta)." -ForegroundColor Yellow
  exit 1
}

# --- Le o prompt de correcao externo (regras de tipagem) ---
$sysCorrecao = Get-Content -LiteralPath (Join-Path $dirScript 'prompts\sys_correcao.txt') -Raw -Encoding UTF8

# --- Extrai o bloco de codigo da resposta ---
# Regex unificada case-insensitive aceita ```c, ```C, ```cpp, ```c++ etc.
# Fallback: bloco sem fechar (resposta truncada por max_tokens) captura ate o final.
function Extrair-Codigo($conteudo) {
  $match = [regex]::Match($conteudo, '(?si)' + [regex]::Escape($cb) + '[a-z0-9+#]*\s*\r?\n(.*?)' + [regex]::Escape($cb))
  if (-not $match.Success) {
    $match = [regex]::Match($conteudo, '(?si)' + [regex]::Escape($cb) + '[a-z0-9+#]*\s*\r?\n(.*)$')
  }
  if (-not $match.Success) { return $null }
  return $match.Groups[1].Value.Trim()
}

# --- Compila e retorna os erros do gcc ---
function Compilar-Erros($codigo, $arqTemp) {
  $utf8NoBom = New-Object System.Text.UTF8Encoding($false)
  [System.IO.File]::WriteAllText($arqTemp, $codigo + "`n", $utf8NoBom)
  $gcc = Get-Command gcc -ErrorAction SilentlyContinue
  if (-not $gcc) { return @{ ok = $true; erros = ""; temGcc = $false } }
  $tmpExe = [System.IO.Path]::GetTempFileName()
  $psi = New-Object System.Diagnostics.ProcessStartInfo
  $psi.FileName = $gcc.Source
  $psi.Arguments = '"' + $arqTemp + '" -o "' + $tmpExe + '" -Wall 2>&1'
  $psi.UseShellExecute = $false
  $psi.RedirectStandardError = $true
  $psi.RedirectStandardOutput = $true
  $psi.CreateNoWindow = $true
  $p = [System.Diagnostics.Process]::Start($psi)
  # ler stdout ANTES de stderr evita deadlock (buffer de stdout cheio trava o ReadToEnd do stderr).
  $null = $p.StandardOutput.ReadToEnd()
  $errOut = $p.StandardError.ReadToEnd()
  $p.WaitForExit(15000) | Out-Null
  Remove-Item $tmpExe -ErrorAction SilentlyContinue
  $linhas = $errOut -split "`n" | Where-Object { $_ -match 'error:|warning:' } | Select-Object -First 15
  return @{ ok = ($p.ExitCode -eq 0); erros = ($linhas -join "`n"); temGcc = $true }
}

# --- Re-sobe o servidor se ele caiu (o 1B as vezes crasha o llama-server em geracoes longas) ---
function Garantir-Servidor {
  $ok = $false
  try { Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null; $ok = $true } catch {}
  if ($ok) { return $true }
  Write-Host "  [AVISO] Servidor caiu. Re-subindo..." -ForegroundColor Yellow
  $runBat = Join-Path $dirScript 'run-minicpm.bat'
  $null = Start-Process -FilePath 'cmd.exe' -ArgumentList '/c', $runBat -WorkingDirectory $dirScript -WindowStyle Hidden -PassThru
  for ($i = 0; $i -lt 45; $i++) {
    Start-Sleep -Seconds 2
    try { Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null; return $true } catch {}
  }
  return $false
}

# --- Chamada ao modelo com retry + re-subida do servidor ---
function Chamar-Modelo($mensagens) {
  $body = @{
    model = "minicpm5-1b"
    messages = $mensagens
    stream = $false
    temperature = 0.3
    top_p = 0.95
    max_tokens = 4096
  } | ConvertTo-Json -Depth 10 -Compress
  for ($tentativa = 1; $tentativa -le 3; $tentativa++) {
    try {
      return Invoke-RestMethod -Uri $Url -Method Post -ContentType "application/json; charset=utf-8" -Body $body -TimeoutSec 300
    } catch {
      Write-Host "  [AVISO] Tentativa $tentativa falhou: $($_.Exception.Message)" -ForegroundColor Yellow
      if ($tentativa -lt 3) {
        $subiu = Garantir-Servidor
        if (-not $subiu) { throw "Servidor nao voltou apos re-subir." }
      } else {
        throw
      }
    }
  }
}

$arqTemp = [System.IO.Path]::GetTempFileName() + ".c"

# --- Compila o codigo original pra ver se tem erro ---
$compilacao = Compilar-Erros $codigo $arqTemp
if ($compilacao.temGcc -and $compilacao.ok) {
  Write-Host "[INFO] O codigo original ja compila. Vou revisar por bugs de logica/memoria." -ForegroundColor DarkGray
  $temErro = $false
} elseif ($compilacao.temGcc) {
  Write-Host "[INFO] O codigo original tem erros de compilacao. Vou corrigir." -ForegroundColor DarkGray
  $temErro = $true
} else {
  $temErro = $false
}

# --- 1a revisao ---
if ($temErro) {
  Write-Host "Revisando (corrigindo erros de compilacao)..." -ForegroundColor Cyan
  $userMsg = "Corrija este codigo C que nao compila:`n`n" + $cb + "c`n" + $codigo + "`n" + $cb + "`n`nErros do gcc:`n" + $compilacao.erros + "`n`nDevolva o codigo corrigido no bloco " + $cb + "c."
} else {
  Write-Host "Revisando (bugs de logica/memoria)..." -ForegroundColor Cyan
  $userMsg = "Revise este codigo C. Corrija bugs, vazamentos de memoria, erros de logica e melhore a qualidade mantendo a funcionalidade. Se nao houver nada a corrigir, devolva o codigo igual.`n`nCodigo:`n" + $cb + "c`n" + $codigo + "`n" + $cb + "`n`nDevolva o codigo corrigido no bloco " + $cb + "c."
}

try {
  $resp = Chamar-Modelo @(@{ role = "system"; content = $sysCorrecao }, @{ role = "user"; content = $userMsg })
} catch {
  Write-Host "[ERRO] Falha ao chamar o modelo: $($_.Exception.Message)" -ForegroundColor Red
  exit 1
}
$codigoCorrigido = Extrair-Codigo $resp.choices[0].message.content
# avisa se a resposta foi truncada por limite de tokens (codigo corrigido incompleto)
$finish = $resp.choices[0].finish_reason
if ($finish -eq "length") {
  Write-Host "[AVISO] Resposta truncada por limite de tokens (finish_reason=length)." -ForegroundColor Yellow
  Write-Host "        O codigo corrigido pode estar incompleto. Considere aumentar o contexto (config.bat)." -ForegroundColor Yellow
}
if (-not $codigoCorrigido) {
  Write-Host "[AVISO] Nao achei bloco de codigo na resposta. Mostrando a resposta bruta:" -ForegroundColor Yellow
  Write-Host $resp.choices[0].message.content
  exit 0
}

# --- LOOP DE CORRECAO: se ainda tem erro de compilacao, itera (janela deslizante p/ nao estourar ctx) ---
$iter = 0
if ($compilacao.temGcc) {
  $compilacao = Compilar-Erros $codigoCorrigido $arqTemp
  while (-not $compilacao.ok -and $iter -lt $MaxIter) {
    $iter++
    Write-Host "  Corrigindo (iteracao $iter de $MaxIter)..." -ForegroundColor Cyan
    $promptCorrecao = "Aqui esta um codigo C que NAO compila. Erros do gcc (gcc -Wall):`n`n" + $compilacao.erros + "`n`nCodigo atual:`n" + $cb + "c`n" + $codigoCorrigido + "`n" + $cb + "`n`nCorrija TODOS os erros. Devolva o codigo C COMPLETO corrigido num bloco " + $cb + "c ... " + $cb + ". Nao escreva texto fora do bloco."
    $historico = @(
      @{ role = "system"; content = $sysCorrecao }
      @{ role = "user";   content = $promptCorrecao }
    )
    try {
      $resp = Chamar-Modelo $historico
    } catch {
      Write-Host "  [ERRO] Falha na correcao: $($_.Exception.Message)" -ForegroundColor Red
      break
    }
    $novoCodigo = Extrair-Codigo $resp.choices[0].message.content
    if (-not $novoCodigo) {
      Write-Host "  [AVISO] Iteracao $iter sem bloco. Mantendo versao anterior." -ForegroundColor Yellow
      break
    }
    $codigoCorrigido = $novoCodigo
    $compilacao = Compilar-Erros $codigoCorrigido $arqTemp
    if ($compilacao.ok) {
      Write-Host "  [OK] Compilou sem erros na iteracao $iter!" -ForegroundColor Green
      break
    } else {
      $nLinhas = ($compilacao.erros -split "`n").Count
      Write-Host "  Ainda com $nLinhas erro(s) apos iteracao $iter." -ForegroundColor Yellow
    }
  }
}

Remove-Item $arqTemp -ErrorAction SilentlyContinue

# --- Mostra resultado ---
Write-Host ""
Write-Host "===== CODIGO CORRIGIDO =====" -ForegroundColor Green
Write-Host $codigoCorrigido
Write-Host "=============================" -ForegroundColor Green
if ($compilacao.temGcc) {
  if ($compilacao.ok) {
    Write-Host "[OK] Compila sem erros (gcc -Wall)." -ForegroundColor Green
  } else {
    Write-Host "[AVISO] Ainda tem erros apos $MaxIter iteracoes:" -ForegroundColor Yellow
    Write-Host $compilacao.erros
  }
}
Write-Host ""
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$sobrescrever = $false
if ($Salvar) {
  $sobrescrever = $true
} elseif (-not $Novo) {
  $resp2 = Read-Host "Salvar por cima do arquivo original? (s/N)"
  if ($resp2 -eq "s" -or $resp2 -eq "S") { $sobrescrever = $true }
}
if ($sobrescrever) {
  $bak = $Arquivo + ".bak"
  Copy-Item -LiteralPath $Arquivo -Destination $bak -Force
  [System.IO.File]::WriteAllText($Arquivo, $codigoCorrigido + "`n", $utf8NoBom)
  Write-Host "[OK] Salvo. Backup criado: $bak" -ForegroundColor Green
} else {
  $arquivoCorrigido = [System.IO.Path]::ChangeExtension($Arquivo, ".corrigido.c")
  [System.IO.File]::WriteAllText($arquivoCorrigido, $codigoCorrigido + "`n", $utf8NoBom)
  Write-Host "[OK] Salvo em: $arquivoCorrigido" -ForegroundColor Green
}
