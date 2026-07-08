# criar.ps1 - pede uma descricao, gera um programa C completo e salva no arquivo (MiniCPM5-1B)
# Uso:  powershell -ExecutionPolicy Bypass -File criar.ps1 -Saida programa.c -Desc "lista ligada com 3 nos"
#       (ou rode criar.bat - ele pede a descricao e o nome interativamente)
# Requer: run-minicpm.bat rodando (servidor na porta 8138)
# Versao 1B: temperatura 0.3 (baixa = mais determinista, menos alucinacao).

param(
  [string]$Saida,
  [string]$Desc
)

$ErrorActionPreference = "Stop"
$Porta = 8138
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"

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

$system = 'Voce e um assistente especializado em C. Pense com cuidado no problema antes de escrever o codigo - verifique logica, tipos, limites de loop e includes necessarios. Devolva SEMPRE o codigo C completo e pronto para compilar dentro de um unico bloco ```c ... ```. O codigo deve incluir todos os #include necessarios, a funcao main, e ser autonomo. Nao invente funcoes da biblioteca padrao (use stdio.h, stdlib.h, string.h reais). Antes do bloco, escreva 1-2 linhas curtas em portugues descrevendo o que o programa faz. Nao escreva texto depois do bloco.'
$userMsg = "Escreva um programa C completo que: $Desc`n`nDevolva o codigo no bloco ```c."

$body = @{
  model = "minicpm5-1b"
  messages = @(
    @{ role = "system"; content = $system }
    @{ role = "user";   content = $userMsg }
  )
  stream = $false
  temperature = 0.3
  top_p = 0.95
  max_tokens = 4096
} | ConvertTo-Json -Depth 10 -Compress

Write-Host ""
Write-Host "Gerando programa..." -ForegroundColor Cyan
try {
  $resp = Invoke-RestMethod -Uri $Url -Method Post -ContentType "application/json; charset=utf-8" -Body $body -TimeoutSec 300
} catch {
  Write-Host "[ERRO] Falha ao chamar o modelo: $($_.Exception.Message)" -ForegroundColor Red
  exit 1
}

$conteudo = $resp.choices[0].message.content

$cb = '```'
$match = [regex]::Match($conteudo, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*?)' + [regex]::Escape($cb))
if (-not $match.Success) {
  $match = [regex]::Match($conteudo, '(?s)' + [regex]::Escape($cb) + '\s*\r?\n(.*?)' + [regex]::Escape($cb))
}
if (-not $match.Success) {
  Write-Host "[AVISO] Nao achei bloco de codigo na resposta. Mostrando a resposta bruta:" -ForegroundColor Yellow
  Write-Host $conteudo
  exit 0
}

$codigo = $match.Groups[1].Value.Trim()
$intro = $conteudo.Substring(0, $match.Index).Trim()
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

$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
if (Test-Path -LiteralPath $Saida) {
  Copy-Item -LiteralPath $Saida -Destination ($Saida + ".bak") -Force
  Write-Host ""
  Write-Host "[AVISO] $Saida ja existia - backup criado em $Saida.bak" -ForegroundColor Yellow
}
[System.IO.File]::WriteAllText($Saida, $codigo + "`n", $utf8NoBom)
Write-Host ""
Write-Host "[OK] Salvo em: $Saida" -ForegroundColor Green

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
  # ler stdout ANTES de stderr evita deadlock: se o buffer de stdout encher
  # (>4KB de warnings do gcc) o processo bloqueia e ReadToEnd() do stderr trava.
  $null = $p.StandardOutput.ReadToEnd()
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
