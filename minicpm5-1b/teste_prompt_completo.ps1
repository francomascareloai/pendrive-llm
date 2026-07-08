$ErrorActionPreference = "Stop"
$Url = "http://127.0.0.1:8138/v1/chat/completions"
$dirScript = "E:\LLM\pendrive-backup-2026-07-08\minicpm5-1b"

# le o system prompt COMPLETO (com regras de tipagem + exemplos canonicos)
$sys = Get-Content -LiteralPath (Join-Path $dirScript 'prompts\sys_corpos.txt') -Raw -Encoding UTF8

$user = "Escreva um programa C completo: cadastro de alunos com menu. Struct Aluno com nome[50], matricula, nota. Array MAX 50. Menu: 1-Cadastrar, 2-Listar, 3-Buscar por matricula, 0-Sair. Devolva APENAS o codigo no bloco ```c."

$body = @{
  model = "minicpm5-1b"
  messages = @(
    @{ role = "system"; content = $sys }
    @{ role = "user";   content = $user }
  )
  stream = $false
  temperature = 0.3
  top_p = 0.95
  max_tokens = 2048
} | ConvertTo-Json -Depth 10 -Compress

Write-Host "Enviando (com prompt completo de tipagem)..."
$t0 = Get-Date
$resp = Invoke-RestMethod -Uri $Url -Method Post -ContentType "application/json; charset=utf-8" -Body $body -TimeoutSec 300
$t1 = Get-Date
$seg = [math]::Round(($t1 - $t0).TotalSeconds, 1)

$finish = $resp.choices[0].finish_reason
$tokGen = $resp.usage.completion_tokens
$tokPrompt = $resp.usage.prompt_tokens
Write-Host "Tempo: ${seg}s | Prompt: ${tokPrompt} tok | Gerados: ${tokGen} tok | Finish: ${finish}"
Write-Host "Tokens/s: $([math]::Round($tokGen / $seg, 1))"

$content = $resp.choices[0].message.content
$cb = [string][char]96 * 3
$m = [regex]::Match($content, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*?)' + [regex]::Escape($cb))
if (-not $m.Success) { $m = [regex]::Match($content, '(?s)' + [regex]::Escape($cb) + '\s*\r?\n(.*?)' + [regex]::Escape($cb)) }
if (-not $m.Success) { $m = [regex]::Match($content, '(?s)' + [regex]::Escape($cb) + 'c?\s*\r?\n(.*)$') }

if ($m.Success) {
  $codigo = $m.Groups[1].Value.Trim()
  $linhas = ($codigo -split "`n").Count
  $nv = ($codigo -split "`n" | Where-Object { $_.Trim() -ne "" }).Count
  Write-Host ""
  Write-Host "Linhas totais: $linhas | Nao-vazias: $nv"
  Write-Host ""
  Write-Host "===== CODIGO ====="
  Write-Host $codigo
  Write-Host "================="
  $utf8 = New-Object System.Text.UTF8Encoding($false)
  $out = Join-Path $dirScript "teste_prompt_completo.c"
  [System.IO.File]::WriteAllText($out, $codigo + "`n", $utf8)
  # compila
  $gcc = Get-Command gcc -ErrorAction SilentlyContinue
  if ($gcc) {
    $tmp = [System.IO.Path]::GetTempFileName()
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $gcc.Source
    $psi.Arguments = '"' + $out + '" -o "' + $tmp + '" -Wall 2>&1'
    $psi.UseShellExecute = $false
    $psi.RedirectStandardError = $true
    $psi.RedirectStandardOutput = $true
    $psi.CreateNoWindow = $true
    $p = [System.Diagnostics.Process]::Start($psi)
    $null = $p.StandardOutput.ReadToEnd()
    $err = $p.StandardError.ReadToEnd()
    $p.WaitForExit(15000) | Out-Null
    Remove-Item $tmp -ErrorAction SilentlyContinue
    Write-Host ""
    if ($p.ExitCode -eq 0) {
      Write-Host "[OK] Compilou sem erros (gcc -Wall)" -ForegroundColor Green
    } else {
      $nErr = ($err -split "`n" | Where-Object { $_ -match 'error:' }).Count
      $nWarn = ($err -split "`n" | Where-Object { $_ -match 'warning:' }).Count
      Write-Host "[FALHOU] Erros: $nErr | Warnings: $nWarn" -ForegroundColor Red
      ($err -split "`n" | Where-Object { $_ -match 'error:' }) | ForEach-Object { Write-Host "  $_" -ForegroundColor Red }
    }
  }
} else {
  Write-Host "NAO ACHOU BLOCO. Resposta bruta:"
  Write-Host $content
}
