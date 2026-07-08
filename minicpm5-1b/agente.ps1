# agente.ps1 - agente de terminal que faz tarefas sozinho (ler/editar/compilar/rodar)
# Versao MiniCPM5-1B. Loop de tool calling direto na API. Sem timeout agressivo (suporta CPU lenta).
# Portatil: nada em %USERPROFILE%. Tudo no pendrive.
# Uso: agente.bat  (depois rode: "corrija o soma.c", "crie um programa que...", etc.)
# Requer: run-minicpm.bat rodando (servidor na porta 8138)

param(
  [Parameter(ValueFromRemainingArguments=$true)]
  [string[]]$TarefaArgs
)

$ErrorActionPreference = "Continue"
$Porta = 8138
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"

$tarefaInicial = ""
if ($TarefaArgs -and $TarefaArgs.Count -gt 0) {
  $tarefaInicial = ($TarefaArgs -join " ").Trim()
}
$modoInterativo = [string]::IsNullOrEmpty($tarefaInicial)

# sincroniza CurrentDirectory do .NET com a pasta atual do PowerShell
[System.IO.Directory]::SetCurrentDirectory((Get-Location).Path)

function Resolver-Caminho([string]$caminho) {
  if ([string]::IsNullOrWhiteSpace($caminho)) { return $caminho }
  if ([System.IO.Path]::IsPathRooted($caminho)) {
    return [System.IO.Path]::GetFullPath($caminho)
  }
  return [System.IO.Path]::GetFullPath([System.IO.Path]::Combine([System.IO.Directory]::GetCurrentDirectory(), $caminho))
}

# checa servidor
try {
  Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 5 | Out-Null
} catch {
  Write-Host "[ERRO] Servidor nao esta rodando na porta $Porta." -ForegroundColor Red
  Write-Host "       Abra run-minicpm.bat primeiro (deixa a janela aberta)." -ForegroundColor Yellow
  if ($modoInterativo) { Read-Host "Pressione Enter para sair" }
  exit 1
}

# --- ferramentas que o agente pode usar ---
$tools = @(
  @{
    type = "function"
    function = @{
      name = "ler_arquivo"
      description = "Le o conteudo de um arquivo do disco. Use para inspecionar codigo antes de corrigir."
      parameters = @{
        type = "object"
        properties = @{ caminho = @{ type = "string"; description = "caminho do arquivo (absoluto ou relativo a pasta atual)" } }
        required = @("caminho")
      }
    }
  },
  @{
    type = "function"
    function = @{
      name = "escrever_arquivo"
      description = "Escreve/cria um arquivo no disco com o conteudo dado. Use para criar ou corrigir arquivos."
      parameters = @{
        type = "object"
        properties = @{
          caminho = @{ type = "string"; description = "caminho do arquivo" }
          conteudo = @{ type = "string"; description = "conteudo completo do arquivo" }
        }
        required = @("caminho", "conteudo")
      }
    }
  },
  @{
    type = "function"
    function = @{
      name = "listar_arquivos"
      description = "Lista arquivos e pastas num diretorio."
      parameters = @{
        type = "object"
        properties = @{ pasta = @{ type = "string"; description = "caminho da pasta (default: pasta atual)" } }
      }
    }
  },
  @{
    type = "function"
    function = @{
      name = "rodar_comando"
      description = "Roda um comando no shell (cmd). Use para compilar (gcc) e executar programas."
      parameters = @{
        type = "object"
        properties = @{ comando = @{ type = "string"; description = "comando a executar" } }
        required = @("comando")
      }
    }
  }
)

