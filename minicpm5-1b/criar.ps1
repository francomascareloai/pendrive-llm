# criar.ps1 - pede uma descricao, gera um programa C completo e salva no arquivo (MiniCPM5-1B)
# Uso:  powershell -ExecutionPolicy Bypass -File criar.ps1 -Saida programa.c -Desc "lista ligada com 3 nos"
#       (ou rode criar.bat - ele pede a descricao e o nome interativamente)
# Requer: run-minicpm.bat rodando (servidor na porta 8138)
#
# ESTRATEGIA: 1 chamada com prompt forte (regras de tipagem + 2 exemplos canonicos) + LOOP DE CORRECAO.
# O loop e deterministico: gera -> compila com gcc -> se erro, alimenta o stderr do gcc de volta -> repete ate 3x.
# Retry com re-subida do servidor: se o 1B crashar o llama-server, o script re-sobe sozinho.

param(
  [string]$Saida,
  [string]$Desc,
  [int]$MaxIter = 3
)

$ErrorActionPreference = "Stop"
$Porta = 8138
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"
$cb = '```'
$dirScript = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not $dirScript) { $dirScript = $PSScriptRoot }
if (-not $dirScript) { $dirScript = (Get-Location).Path }

if (-not $Desc) {
  Write-Host "Descreva o programa C que voce quer (ex: lista ligada com 3 nos, ordenar array, calcular fatorial recursivo):" -ForegroundColor Cyan
  $Desc = Read-Host "Descricao"
}
if (-not $Desc) {
  Write-Host "[ERRO] Descricao vazia." -ForegroundColor Red
  exit 1
}

if (-not $Saida) {
  Write-Host ""
  Write-Host "Nome do arquivo de saida (ex: programa.c). Enter = programa.c:" -ForegroundColor Cyan
  $Saida = Read-Host "Arquivo"
  if (-not $Saida) { $Saida = "programa.c" }
}

$ext = [System.IO.Path]::GetExtension($Saida).ToLower()
if ($ext -eq ".cpp" -or $ext -eq ".cc" -or $ext -eq ".cxx") {
  $Saida = [System.IO.Path]::ChangeExtension($Saida, ".c")
} elseif ($ext -ne ".c" -and $ext -ne ".h") {
  $Saida = $Saida + ".c"
}
$Saida = [System.IO.Path]::GetFullPath($Saida)

try {
  Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null
} catch {
  Write-Host "[ERRO] Servidor nao esta rodando na porta $Porta." -ForegroundColor Red
  Write-Host "       Abra run-minicpm.bat primeiro (deixa a janela aberta)." -ForegroundColor Yellow
  exit 1
}

# --- Le os prompts externos ---
$sysGerar   = Get-Content -LiteralPath (Join-Path $dirScript 'prompts\sys_corpos.txt')   -Raw -Encoding UTF8
$sysCorrecao = Get-Content -LiteralPath (Join-Path $dirScript 'prompts\sys_correcao.txt') -Raw -Encoding UTF8

# --- Extrai o PRIMEIRO bloco de codigo da resposta ---
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

# --- Compila e retorna os erros do gcc (stderr), limitado ---
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

# ===================== GERACAO INICIAL =====================
Write-Host ""
Write-Host "Gerando programa..." -ForegroundColor Cyan
$userMsg = "Escreva um programa C completo que: $Desc`n`nDevolva APENAS o codigo no bloco " + $cb + "c. Nao escreva texto explicativo antes nem depois do bloco."
try {
  $resp = Chamar-Modelo @(@{ role = "system"; content = $sysGerar }, @{ role = "user"; content = $userMsg })
} catch {
  Write-Host "[ERRO] Falha ao chamar o modelo: $($_.Exception.Message)" -ForegroundColor Red
  exit 1
}
$codigo = Extrair-Codigo $resp.choices[0].message.content
# avisa se a resposta foi truncada por limite de tokens (programa grande/incompleto)
$finish = $resp.choices[0].finish_reason
if ($finish -eq "length") {
  Write-Host "[AVISO] Resposta truncada por limite de tokens (finish_reason=length)." -ForegroundColor Yellow
  Write-Host "        O programa pode estar incompleto. Considere aumentar o contexto (config.bat)." -ForegroundColor Yellow
}
if (-not $codigo) {
  Write-Host "[AVISO] Nao achei bloco de codigo na resposta. Mostrando a resposta bruta:" -ForegroundColor Yellow
  Write-Host $resp.choices[0].message.content
  exit 0
}

