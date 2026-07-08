# revisar.ps1 — manda um arquivo .c pro Ornith revisar/corrigir
# Uso:  powershell -ExecutionPolicy Bypass -File revisar.ps1 programa.c
#       (ou arraste o .c em cima de revisar.bat)
# Requer: run.bat rodando (servidor na porta 8137)
# Nao precisa de jq/node — so PowerShell 5.1 (ja vem no Windows)

param(
  [Parameter(Mandatory=$true, Position=0)]
  [string]$Arquivo,
  # -Salvar: sobrescreve o original sem perguntar (cria .bak). Default: pergunta.
  [switch]$Salvar,
  # -Novo: sempre salva como .corrigido.c sem perguntar. Default: pergunta.
  [switch]$Novo
)

$ErrorActionPreference = "Stop"
$Porta = 8137
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"

# sincroniza CurrentDirectory do .NET com a pasta atual do PowerShell.
# Sem isto, GetFullPath resolve caminhos relativos contra o dir de inicio do
# processo (nao atualizado por Set-Location/cd), e 'revisar.ps1 programa.c'
# falha com "arquivo nao encontrado" apos um cd. Mesmo fix do agente.ps1:33.
[System.IO.Directory]::SetCurrentDirectory((Get-Location).Path)

# resolve caminho absoluto
$Arquivo = [System.IO.Path]::GetFullPath($Arquivo)
if (-not (Test-Path -LiteralPath $Arquivo)) {
  Write-Host "[ERRO] Arquivo nao encontrado: $Arquivo" -ForegroundColor Red
  exit 1
}

$codigo = Get-Content -LiteralPath $Arquivo -Raw -Encoding UTF8

# tenta compilar pra pegar o erro do gcc (se gcc existir)
# stderr do gcc nao deve disparar erro — capturamos manualmente
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
  $errOut = $p.StandardError.ReadToEnd()
  $p.WaitForExit(15000) | Out-Null
  Remove-Item $tmpExe -ErrorAction SilentlyContinue
  if ($p.ExitCode -ne 0) {
    $erroGcc = $errOut.Trim()
  }
}

# monta o prompt
if ($erroGcc) {
  $tarefa = "Este codigo C tem erro de compilacao. Corrija-o."
  $extra = "`n`nErro do gcc:`n$erroGcc"
} else {
  $tarefa = "Revise este codigo C. Corrija bugs, vazamentos de memoria, erros de logica e melhore a qualidade mantendo a mesma funcionalidade. Se nao houver nada a corrigir, devolva o codigo igual."
  $extra = ""
}

$cb = '```'  # marcador de bloco de codigo
$system = 'Voce e um assistente especializado em C. Pense com cuidado no codigo - analise o erro do compilador, verifique logica, vazamentos de memoria, erros de sintaxe e off-by-one antes de corrigir. Devolva SEMPRE o codigo C completo e corrigido dentro de um unico bloco ' + $cb + 'c ... ' + $cb + '. Apos o bloco, escreva 1-3 linhas curtas em portugues explicando o que corrigiu.'
$userMsg = "$tarefa`n`nCodigo:`n" + $cb + "c`n$codigo`n" + $cb + "$extra`n`nDevolva o codigo corrigido no bloco " + $cb + "c."

# monta o JSON com ConvertTo-Json (escapa aspas/newlines corretamente)
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

# checa se servidor ta no ar
try {
  Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null
} catch {
  Write-Host "[ERRO] Servidor nao esta rodando na porta $Porta." -ForegroundColor Red
  Write-Host "       Abra run.bat primeiro (deixa a janela aberta)." -ForegroundColor Yellow
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

# avisa se a resposta foi truncada por limite de tokens (codigo corrigido incompleto)
$finish = $resp.choices[0].finish_reason
if ($finish -eq "length") {
  Write-Host "[AVISO] Resposta truncada por limite de tokens (finish_reason=length)." -ForegroundColor Yellow
  Write-Host "        O codigo corrigido pode estar incompleto. Considere aumentar o contexto (config.bat)." -ForegroundColor Yellow
}

# extrai o bloco de codigo. Regex unificada case-insensitive aceita ```c, ```C,
# ```cpp, ```c++ etc. ([a-z0-9+#]* apos o fence). Fallback: bloco sem fechar
# (resposta truncada por max_tokens) captura ate o final da string.
$cb = '```'  # close/backtick-block
$match = [regex]::Match($conteudo, '(?si)' + [regex]::Escape($cb) + '[a-z0-9+#]*\s*\r?\n(.*?)' + [regex]::Escape($cb))
if (-not $match.Success) {
  $match = [regex]::Match($conteudo, '(?si)' + [regex]::Escape($cb) + '[a-z0-9+#]*\s*\r?\n(.*)$')
}
if (-not $match.Success) {
  Write-Host "[AVISO] Nao achei bloco de codigo na resposta. Mostrando a resposta bruta:" -ForegroundColor Yellow
  Write-Host $conteudo
  exit 0
}

$codigoCorrigido = $match.Groups[1].Value.Trim()
$explicacao = $conteudo.Substring($match.Index + $match.Length).Trim()
# limpa tokens de fim de template que vazam (caso o modelo emita)
$explicacao = $explicacao -replace '<\|[^|]*\|>', ''
$explicacao = $explicacao.Trim()

# pergunta se salva
Write-Host ""
Write-Host "===== CODIGO CORRIGIDO =====" -ForegroundColor Green
Write-Host $codigoCorrigido
Write-Host "=============================" -ForegroundColor Green
if ($explicacao) {
  Write-Host ""
  Write-Host "Explicacao: $explicacao" -ForegroundColor DarkGray
}
Write-Host ""
# UTF8 sem BOM (System.IO.File nao adiciona BOM) — evita problema com compiladores
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
# decide como salvar: -Salvar sobrescreve, -Novo cria .corrigido.c, senao pergunta
$sobrescrever = $false
if ($Salvar) {
  $sobrescrever = $true
} elseif (-not $Novo) {
  $resp2 = Read-Host "Salvar por cima do arquivo original? (s/N)"
  if ($resp2 -eq "s" -or $resp2 -eq "S") { $sobrescrever = $true }
}
if ($sobrescrever) {
  # backup do original
  $bak = $Arquivo + ".bak"
  Copy-Item -LiteralPath $Arquivo -Destination $bak -Force
  [System.IO.File]::WriteAllText($Arquivo, $codigoCorrigido + "`n", $utf8NoBom)
  Write-Host "[OK] Salvo. Backup criado: $bak" -ForegroundColor Green
} else {
  # salva como .corrigido.c
  $novo = [System.IO.Path]::ChangeExtension($Arquivo, ".corrigido.c")
  [System.IO.File]::WriteAllText($novo, $codigoCorrigido + "`n", $utf8NoBom)
  Write-Host "[OK] Salvo em: $novo" -ForegroundColor Green
}