function Executar-Tool($nome, $toolArgs) {
  # acesso seguro a propriedades do PSCustomObject: retorna '' se o modelo omitir o parametro.
  function Get-Arg($obj, $prop) {
    if ($null -eq $obj) { return '' }
    if ($obj.PSObject.Properties.Name -contains $prop) { return [string]$obj.$prop }
    return ''
  }
  try {
    switch ($nome) {
      "ler_arquivo" {
        $caminho = Resolver-Caminho (Get-Arg $toolArgs 'caminho')
        if (-not $caminho) { return "[ERRO] parametro 'caminho' ausente" }
        if (-not (Test-Path -LiteralPath $caminho)) { return "[ERRO] arquivo nao encontrado: $caminho" }
        $conteudo = Get-Content -LiteralPath $caminho -Raw -Encoding UTF8
        if ($null -eq $conteudo) { return "[OK] arquivo vazio: $caminho" }
        return [string]$conteudo
      }
      "escrever_arquivo" {
        $caminho = Resolver-Caminho (Get-Arg $toolArgs 'caminho')
        $conteudo = Get-Arg $toolArgs 'conteudo'
        if (-not $caminho) { return "[ERRO] parametro 'caminho' ausente" }
        $dir = [System.IO.Path]::GetDirectoryName($caminho)
        if (-not (Test-Path $dir)) { New-Item -ItemType Directory -Force -Path $dir | Out-Null }
        $utf8 = New-Object System.Text.UTF8Encoding($false)
        [System.IO.File]::WriteAllText($caminho, $conteudo, $utf8)
        return "[OK] arquivo salvo: $caminho"
      }
      "listar_arquivos" {
        $pastaArg = Get-Arg $toolArgs 'pasta'
        $pasta = if ($pastaArg) { Resolver-Caminho $pastaArg } else { (Get-Location).Path }
        if (-not (Test-Path -LiteralPath $pasta)) { return "[ERRO] pasta nao encontrada: $pasta" }
        $itens = Get-ChildItem -LiteralPath $pasta | Select-Object @{n='nome';e={$_.Name}}, @{n='tipo';e={if($_.PSIsContainer){'pasta'}else{'arquivo'}}}, @{n='tamanho';e={$_.Length}}
        $s = ($itens | Format-Table -AutoSize | Out-String)
        if ([string]::IsNullOrWhiteSpace($s)) { return "[OK] pasta vazia: $pasta" }
        return $s
      }
      "rodar_comando" {
        $cmd = Get-Arg $toolArgs 'comando'
        if (-not $cmd) { return "[ERRO] parametro 'comando' ausente" }
        Write-Host "  [exec] $cmd" -ForegroundColor DarkGray
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = "cmd.exe"
        $psi.Arguments = "/c " + $cmd
        $psi.UseShellExecute = $false
        $psi.RedirectStandardInput = $true
        $psi.RedirectStandardOutput = $true
        $psi.RedirectStandardError = $true
        $psi.CreateNoWindow = $true
        $psi.WorkingDirectory = (Get-Location).Path
        $p = [System.Diagnostics.Process]::Start($psi)
        # Fecha stdin: programas com scanf/gets recebem EOF e retornam em vez de
        # travar aguardando teclado. Sem isto, ReadToEnd bloqueia pra sempre.
        $p.StandardInput.Close()
        # Le assincrono evita o deadlock de buffer de pipe (stdout/stderr cheios
        # bloqueando o processo) e deixa o WaitForExit(30000) ser alcancado.
        $sb = New-Object System.Text.StringBuilder
        $errSb = New-Object System.Text.StringBuilder
        $outh = Register-ObjectEvent -InputObject $p -EventName OutputDataReceived -MessageData $sb -Action { if ($EventArgs.Data) { [void]$MessageData.AppendLine($EventArgs.Data) } }
        $errh = Register-ObjectEvent -InputObject $p -EventName ErrorDataReceived -MessageData $errSb -Action { if ($EventArgs.Data) { [void]$MessageData.AppendLine($EventArgs.Data) } }
        $p.BeginOutputReadLine()
        $p.BeginErrorReadLine()
        $timeout = $false
        if (-not $p.WaitForExit(30000)) {
          $timeout = $true
          try { $p.Kill() } catch {}
        }
        Unregister-Event -SourceIdentifier $outh.Name -ErrorAction SilentlyContinue
        Unregister-Event -SourceIdentifier $errh.Name -ErrorAction SilentlyContinue
        $outh | Remove-Job -Force -ErrorAction SilentlyContinue
        $errh | Remove-Job -Force -ErrorAction SilentlyContinue
        $stdout = $sb.ToString()
        $stderr = $errSb.ToString()
        if ($timeout) {
          return "[ERRO] comando excedeu 30s (provavelmente interativo/travou). Processo morto."
        }
        $saida = $stdout
        if ($stderr) { $saida += "`n" + $stderr }
        $saida += "`n[codigo de saida: $($p.ExitCode)]"
        if ([string]::IsNullOrWhiteSpace($stdout) -and [string]::IsNullOrWhiteSpace($stderr)) {
          $saida = "[OK] comando executado (sem saida). [codigo de saida: $($p.ExitCode)]"
        }
        $p.Dispose()
        return $saida
      }
      default { return "[ERRO] ferramenta desconhecida: $nome" }
    }
  } catch {
    return "[ERRO] $($_.Exception.Message)"
  }
}

