# chat.ps1 - chat direto com o MiniCPM5-1B (sem TUI, sem agente)
# Bate direto na API do llama-server (porta 8138).
# Versao 1B: temperatura 0.3 (baixa = mais determinista, menos alucinacao).
# Uso: chat.bat

$ErrorActionPreference = "Stop"
Set-Location -LiteralPath $PSScriptRoot

$Porta = 8138
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"

# checa servidor
try {
  Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null
} catch {
  Write-Host "[ERRO] Servidor nao esta rodando na porta $Porta." -ForegroundColor Red
  Write-Host "       Abra run-minicpm.bat primeiro (deixa a janela aberta)." -ForegroundColor Yellow
  Read-Host "Pressione Enter para sair"
  exit 1
}

$system = 'Voce e o MiniCPM5, um assistente de programacao em C. Responda em portugues do Brasil. Seja direto e conciso. Para tarefas de codigo (escrever, explicar, depurar programa C), seja preciso e coloque o codigo num bloco ```c. Nao invente funcoes da biblioteca padrao.'

# historico da conversa (mantem contexto)
$historico = @()

Clear-Host
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  MiniCPM5-1B Chat  (porta $Porta, temp 0.3)" -ForegroundColor Cyan
Write-Host "  Digite 'sair' para encerrar." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

while ($true) {
  Write-Host -NoNewline "> " -ForegroundColor Green
  $msg = Read-Host
  if ($null -eq $msg) { continue }
  $msg = $msg.Trim()
  if ($msg -eq "") { continue }
  if ($msg -eq "sair" -or $msg -eq "exit" -or $msg -eq "quit") { break }

  $historico += @{ role = "user"; content = $msg }

  $body = @{
    model = "minicpm5-1b"
    messages = @(@{ role = "system"; content = $system }) + $historico
    stream = $false
    temperature = 0.3
    top_p = 0.95
    max_tokens = 4096
  } | ConvertTo-Json -Depth 10 -Compress

  try {
    $resp = Invoke-RestMethod -Uri $Url -Method Post -ContentType "application/json; charset=utf-8" -Body $body -TimeoutSec 600
  } catch {
    Write-Host "[ERRO] Falha ao chamar o modelo: $($_.Exception.Message)" -ForegroundColor Red
    continue
  }

  $resposta = $resp.choices[0].message.content
  $historico += @{ role = "assistant"; content = $resposta }

  Write-Host ""
  Write-Host $resposta -ForegroundColor White
  Write-Host ""
}

Write-Host ""
Write-Host "Ate logo!" -ForegroundColor Cyan