# ===================== LOOP DE CORRECAO =====================
$compilacao = Compilar-Erros $codigo $arqTemp
if ($compilacao.temGcc -and $compilacao.ok) {
  Write-Host "[OK] Compilou sem erros na 1a tentativa!" -ForegroundColor Green
} elseif ($compilacao.temGcc) {
  Write-Host "Loop de correcao (ate $MaxIter iteracoes)..." -ForegroundColor Cyan
  $nLinhas = ($compilacao.erros -split "`n").Count
  Write-Host "  Erros iniciais: $nLinhas linha(s)" -ForegroundColor DarkGray
}

$iter = 0
# janela deslizante: so system + codigo atual + erros (nao acumula historico, evita estourar ctx 8192).
while ($compilacao.temGcc -and -not $compilacao.ok -and $iter -lt $MaxIter) {
  $iter++
  Write-Host "  Corrigindo (iteracao $iter de $MaxIter)..." -ForegroundColor Cyan
  $promptCorrecao = "Aqui esta um codigo C que NAO compila. Erros do gcc (gcc -Wall):`n`n" + $compilacao.erros + "`n`nCodigo atual:`n" + $cb + "c`n" + $codigo + "`n" + $cb + "`n`nCorrija TODOS os erros. Devolva o codigo C COMPLETO corrigido num bloco " + $cb + "c ... " + $cb + ". Nao escreva texto fora do bloco."
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
  $codigo = $novoCodigo
  $compilacao = Compilar-Erros $codigo $arqTemp
  if ($compilacao.ok) {
    Write-Host "  [OK] Compilou sem erros na iteracao $iter!" -ForegroundColor Green
    break
  } else {
    $nLinhas = ($compilacao.erros -split "`n").Count
    Write-Host "  Ainda com $nLinhas erro(s) apos iteracao $iter." -ForegroundColor Yellow
  }
}

Remove-Item $arqTemp -ErrorAction SilentlyContinue

# --- Mostra resultado final ---
Write-Host ""
Write-Host "===== PROGRAMA GERADO =====" -ForegroundColor Green
Write-Host $codigo
Write-Host "===========================" -ForegroundColor Green

$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
if (Test-Path -LiteralPath $Saida) {
  Copy-Item -LiteralPath $Saida -Destination ($Saida + ".bak") -Force
  Write-Host ""
  Write-Host "[AVISO] $Saida ja existia - backup criado em $Saida.bak" -ForegroundColor Yellow
}
[System.IO.File]::WriteAllText($Saida, $codigo + "`n", $utf8NoBom)
Write-Host ""
Write-Host "[OK] Salvo em: $Saida" -ForegroundColor Green

if ($compilacao.temGcc) {
  if ($compilacao.ok) {
    Write-Host "[OK] Compila sem erros (gcc -Wall)." -ForegroundColor Green
  } else {
    Write-Host "[AVISO] Ainda tem erros apos $MaxIter iteracoes:" -ForegroundColor Yellow
    Write-Host $compilacao.erros
    Write-Host "       Rode revisar.bat no arquivo para mais uma rodada de correcao." -ForegroundColor Cyan
  }
} else {
  Write-Host "[INFO] gcc nao encontrado - nao foi possivel validar a compilacao." -ForegroundColor DarkGray
}
