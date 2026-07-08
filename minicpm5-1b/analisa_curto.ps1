$resp = Get-Content 'E:\LLM\pendrive-backup-2026-07-08\minicpm5-1b\resp_curto.json' -Raw | ConvertFrom-Json
$finish = $resp.choices[0].finish_reason
$tokens = $resp.usage.completion_tokens
$promptTokens = $resp.usage.prompt_tokens
Write-Host "Finish reason:  $finish"
Write-Host "Prompt tokens:  $promptTokens"
Write-Host "Generated tokens: $tokens"
Write-Host ""

$content = $resp.choices[0].message.content
$cb = [string][char]96 + [string][char]96 + [string][char]96
$match = [regex]::Match($content, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*?)' + [regex]::Escape($cb))
if (-not $match.Success) {
  $match = [regex]::Match($content, '(?s)' + [regex]::Escape($cb) + '\s*\r?\n(.*?)' + [regex]::Escape($cb))
}
if (-not $match.Success) {
  $match = [regex]::Match($content, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*)$')
}
if ($match.Success) {
  $codigo = $match.Groups[1].Value.Trim()
  $linhas = ($codigo -split "`n").Count
  $naoVazias = ($codigo -split "`n" | Where-Object { $_.Trim() -ne "" }).Count
  Write-Host "Total de linhas:    $linhas"
  Write-Host "Linhas nao-vazias:  $naoVazias"
  Write-Host ""
  Write-Host "===== CODIGO GERADO ====="
  Write-Host $codigo
  Write-Host "========================="
  # salva
  $utf8 = New-Object System.Text.UTF8Encoding($false)
  [System.IO.File]::WriteAllText('E:\LLM\pendrive-backup-2026-07-08\minicpm5-1b\teste_curto_gerado.c', $codigo + "`n", $utf8)
  Write-Host ""
  Write-Host "[OK] Salvo em teste_curto_gerado.c"
  # compila
  $gcc = Get-Command gcc -ErrorAction SilentlyContinue
  if ($gcc) {
    $tmpExe = [System.IO.Path]::GetTempFileName()
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $gcc.Source
    $psi.Arguments = '"E:\LLM\pendrive-backup-2026-07-08\minicpm5-1b\teste_curto_gerado.c" -o "' + $tmpExe + '" -Wall 2>&1'
    $psi.UseShellExecute = $false
    $psi.RedirectStandardError = $true
    $psi.RedirectStandardOutput = $true
    $psi.CreateNoWindow = $true
    $p = [System.Diagnostics.Process]::Start($psi)
    $null = $p.StandardOutput.ReadToEnd()
    $errOut = $p.StandardError.ReadToEnd()
    $p.WaitForExit(15000) | Out-Null
    Remove-Item $tmpExe -ErrorAction SilentlyContinue
    Write-Host ""
    Write-Host "===== COMPILACAO (gcc -Wall) ====="
    if ($p.ExitCode -eq 0) {
      Write-Host "[OK] Compilou sem erros!" -ForegroundColor Green
    } else {
      $erros = ($errOut -split "`n" | Where-Object { $_ -match 'error:' })
      $warnings = ($errOut -split "`n" | Where-Object { $_ -match 'warning:' })
      Write-Host "[FALHOU] Erros: $($erros.Count) | Warnings: $($warnings.Count)" -ForegroundColor Red
      $erros | ForEach-Object { Write-Host "  $_" -ForegroundColor Red }
    }
  }
} else {
  Write-Host "NAO ACHOU BLOCO DE CODIGO"
  Write-Host $content
}