# janela deslizante do historico. Em sessoes longas o historico cresce sem
# limite e estoura a janela de contexto do modelo (8192 tokens no 1B). Esta
# funcao poda do inicio mantendo as ultimas N mensagens, avancando o corte ate
# cair numa mensagem 'user'/'assistant' (nunca corta no meio de um par
# tool_call/tool_result, quebraria o protocolo). O system prompt e sempre
# reenviado a parte.
function Compactar-Historico {
  param([array]$Hist, [int]$MaxMensagens = 30)
  if ($null -eq $Hist -or $Hist.Count -le $MaxMensagens) { return $Hist }
  $corte = $Hist.Count - $MaxMensagens
  while ($corte -lt $Hist.Count -and $Hist[$corte].role -eq 'tool') { $corte++ }
  return $Hist[$corte..($Hist.Count - 1)]
}

$system = @"
Voce e o MiniCPM5, um agente de programacao em C que opera no terminal Windows (cmd).
Responda em portugues do Brasil.
Voce tem ferramentas: ler_arquivo, escrever_arquivo, listar_arquivos, rodar_comando.
Para qualquer tarefa de codigo: PRIMEIRO leia os arquivos relevantes, depois aja.
Para corrigir um arquivo: leia-o, analise, escreva a versao corrigida com escrever_arquivo, depois compile e rode para confirmar.
IMPORTANTE - comandos no Windows (cmd):
- Compilar: gcc arquivo.c -o arquivo.exe
- Rodar o executavel: .\arquivo.exe   (PRECISA do .\ antes do nome, senao nao acha)
- Listar arquivos: dir
- Nao use comandos Linux (ls, cat). Use dir, type.
Seja preciso e direto. Use as ferramentas quantas vezes precisar.
Quando terminar a tarefa, de uma resposta curta em portugues explicando o que fez.
"@

$historico = @()

if ($modoInterativo) {
  Clear-Host
  Write-Host "========================================" -ForegroundColor Cyan
  Write-Host "  MiniCPM5-1B Agente  (porta $Porta)" -ForegroundColor Cyan
  Write-Host "  Digite 'sair' para encerrar." -ForegroundColor Cyan
  Write-Host "  Ex: 'corrija o soma.c' / 'crie um programa que ordene 5 numeros'" -ForegroundColor Cyan
  Write-Host "========================================" -ForegroundColor Cyan
  Write-Host "  Pasta de trabalho: $((Get-Location).Path)" -ForegroundColor DarkGray
  Write-Host "  (o agente le/edita/compila arquivos nesta pasta)" -ForegroundColor DarkGray
  Write-Host ""
}

$fila = @()
if (-not [string]::IsNullOrEmpty($tarefaInicial)) { $fila += $tarefaInicial }

