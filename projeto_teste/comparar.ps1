# comparar.ps1 - roda a mesma tarefa nos dois modelos e avalia compilacao + warnings
param(
  [string]$Desc,
  [string]$SaidaMini = 'out_mini.c',
  [string]$SaidaOrnith = 'out_ornith.c'
)

$ErrorActionPreference = 'Continue'
$dir = 'E:\LLM\pendrive-backup-2026-07-08\projeto_teste'
Set-Location $dir

function Chamar-Modelo($porta, $modelo, $desc, $saida) {
  $system = 'Voce e um assistente especializado em C. Devolva SEMPRE o codigo C completo e pronto para compilar dentro de um unico bloco ```c ... ```. Inclua todos os #include necessarios e a funcao main. Nao invente funcoes da biblioteca padrao. Antes do bloco, 1-2 linhas curtas em portugues descrevendo o programa. Nao escreva texto depois do bloco.'
  $userMsg = "Escreva um programa C completo que: $desc`n`nDevolva o codigo no bloco ```c."
  $body = @{
    model = $modelo
    messages = @(@{role='system';content=$system}, @{role='user';content=$userMsg})
    stream = $false; temperature = 0.3; top_p = 0.95; max_tokens = 4096
  } | ConvertTo-Json -Depth 10 -Compress
  try {
    $resp = Invoke-RestMethod -Uri "http://127.0.0.1:$porta/v1/chat/completions" -Method Post -ContentType 'application/json; charset=utf-8' -Body $body -TimeoutSec 300
  } catch { return "[ERRO API] $($_.Exception.Message)" }
  $c = $resp.choices[0].message.content
  $cb = '```'
  $m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*?)' + [regex]::Escape($cb))
  if (-not $m.Success) { $m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + '\s*\r?\n(.*?)' + [regex]::Escape($cb)) }
  if (-not $m.Success) { return $null }
  $codigo = $m.Groups[1].Value.Trim()
  $utf8 = New-Object System.Text.UTF8Encoding($false)
  [System.IO.File]::WriteAllText((Join-Path $dir $saida), $codigo + "`n", $utf8)
  return $codigo
}

function Avaliar($arquivo) {
  $gcc = (Get-Command gcc).Source
  $exe = [System.IO.Path]::GetTempFileName()
  $sem = & $gcc $arquivo -o $exe 2>&1
  $semCode = $LASTEXITCODE
  $comWall = & $gcc $arquivo -o $exe -Wall -Wextra 2>&1
  $wallCode = $LASTEXITCODE
  Remove-Item $exe -ErrorAction SilentlyContinue
  $nWarn = ($comWall | Where-Object { $_ -match 'warning:' }).Count
  $nErr = ($comWall | Where-Object { $_ -match 'error:' }).Count
  return @{ compila = ($semCode -eq 0); warnings = $nWarn; erros = $nErr; saidaWall = ($comWall -join "`n") }
}

Write-Host "=== TAREFA ===" -ForegroundColor Cyan
Write-Host $Desc
Write-Host ""

Write-Host ">>> MiniCPM5-1B (porta 8138, Q4_K_M, repeat-penalty 1.1)..." -ForegroundColor Yellow
$codigoMini = Chamar-Modelo 8138 'minicpm5-1b' $Desc $SaidaMini
if ($codigoMini) {
  $avMini = Avaliar (Join-Path $dir $SaidaMini)
  Write-Host "    Compila: $($avMini.compila)  | Warnings(-Wall): $($avMini.warnings)  | Erros: $($avMini.erros)"
  Write-Host "    Linhas geradas: $(($codigoMini -split "`n").Count)"
} else { Write-Host "    [FALHA] nao gerou bloco de codigo" -ForegroundColor Red }

Write-Host ""
Write-Host ">>> Ornith 9B (porta 8137, Q5_K_M, padrao ouro)..." -ForegroundColor Yellow
$codigoOrnith = Chamar-Modelo 8137 'ornith' $Desc $SaidaOrnith
if ($codigoOrnith) {
  $avOrn = Avaliar (Join-Path $dir $SaidaOrnith)
  Write-Host "    Compila: $($avOrn.compila)  | Warnings(-Wall): $($avOrn.warnings)  | Erros: $($avOrn.erros)"
  Write-Host "    Linhas geradas: $(($codigoOrnith -split "`n").Count)"
} else { Write-Host "    [FALHA] nao gerou bloco de codigo" -ForegroundColor Red }

Write-Host ""
Write-Host "=== RESUMO ===" -ForegroundColor Cyan
Write-Host "MiniCPM5:  compila=$($avMini.compila) warnings=$($avMini.warnings)"
Write-Host "Ornith:    compila=$($avOrn.compila) warnings=$($avOrn.warnings)"
