# criar.ps1 — pede uma descricao, gera um programa C completo e salva no arquivo (Ornith)
# Uso:  powershell -ExecutionPolicy Bypass -File criar.ps1 -Saida programa.c -Desc "lista ligada com 3 nos"
#       (ou rode criar.bat — ele pede a descricao e o nome interativamente)
# Requer: run.bat rodando (servidor na porta 8137)

param(
  # nome do arquivo de saida (opcional — se vazio, pergunta)
  [string]$Saida,
  # descricao do programa (opcional — se vazio, pergunta)
  [string]$Desc
)

$ErrorActionPreference = "Stop"
$Porta = 8137
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"

# se nao veio descricao, pede
if (-not $Desc) {
  Write-Host "Descreva o programa C que voce quer (ex: lista ligada com 3 nos, ordenar array, calcular fatorial recursivo):" -ForegroundColor Cyan
  $Desc = Read-Host "Descricao"
}
if (-not $Desc) {
  Write-Host "[ERRO] Descricao vazia." -ForegroundColor Red
  exit 1
}

# se nao veio nome de saida, pergunta
if (-not $Saida) {
  Write-Host ""
  Write-Host "Nome do arquivo de saida (ex: programa.c). Enter = programa.c:" -ForegroundColor Cyan
  $Saida = Read-Host "Arquivo"
  if (-not $Saida) { $Saida = "programa.c" }
}

# garante extensao .c (ou .h se o usuario pediu header)
# aceita .c/.h em qualquer case; .cpp/.cc/.cxx viram .c (evita "arquivo.cpp.c")
$ext = [System.IO.Path]::GetExtension($Saida).ToLower()
if ($ext -eq ".cpp" -or $ext -eq ".cc" -or $ext -eq ".cxx") {
  $Saida = [System.IO.Path]::ChangeExtension($Saida, ".c")
} elseif ($ext -ne ".c" -and $ext -ne ".h") {
  $Saida = $Saida + ".c"
}
$Saida = [System.IO.Path]::GetFullPath($Saida)

# checa servidor
try {
  Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null
} catch {
  Write-Host "[ERRO] Servidor nao esta rodando na porta $Porta." -ForegroundColor Red
  Write-Host "       Abra run.bat primeiro (deixa a janela aberta)." -ForegroundColor Yellow
  exit 1
}

$system = 'Voce e um assistente especializado em C. Pense com cuidado no problema antes de escrever o codigo - verifique logica, tipos, limites de loop e includes necessarios. Devolva SEMPRE o codigo C completo e pronto para compilar dentro de um unico bloco ```c ... ```. O codigo deve incluir todos os #include necessarios, a funcao main, e ser autonomo. Antes do bloco, escreva 1-2 linhas curtas em portugues descrevendo o que o programa faz. Nao escreva texto depois do bloco.'
$userMsg = "Escreva um programa C completo que: $Desc`n`nDevolva o codigo no bloco ```c."

$body = @{
  model = "ornith"
  messages = @(
    @{ role = "system"; content = $system }
    @{ role = "user";   content = $userMsg }
  )
  stream = $false
  temperature = 0.3
  top_p = 0.95
  max_tokens = 8192
} | ConvertTo-Json -Depth 10 -Compress

Write-Host ""
Write-Host "Gerando programa..." -ForegroundColor Cyan
# retry com backoff: o servidor pode cair no meio de uma geracao longa (OOM em
# PC com pouca RAM). Tenta ate 3x com pausa crescente, avisando o usuario.
$resp = $null
for ($tent = 1; $tent -le 3; $tent++) {
  try {
    $resp = Invoke-RestMethod -Uri $Url -Method Post -ContentType "application/json; charset=utf-8" -Body $body -TimeoutSec 600
    break
  } catch {
    if ($tent -lt 3) {
      $espera = $tent * 5
      Write-Host "[AVISO] Servidor nao respondeu (tentativa $tent/3). Aguardando ${espera}s..." -ForegroundColor Yellow
      Start-Sleep -Seconds $espera
      try { Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null }
      catch {
        Write-Host "[ERRO] O servidor caiu (provavelmente RAM insuficiente)." -ForegroundColor Red
        Write-Host "       Feche o run.bat, feche outros programas pesados, e suba o servidor de novo." -ForegroundColor Yellow
        exit 1
      }
    } else {
      Write-Host "[ERRO] Falha ao chamar o modelo apos 3 tentativas: $($_.Exception.Message)" -ForegroundColor Red
      Write-Host "       Se o servidor caiu, reinicie o run.bat (feche outros programas pesados)." -ForegroundColor Yellow
      exit 1
    }
  }
}
if (-not $resp) { exit 1 }

