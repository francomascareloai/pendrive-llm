# comparar-revisao.ps1 - da o mesmo codigo bugado pros dois modelos corrigirem
$ErrorActionPreference = 'Continue'
$dir = 'E:\LLM\pendrive-backup-2026-07-08\projeto_teste'
Set-Location $dir

$codigoBugado = @'
#include <stdio.h>

int main() {
    int lista[] = {10, 20, 30};
    int n = sizeof(lista) / sizeof(lista[0]);

    for (int i = 0; i < n; i++) {
        printf("%d\n", lista[i]);
    }

    free(lista);

    return 0;
}
'@

$utf8 = New-Object System.Text.UTF8Encoding($false)
[System.IO.File]::WriteAllText((Join-Path $dir 'bug.c'), $codigoBugado + "`n", $utf8)

function Revisar($porta, $modelo, $saida) {
  $cb = '```'
  $system = 'Voce e um assistente especializado em C. O usuario pediu uma lista ligada mas o codigo abaixo usa array e chama free() em memoria nao alocada. Corrija transformando numa LISTA LIGADA de verdade (struct com ponteiro proximo, malloc, percorrer, liberar). Devolva o codigo C completo corrigido num bloco ' + $cb + 'c ... ' + $cb + '. Nao invente funcoes da biblioteca padrao.'
  $userMsg = "Corrija este codigo para ser uma lista ligada real:`n`n" + $cb + "c`n$codigoBugado`n" + $cb + "`n`nDevolva o codigo corrigido no bloco " + $cb + "c."
  $body = @{
    model = $modelo
    messages = @(@{role='system';content=$system}, @{role='user';content=$userMsg})
    stream = $false; temperature = 0.3; top_p = 0.95; max_tokens = 4096
  } | ConvertTo-Json -Depth 10 -Compress
  $resp = Invoke-RestMethod -Uri "http://127.0.0.1:$porta/v1/chat/completions" -Method Post -ContentType 'application/json; charset=utf-8' -Body $body -TimeoutSec 300
  $c = $resp.choices[0].message.content
  $m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*?)' + [regex]::Escape($cb))
  if (-not $m.Success) { $m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + '\s*\r?\n(.*?)' + [regex]::Escape($cb)) }
  if (-not $m.Success) { return $null }
  $codigo = $m.Groups[1].Value.Trim()
  [System.IO.File]::WriteAllText((Join-Path $dir $saida), $codigo + "`n", $utf8)
  return $codigo
}

function Avaliar($arquivo) {
  $gcc = (Get-Command gcc).Source
  $exe = [System.IO.Path]::GetTempFileName()
  $sem = & $gcc $arquivo -o $exe 2>&1; $semCode = $LASTEXITCODE
  $comWall = & $gcc $arquivo -o $exe -Wall -Wextra 2>&1; $wallCode = $LASTEXITCODE
  Remove-Item $exe -ErrorAction SilentlyContinue
  $nWarn = ($comWall | Where-Object { $_ -match 'warning:' }).Count
  $nErr = ($comWall | Where-Object { $_ -match 'error:' }).Count
  $temStruct = (Select-String -Path $arquivo -Pattern 'struct' -Quiet)
  $temMalloc = (Select-String -Path $arquivo -Pattern 'malloc' -Quiet)
  $temProximo = (Select-String -Path $arquivo -Pattern 'proximo|next|prox' -Quiet)
  return @{ compila = ($semCode -eq 0); warnings = $nWarn; erros = $nErr; struct = $temStruct; malloc = $temMalloc; ponteiro = $temProximo }
}

Write-Host ">>> MiniCPM5-1B revisando..." -ForegroundColor Yellow
$cm = Revisar 8138 'minicpm5-1b' 'rev_mini.c'
if ($cm) { $am = Avaliar (Join-Path $dir 'rev_mini.c'); Write-Host "    Compila: $($am.compila) | Warnings: $($am.warnings) | struct: $($am.struct) | malloc: $($am.malloc) | ponteiro: $($am.ponteiro)" }
else { Write-Host "    [FALHA] nao gerou bloco" -ForegroundColor Red }

Write-Host ""
Write-Host ">>> Ornith 9B revisando..." -ForegroundColor Yellow
$co = Revisar 8137 'ornith' 'rev_ornith.c'
if ($co) { $ao = Avaliar (Join-Path $dir 'rev_ornith.c'); Write-Host "    Compila: $($ao.compila) | Warnings: $($ao.warnings) | struct: $($ao.struct) | malloc: $($ao.malloc) | ponteiro: $($ao.ponteiro)" }
else { Write-Host "    [FALHA] nao gerou bloco" -ForegroundColor Red }
