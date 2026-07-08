# testar-params.ps1 - testa diferentes parametros de sampling no MiniCPM5
$dir = 'E:\LLM\pendrive-backup-2026-07-08\projeto_teste'
Set-Location $dir

$desc = 'lista ligada simples em C: cria 3 nos com valores 10, 20 e 30, percorre a lista imprimindo os valores e libera a memoria no final'

$system = 'Voce e um assistente especializado em C. Devolva SEMPRE o codigo C completo e pronto para compilar dentro de um unico bloco ```c ... ```. Inclua todos os #include necessarios e a funcao main. Nao invente funcoes da biblioteca padrao. Antes do bloco, 1-2 linhas curtas em portugues. Nao escreva texto depois do bloco.'
$userMsg = "Escreva um programa C completo que: $desc`n`nDevolva o codigo no bloco ```c."

$configs = @(
  @{ nome = 'A: temp0.3 top0.95 rp1.1 (atual)'; temp = 0.3; top = 0.95; rp = 1.1; pp = 0.0 }
  @{ nome = 'B: temp0.2 top0.9 rp1.2 pp0.3';      temp = 0.2; top = 0.9;  rp = 1.2; pp = 0.3 }
  @{ nome = 'C: temp0.1 top0.85 rp1.3 pp0.5';     temp = 0.1; top = 0.85; rp = 1.3; pp = 0.5 }
)

$utf8 = New-Object System.Text.UTF8Encoding($false)
$gcc = (Get-Command gcc).Source

foreach ($cfg in $configs) {
  Write-Host ""
  Write-Host ">>> $($cfg.nome)" -ForegroundColor Yellow
  $body = @{
    model = 'minicpm5-1b'
    messages = @(@{role='system';content=$system}, @{role='user';content=$userMsg})
    stream = $false; temperature = $cfg.temp; top_p = $cfg.top; max_tokens = 4096
    repeat_penalty = $cfg.rp; presence_penalty = $cfg.pp
  } | ConvertTo-Json -Depth 10 -Compress
  try {
    $resp = Invoke-RestMethod -Uri 'http://127.0.0.1:8138/v1/chat/completions' -Method Post -ContentType 'application/json; charset=utf-8' -Body $body -TimeoutSec 300
  } catch { Write-Host "    [ERRO API] $($_.Exception.Message)" -ForegroundColor Red; continue }
  $c = $resp.choices[0].message.content
  $cb = '```'
  $m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*?)' + [regex]::Escape($cb))
  if (-not $m.Success) { $m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + '\s*\r?\n(.*?)' + [regex]::Escape($cb)) }
  if (-not $m.Success) { Write-Host "    [FALHA] sem bloco de codigo" -ForegroundColor Red; continue }
  $codigo = $m.Groups[1].Value.Trim()
  $arq = Join-Path $dir ("param_$($cfg.nome.Split(':')[0]).c")
  [System.IO.File]::WriteAllText($arq, $codigo + "`n", $utf8)
  $exe = [System.IO.Path]::GetTempFileName()
  $null = & $gcc $arq -o $exe 2>$null
  $compila = ($LASTEXITCODE -eq 0)
  $comWall = & $gcc $arq -o $exe -Wall -Wextra 2>&1
  $nWarn = ($comWall | Where-Object { $_ -match 'warning:' }).Count
  $temStruct = (Select-String -Path $arq -Pattern 'struct' -Quiet)
  $temMalloc = (Select-String -Path $arq -Pattern 'malloc' -Quiet)
  $temPont = (Select-String -Path $arq -Pattern 'proximo|next|prox' -Quiet)
  Remove-Item $exe -ErrorAction SilentlyContinue
  Write-Host "    Compila: $compila | Warnings: $nWarn | struct: $temStruct | malloc: $temMalloc | ponteiro: $temPont | linhas: $(($codigo -split "`n").Count)"
}
