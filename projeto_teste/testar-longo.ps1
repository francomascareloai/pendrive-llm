# testar-longo.ps1 - testa se o MiniCPM5 gera programa longo de uma vez
$dir = 'E:\LLM\pendrive-backup-2026-07-08\projeto_teste'
Set-Location $dir

$system = 'Voce e um assistente especializado em C. Devolva SEMPRE o codigo C completo e pronto para compilar dentro de um unico bloco ```c ... ```. Inclua todos os #include necessarios. Nao invente funcoes da biblioteca padrao. Nao escreva texto depois do bloco.'

# Tarefa longa e estruturada (nivel prova)
$desc = @"
Um sistema de cadastro de alunos em C com menu interativo. Deve ter:
1. Um struct Aluno com nome (char[50]), matricula (int) e nota (float).
2. Um array global de ate 50 alunos e um contador.
3. Funcao cadastrar() que le os dados do usuario e adiciona no array.
4. Funcao listar() que imprime todos os alunos cadastrados.
5. Funcao buscar_por_matricula() que recebe uma matricula e imprime o aluno ou "nao encontrado".
6. Funcao salvar_arquivo() que grava todos os alunos num arquivo binario "alunos.dat".
7. Funcao carregar_arquivo() que le o arquivo binario ao iniciar.
8. Um menu switch-case no main com opcoes 1-cadastrar, 2-listar, 3-buscar, 4-salvar, 5-carregar, 0-sair.
"@

$body = @{
  model = 'minicpm5-1b'
  messages = @(@{role='system';content=$system}, @{role='user';content="Escreva um programa C completo que: $desc`n`nDevolva o codigo no bloco ```c."})
  stream = $false; temperature = 0.3; top_p = 0.95; max_tokens = 8192
} | ConvertTo-Json -Depth 10 -Compress

Write-Host "Gerando sistema de cadastro (tarefa longa)..." -ForegroundColor Cyan
$resp = Invoke-RestMethod -Uri 'http://127.0.0.1:8138/v1/chat/completions' -Method Post -ContentType 'application/json; charset=utf-8' -Body $body -TimeoutSec 600
$c = $resp.choices[0].message.content
$tokens = $resp.usage.completion_tokens
Write-Host "Tokens gerados: $tokens" -ForegroundColor DarkGray

$cb = '```'
$m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + 'c\s*\r?\n(.*?)' + [regex]::Escape($cb))
if (-not $m.Success) { $m = [regex]::Match($c, '(?s)' + [regex]::Escape($cb) + '\s*\r?\n(.*?)' + [regex]::Escape($cb)) }
if (-not $m.Success) { Write-Host "[FALHA] sem bloco de codigo" -ForegroundColor Red; Write-Host $c; exit 1 }

$codigo = $m.Groups[1].Value.Trim()
$utf8 = New-Object System.Text.UTF8Encoding($false)
$arq = Join-Path $dir 'cadastro.c'
[System.IO.File]::WriteAllText($arq, $codigo + "`n", $utf8)

$linhas = ($codigo -split "`n").Count
Write-Host "Linhas geradas: $linhas" -ForegroundColor Cyan

# Avalia
$gcc = (Get-Command gcc).Source
$exe = [System.IO.Path]::GetTempFileName()
$null = & $gcc $arq -o $exe 2>$null
$compila = ($LASTEXITCODE -eq 0)
$comWall = & $gcc $arq -o $exe -Wall -Wextra 2>&1
$nWarn = ($comWall | Where-Object { $_ -match 'warning:' }).Count
$nErr = ($comWall | Where-Object { $_ -match 'error:' }).Count
Remove-Item $exe -ErrorAction SilentlyContinue

Write-Host ""
Write-Host "=== AVALIACAO ===" -ForegroundColor Cyan
Write-Host "Compila:        $compila"
Write-Host "Warnings (-Wall): $nWarn"
Write-Host "Erros:          $nErr"
Write-Host "Tem struct:     $(Select-String -Path $arq -Pattern 'struct' -Quiet)"
Write-Host "Tem menu switch: $(Select-String -Path $arq -Pattern 'switch' -Quiet)"
Write-Host "Tem fopen:      $(Select-String -Path $arq -Pattern 'fopen' -Quiet)"
Write-Host "Tem fwrite:     $(Select-String -Path $arq -Pattern 'fwrite|fprintf' -Quiet)"
Write-Host "Tem fread:      $(Select-String -Path $arq -Pattern 'fread|fscanf' -Quiet)"
Write-Host "Tem buscar:     $(Select-String -Path $arq -Pattern 'buscar' -Quiet)"
Write-Host ""
Write-Host "=== ERROS DO GCC (se houver) ===" -ForegroundColor Yellow
$comWall | Where-Object { $_ -match 'error:|warning:' } | Select-Object -First 10