while ($true) {
  if ($fila.Count -gt 0) {
    $msg = $fila[0]
    # slice robusto: 1 elemento -> fila vazia (slice [1..1] em PS 5.1 retorna $null, quebra iteracao).
    if ($fila.Count -gt 1) { $fila = $fila[1..($fila.Count - 1)] } else { $fila = @() }
    if ($modoInterativo) { Write-Host "> $msg" -ForegroundColor Green }
  } else {
    if (-not $modoInterativo) { break }
    Write-Host -NoNewline "> " -ForegroundColor Green
    $msg = Read-Host
    if ($null -eq $msg) { continue }
    $msg = $msg.Trim()
    if ($msg -eq "") { continue }
    if ($msg -eq "sair" -or $msg -eq "exit" -or $msg -eq "quit") { break }
  }

  $historico += @{ role = "user"; content = $msg }

  # loop de tool calling: o modelo pode chamar varias ferramentas em sequencia
  $turno = 0
  while ($turno -lt 15) {
    $turno++
    # poda o historico se crescer demais (mantem as ultimas 30 mensagens)
    $historico = Compactar-Historico $historico
    $body = @{
      model = "minicpm5-1b"
      messages = @(@{ role = "system"; content = $system }) + $historico
      tools = $tools
      tool_choice = "auto"
      stream = $false
      temperature = 0.3
      top_p = 0.95
      max_tokens = 2048
    } | ConvertTo-Json -Depth 20 -Compress

    if ($modoInterativo) { Write-Host "pensando..." -ForegroundColor DarkGray }
    # retry com backoff: o servidor pode cair no meio de uma geracao (OOM em
    # PC com pouca RAM). Tenta ate 3x com pausa crescente. Se o servidor morreu,
    # avisa e sai do loop de tool calling (nao adianta insistir).
    $resp = $null
    for ($tent = 1; $tent -le 3; $tent++) {
      try {
        $resp = Invoke-RestMethod -Uri $Url -Method Post -ContentType "application/json; charset=utf-8" -Body $body -TimeoutSec 600
        break
      } catch {
        if ($tent -lt 3) {
          $espera = $tent * 5
          Write-Host "[AVISO] Servidor nao respondeu (tentativa $tent/3). Aguardando ${espera}s..." -ForegroundColor Yellow
          Start-Sleep -Seconds $espera
          try { Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 3 | Out-Null }
          catch {
            Write-Host "[ERRO] O servidor caiu (provavelmente RAM insuficiente)." -ForegroundColor Red
            Write-Host "       Feche o run-minicpm.bat, feche outros programas pesados, e suba o servidor de novo." -ForegroundColor Yellow
            $resp = $null
            break
          }
        } else {
          Write-Host "[ERRO] Falha ao chamar o modelo apos 3 tentativas: $($_.Exception.Message)" -ForegroundColor Red
          Write-Host "       Se o servidor caiu, reinicie o run-minicpm.bat (feche outros programas pesados)." -ForegroundColor Yellow
        }
      }
    }
    if (-not $resp) { break }

    $tc = $resp.choices[0].message.tool_calls
    if ($null -eq $tc -or $tc.Count -eq 0) {
      $resposta = $resp.choices[0].message.content
      $historico += @{ role = "assistant"; content = $resposta }
      Write-Host ""
      Write-Host $resposta -ForegroundColor White
      Write-Host ""
      break
    }

    # converte a message pra hashtable com valores [string] (evita deadlock do ConvertTo-Json no PS 5.1)
    $msgRaw = $resp.choices[0].message
    $msgLimpa = @{
      role = "assistant"
      content = [string]$msgRaw.content
    }
    if ($msgRaw.PSObject.Properties.Name -contains "reasoning_content") {
      $msgLimpa["reasoning_content"] = [string]$msgRaw.reasoning_content
    }
    if ($msgRaw.tool_calls) {
      $callsLimpos = @()
      foreach ($c in $msgRaw.tool_calls) {
        $callsLimpos += @{
          type = [string]$c.type
          function = @{ name = [string]$c.function.name; arguments = [string]$c.function.arguments }
          id = [string]$c.id
        }
      }
      $msgLimpa["tool_calls"] = $callsLimpos
    }
    $historico += $msgLimpa

    foreach ($call in $tc) {
      $fn = [string]$call.function.name
      Write-Host "  [tool] $fn" -ForegroundColor Yellow
      try {
        $toolArgs = $call.function.arguments | ConvertFrom-Json
      } catch {
        $toolArgs = [PSCustomObject]@{}
      }
      $resultado = Executar-Tool -nome $fn -toolArgs $toolArgs
      $resultado = [string]$resultado
      $resumo = ($resultado -split "`n" | Select-Object -First 3) -join "`n"
      Write-Host "  -> $resumo" -ForegroundColor DarkGray
      $historico += @{ role = "tool"; tool_call_id = [string]$call.id; content = $resultado }
    }
  }

  # se saiu do loop por limite de turnos (nao por resposta final), avisa.
  if ($turno -ge 15) {
    Write-Host "[AVISO] Limite de 15 turnos de ferramentas atingido; interrompendo esta tarefa." -ForegroundColor Yellow
    Write-Host "        (continue pedindo, ou digite 'sair' para encerrar.)" -ForegroundColor Yellow
    $historico += @{ role = "assistant"; content = "[interrompido por limite de turnos de ferramentas]" }
  }
}

if ($modoInterativo) {
  Write-Host ""
  Write-Host "Ate logo!" -ForegroundColor Cyan
}