$conteudo = $resp.choices[0].message.content

# avisa se a resposta foi truncada por limite de tokens (programa grande/incompleto)
$finish = $resp.choices[0].finish_reason
if ($finish -eq "length") {
  Write-Host "[AVISO] Resposta truncada por limite de tokens (finish_reason=length)." -ForegroundColor Yellow
  Write-Host "        O programa pode estar incompleto. Considere aumentar o contexto (config.bat)." -ForegroundColor Yellow
}

# extrai o bloco de codigo. Regex unificada case-insensitive aceita ```c, ```C,
# ```cpp, ```c++ etc. ([a-z0-9+#]* apos o fence). Fallback: bloco sem fechar
# (resposta truncada por max_tokens) captura ate o final da string.
$cb = '```'
$match = [regex]::Match($conteudo, '(?si)' + [regex]::Escape($cb) + '[a-z0-9+#]*\s*\r?\n(.*?)' + [regex]::Escape($cb))
if (-not $match.Success) {
  $match = [regex]::Match($conteudo, '(?si)' + [regex]::Escape($cb) + '[a-z0-9+#]*\s*\r?\n(.*)$')
}
if (-not $match.Success) {
  Write-Host "[AVISO] Nao achei bloco de codigo na resposta. Mostrando a resposta bruta:" -ForegroundColor Yellow
  Write-Host $conteudo
  exit 0
}

$codigo = $match.Groups[1].Value.Trim()
$intro = $conteudo.Substring(0, $match.Index).Trim()
# limpa tokens de fim de template que vazam
$intro = $intro -replace '<\|[^|]*\|>', ''
$intro = $intro.Trim()

Write-Host ""
Write-Host "===== PROGRAMA GERADO =====" -ForegroundColor Green
Write-Host $codigo
Write-Host "===========================" -ForegroundColor Green
if ($intro) {
  Write-Host ""
  Write-Host $intro -ForegroundColor DarkGray
}

# se o arquivo ja existe, faz backup
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
if (Test-Path -LiteralPath $Saida) {
  Copy-Item -LiteralPath $Saida -Destination ($Saida + ".bak") -Force
  Write-Host ""
  Write-Host "[AVISO] $Saida ja existia - backup criado em $Saida.bak" -ForegroundColor Yellow
}
[System.IO.File]::WriteAllText($Saida, $codigo + "`n", $utf8NoBom)
Write-Host ""
Write-Host "[OK] Salvo em: $Saida" -ForegroundColor Green

# tenta compilar pra avisar se tem erro
$gcc = Get-Command gcc -ErrorAction SilentlyContinue
if ($gcc) {
  $tmpExe = [System.IO.Path]::GetTempFileName()
  $psi = New-Object System.Diagnostics.ProcessStartInfo
  $psi.FileName = $gcc.Source
  $psi.Arguments = '"' + $Saida + '" -o "' + $tmpExe + '"'
  $psi.UseShellExecute = $false
  $psi.RedirectStandardError = $true
  $psi.RedirectStandardOutput = $true
  $psi.CreateNoWindow = $true
  $p = [System.Diagnostics.Process]::Start($psi)
  $errOut = $p.StandardError.ReadToEnd()
  $p.WaitForExit(15000) | Out-Null
  Remove-Item $tmpExe -ErrorAction SilentlyContinue
  if ($p.ExitCode -eq 0) {
    Write-Host "[OK] Compilou sem erros." -ForegroundColor Green
  } else {
    Write-Host "[AVISO] Gerou erro de compilacao:" -ForegroundColor Yellow
    Write-Host $errOut.Trim()
    Write-Host "       Rode revisar.bat no arquivo para corrigir automaticamente." -ForegroundColor Cyan
  }
}
