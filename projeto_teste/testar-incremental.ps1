# testar-incremental.ps1 - testa construir por partes (funcao isolada com struct dada)
$dir = 'E:\LLM\pendrive-backup-2026-07-08\projeto_teste'
Set-Location $dir

$system = 'Voce e um assistente especializado em C. Devolva SEMPRE o codigo C completo e pronto para compilar dentro de um unico bloco ```c ... ```. Nao invente funcoes da biblioteca padrao. Nao escreva texto depois do bloco.'

# Da a struct e o array, pede SO a funcao cadastrar
$contexto = @"
Ja tenho este codigo base (NAO reescreva):

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 50

typedef struct {
    char nome[50];
    int matricula;
    float nota;
} Aluno;

Aluno alunos[MAX_ALUNOS];
int num_alunos = 0;
```

Escreva APENAS a funcao `void cadastrar()` que le nome, matricula e nota do usuario com scanf e adiciona no array global `alunos` (incrementando `num_alunos`). Nao reescreva o codigo base, so a funcao. Devolva no bloco ```c.
"@

$body = @{
  model = 'minicpm5-1b'
  messages = @(@{role='system';content=$system}, @{role='user';content=$contexto})
  stream = $false; temperature = 0.3; top_p = 0.95; max_tokens = 2048
} | ConvertTo-Json -Depth 10 -Compress

Write-Host "Pedindo SO a funcao cadastrar()..." -ForegroundColor Cyan
$resp = Invoke-RestMethod -Uri 'http://127.0.0.1:8138/v1/chat/completions' -Method Post -ContentType 'application/json; charset=utf-8' -Body $body -TimeoutSec 300
$c = $resp.choices[0].message.content
$cb = '```'
$m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*?)' + [regex]::Escape($cb))
if (-not $m.Success) { $m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + '\s*\r?\n(.*?)' + [regex]::Escape($cb)) }
if (-not $m.Success) { Write-Host "[FALHA] sem bloco" -ForegroundColor Red; Write-Host $c; exit 1 }

$func = $m.Groups[1].Value.Trim()
# monta programa completo = base + funcao + main de teste
$base = @"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 50

typedef struct {
    char nome[50];
    int matricula;
    float nota;
} Aluno;

Aluno alunos[MAX_ALUNOS];
int num_alunos = 0;

$func

int main() {
    cadastrar();
    printf("Cadastrado: %s %d %.1f\n", alunos[0].nome, alunos[0].matricula, alunos[0].nota);
    return 0;
}
"@
$utf8 = New-Object System.Text.UTF8Encoding($false)
$arq = Join-Path $dir 'incremental.c'
[System.IO.File]::WriteAllText($arq, $base + "`n", $utf8)

$gcc = (Get-Command gcc).Source
$exe = [System.IO.Path]::GetTempFileName()
$null = & $gcc $arq -o $exe 2>$null
$compila = ($LASTEXITCODE -eq 0)
$comWall = & $gcc $arq -o $exe -Wall -Wextra 2>&1
$nWarn = ($comWall | Where-Object { $_ -match 'warning:' }).Count
$nErr = ($comWall | Where-Object { $_ -match 'error:' }).Count
Remove-Item $exe -ErrorAction SilentlyContinue

Write-Host ""
Write-Host "=== FUNCAO GERADA ===" -ForegroundColor Cyan
Write-Host $func
Write-Host ""
Write-Host "=== AVALIACAO (base + funcao + main de teste) ===" -ForegroundColor Cyan
Write-Host "Compila: $compila | Warnings: $nWarn | Erros: $nErr"
if (-not $compila) {
  Write-Host ""
  Write-Host "=== ERROS ===" -ForegroundColor Yellow
  $comWall | Where-Object { $_ -match 'error:' } | Select-Object -First 5
}
