# revisar.ps1 - manda um arquivo .c pro MiniCPM5-1B revisar/corrigir
# Uso:  powershell -ExecutionPolicy Bypass -File revisar.ps1 programa.c
#       (ou arraste o .c em cima de revisar.bat)
# Requer: run-minicpm.bat rodando (servidor na porta 8138)
# Versao 1B: temperatura 0.3 (baixa = mais determinista, menos alucinacao).

param(
  [Parameter(Mandatory=$true, Position=0)]
  [string]$Arquivo,
  [switch]$Salvar,
  [switch]$Novo
)

$ErrorActionPreference = "Stop"
$Porta = 8138
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"

$Arquivo = [System.IO.Path]::GetFullPath($Arquivo)
if (-not (Test-Path -LiteralPath $Arquivo)) {
  Write-Host "[ERRO] Arquivo nao encontrado: $Arquivo" -ForegroundColor Red
  exit 1
}

$codigo = Get-Content -LiteralPath $Arquivo -Raw -Encoding UTF8
if ([string]::IsNullOrWhiteSpace($codigo)) {
  Write-Host "[ERRO] Arquivo vazio ou ilegivel: $Arquivo" -ForegroundColor Red
  exit 1
}

# tenta compilar pra pegar o erro do gcc (se gcc existir)
$erroGcc = ""
$gcc = Get-Command gcc -ErrorAction SilentlyContinue
if ($gcc) {
  $tmpExe = [System.IO.Path]::GetTempFileName()
  $psi = New-Object System.Diagnostics.ProcessStartInfo
  $psi.FileName = $gcc.Source
  $psi.Arguments = '"' + $Arquivo + '" -o "' + $tmpExe + '"'
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
  if ($p.ExitCode -ne 0) {
    $erroGcc = $errOut.Trim()
  }
}

if ($erroGcc) {
  $tarefa = "Este codigo C tem erro de compilacao. Corrija-o."
  $extra = "`n`nErro do gcc:`n$erroGcc"
} else {
  $tarefa = "Revise este codigo C. Corrija bugs, vazamentos de memoria, erros de logica e melhore a qualidade mantendo a mesma funcionalidade. Se nao houver nada a corrigir, devolva o codigo igual."
  $extra = ""
}

$cb = '```'
$system = 'Voce e um assistente especializado em C. Pense com cuidado no codigo - analise o erro do compilador, verifique logica, vazamentos de memoria, erros de sintaxe e off-by-one antes de corrigir. Devolva SEMPRE o codigo C completo e corrigido dentro de um unico bloco ' + $cb + 'c ... ' + $cb + '. Nao invente funcoes da biblioteca padrao. Apos o bloco, escreva 1-3 linhas curtas em portugues explicando o que corrigiu.'
$userMsg = "$tarefa`n`nCodigo:`n" + $cb + "c`n$codigo`n" + $cb + "$extra`n`nDevolva o codigo corrigido no bloco " + $cb + "c."

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

try {
  Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null
} catch {
  Write-Host "[ERRO] Servidor nao esta rodando na porta $Porta." -ForegroundColor Red
  Write-Host "       Abra run-minicpm.bat primeiro (deixa a janela aberta)." -ForegroundColor Yellow
  exit 1
}

Write-Host "Enviando para revisao..." -ForegroundColor Cyan
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

$codigoCorrigido = $match.Groups[1].Value.Trim()
$explicacao = $conteudo.Substring($match.Index + $match.Length).Trim()
$explicacao = $explicacao -replace '<\|[^|]*\|>', ''
$explicacao = $explicacao.Trim()

Write-Host ""
Write-Host "===== CODIGO CORRIGIDO =====" -ForegroundColor Green
Write-Host $codigoCorrigido
Write-Host "=============================" -ForegroundColor Green
if ($explicacao) {
  Write-Host ""
  Write-Host "Explicacao: $explicacao" -ForegroundColor DarkGray
}
Write-Host ""
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$sobrescrever = $false
if ($Salvar) {
  $sobrescrever = $true
} elseif (-not $Novo) {
  $resp2 = Read-Host "Salvar por cima do arquivo original? (s/N)"
  if ($resp2 -eq "s" -or $resp2 -eq "S") { $sobrescrever = $true }
}
if ($sobrescrever) {
  $bak = $Arquivo + ".bak"
  Copy-Item -LiteralPath $Arquivo -Destination $bak -Force
  [System.IO.File]::WriteAllText($Arquivo, $codigoCorrigido + "`n", $utf8NoBom)
  Write-Host "[OK] Salvo. Backup criado: $bak" -ForegroundColor Green
} else {
  $novo = [System.IO.Path]::ChangeExtension($Arquivo, ".corrigido.c")
  [System.IO.File]::WriteAllText($novo, $codigoCorrigido + "`n", $utf8NoBom)
  Write-Host "[OK] Salvo em: $novo" -ForegroundColor Green
}
