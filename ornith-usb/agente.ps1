# agente.ps1 — agente de terminal que faz tarefas sozinho (ler/editar/compilar/rodar)
# Loop de tool calling direto na API do Ornith. Sem timeout agressivo (suporta CPU lenta).
# Portatil: nada em %USERPROFILE%. Tudo no pendrive.
# Uso: agente.bat  (depois rode: "corrija o soma.c", "crie um programa que...", etc.)
# Requer: run.bat rodando (servidor na porta 8137)

param(
  # tarefa inicial (opcional). Se passar, roda uma tarefa so e sai (modo nao-interativo).
  # Se nao passar, entra em modo chat interativo.
  [Parameter(ValueFromRemainingArguments=$true)]
  [string[]]$TarefaArgs
)

$ErrorActionPreference = "Continue"
# NAO muda de diretorio: o agente opera na pasta atual do usuario (onde estao os .c dele)
# $PSScriptRoot so e usado pra achar arquivos do proprio agente, se necessario.

$Porta = 8137
$Url = "http://127.0.0.1:$Porta/v1/chat/completions"

# se veio tarefa por argumento, junta numa string so
$tarefaInicial = ""
if ($TarefaArgs -and $TarefaArgs.Count -gt 0) {
  $tarefaInicial = ($TarefaArgs -join " ").Trim()
}
$modoInterativo = [string]::IsNullOrEmpty($tarefaInicial)

# --- sincroniza CurrentDirectory do .NET com a pasta atual do PowerShell ---
# BUG CRITICO que isto resolve: Set-Location (cd) muda o PSDrive mas NAO atualiza
# Environment.CurrentDirectory do .NET. Entao [System.IO.Path]::GetFullPath("soma.c")
# resolvia pro diretorio onde o PowerShell foi iniciado, nao pra onde o usuario fez cd.
# Resultado: as tools nao achavam arquivos relativos. SetCurrentDirectory corrige isso.
[System.IO.Directory]::SetCurrentDirectory((Get-Location).Path)

# --- resolve caminho relativo contra a pasta atual (de forma robusta) ---
function Resolver-Caminho([string]$caminho) {
  if ([string]::IsNullOrWhiteSpace($caminho)) { return $caminho }
  # se ja e absoluto, devolve normalizado
  if ([System.IO.Path]::IsPathRooted($caminho)) {
    return [System.IO.Path]::GetFullPath($caminho)
  }
  # relativo: combina com CurrentDirectory (que sincronizamos acima)
  return [System.IO.Path]::GetFullPath([System.IO.Path]::Combine([System.IO.Directory]::GetCurrentDirectory(), $caminho))
}

# checa servidor
try {
  Invoke-RestMethod -Uri "http://127.0.0.1:$Porta/health" -TimeoutSec 5 | Out-Null
} catch {
  Write-Host "[ERRO] Servidor nao esta rodando na porta $Porta." -ForegroundColor Red
  Write-Host "       Abra run.bat primeiro (deixa a janela aberta)." -ForegroundColor Yellow
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
      description = "Roda um comando no shell (bash/cmd). Use para compilar (gcc) e executar programas."
      parameters = @{
        type = "object"
        properties = @{ comando = @{ type = "string"; description = "comando a executar" } }
        required = @("comando")
      }
    }
  }
)

# --- executa uma ferramenta (sempre retorna string, nunca nulo) ---
# NOTA: $args e variavel automatica reservada do PowerShell — NUNCA usar como
# parametro de funcao (e ignorada/sombreada). Usamos $toolArgs no lugar.
function Executar-Tool($nome, $toolArgs) {
  $resultado = ""
  try {
    switch ($nome) {
      "ler_arquivo" {
        $caminho = Resolver-Caminho $toolArgs.caminho
        if (-not (Test-Path -LiteralPath $caminho)) { return "[ERRO] arquivo nao encontrado: $caminho" }
        $conteudo = Get-Content -LiteralPath $caminho -Raw -Encoding UTF8
        if ($null -eq $conteudo) { return "[OK] arquivo vazio: $caminho" }
        # cast [string] evita deadlock do ConvertTo-Json no PS 5.1
        return [string]$conteudo
      }
      "escrever_arquivo" {
        $caminho = Resolver-Caminho $toolArgs.caminho
        $dir = [System.IO.Path]::GetDirectoryName($caminho)
        if (-not (Test-Path $dir)) { New-Item -ItemType Directory -Force -Path $dir | Out-Null }
        $utf8 = New-Object System.Text.UTF8Encoding($false)
        [System.IO.File]::WriteAllText($caminho, $toolArgs.conteudo, $utf8)
        return "[OK] arquivo salvo: $caminho"
      }
      "listar_arquivos" {
        $pasta = if ($toolArgs.pasta) { Resolver-Caminho $toolArgs.pasta } else { (Get-Location).Path }
        if (-not (Test-Path -LiteralPath $pasta)) { return "[ERRO] pasta nao encontrada: $pasta" }
        $itens = Get-ChildItem -LiteralPath $pasta | Select-Object @{n='nome';e={$_.Name}}, @{n='tipo';e={if($_.PSIsContainer){'pasta'}else{'arquivo'}}}, @{n='tamanho';e={$_.Length}}
        $s = ($itens | Format-Table -AutoSize | Out-String)
        if ([string]::IsNullOrWhiteSpace($s)) { return "[OK] pasta vazia: $pasta" }
        return $s
      }
      "rodar_comando" {
        $cmd = $toolArgs.comando
        Write-Host "  [exec] $cmd" -ForegroundColor DarkGray
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = "cmd.exe"
        $psi.Arguments = "/c " + $cmd
        $psi.UseShellExecute = $false
        $psi.RedirectStandardOutput = $true
        $psi.RedirectStandardError = $true
        $psi.CreateNoWindow = $true
        $psi.WorkingDirectory = (Get-Location).Path
        $p = [System.Diagnostics.Process]::Start($psi)
        $stdout = $p.StandardOutput.ReadToEnd()
        $stderr = $p.StandardError.ReadToEnd()
        $p.WaitForExit(30000) | Out-Null
        $saida = $stdout
        if ($stderr) { $saida += "`n" + $stderr }
        $saida += "`n[codigo de saida: $($p.ExitCode)]"
        if ([string]::IsNullOrWhiteSpace($stdout) -and [string]::IsNullOrWhiteSpace($stderr)) {
          $saida = "[OK] comando executado (sem saida). [codigo de saida: $($p.ExitCode)]"
        }
        return $saida
      }
      default { return "[ERRO] ferramenta desconhecida: $nome" }
    }
  } catch {
    return "[ERRO] $($_.Exception.Message)"
  }
  return $resultado
}

$system = @"
Voce e o Ornith, um agente de programacao em C que opera no terminal Windows (cmd).
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
  Write-Host "  Ornith Agente  (porta $Porta)" -ForegroundColor Cyan
  Write-Host "  Digite 'sair' para encerrar." -ForegroundColor Cyan
  Write-Host "  Ex: 'corrija o soma.c' / 'crie um programa que ordene 5 numeros'" -ForegroundColor Cyan
  Write-Host "========================================" -ForegroundColor Cyan
  Write-Host "  Pasta de trabalho: $((Get-Location).Path)" -ForegroundColor DarkGray
  Write-Host "  (o agente le/edita/compila arquivos nesta pasta)" -ForegroundColor DarkGray
  Write-Host ""
}

# fila de mensagens: comeca com a tarefa inicial (se houver), depois le do terminal
$fila = @()
if (-not [string]::IsNullOrEmpty($tarefaInicial)) { $fila += $tarefaInicial }

while ($true) {
  if ($fila.Count -gt 0) {
    $msg = $fila[0]
    $fila = $fila[1..($fila.Count)]
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
    $body = @{
      model = "ornith"
      messages = @(@{ role = "system"; content = $system }) + $historico
      tools = $tools
      tool_choice = "auto"
      stream = $false
      temperature = 0.3
      top_p = 0.95
      max_tokens = 4096
    } | ConvertTo-Json -Depth 20 -Compress

    if ($modoInterativo) { Write-Host "pensando..." -ForegroundColor DarkGray }
    try {
      $resp = Invoke-RestMethod -Uri $Url -Method Post -ContentType "application/json; charset=utf-8" -Body $body -TimeoutSec 600
    } catch {
      Write-Host "[ERRO] Falha ao chamar o modelo: $($_.Exception.Message)" -ForegroundColor Red
      break
    }

    $tc = $resp.choices[0].message.tool_calls
    if (-not $tc) {
      # modelo terminou - mostra resposta final
      $resposta = $resp.choices[0].message.content
      $historico += @{ role = "assistant"; content = $resposta }
      Write-Host ""
      Write-Host $resposta -ForegroundColor White
      Write-Host ""
      break
    }

    # adiciona mensagem do assistant com tool_calls
    # IMPORTANTE: converte a message (PSCustomObject da API) pra hashtable com
    # valores [string] explicitos. O ConvertTo-Json do PS 5.1 tem um deadlock
    # conhecido ao serializar objetos desserializados do Invoke-RestMethod
    # (PSCustomObject) e strings do Get-Content que carregam metadados do PSDrive.
    # Cast explicito pra [string] em TODOS os valores evita o deadlock.
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

    # executa cada tool chamada
    foreach ($call in $tc) {
      $fn = [string]$call.function.name
      Write-Host "  [tool] $fn" -ForegroundColor Yellow
      try {
        $toolArgs = $call.function.arguments | ConvertFrom-Json
      } catch {
        $toolArgs = [PSCustomObject]@{}
      }
      $resultado = Executar-Tool -nome $fn -toolArgs $toolArgs
      # CAST CRITICO: resultado deve ser [string] senao o ConvertTo-Json trava
      $resultado = [string]$resultado
      # mostra resultado curto (primeiras linhas)
      $resumo = ($resultado -split "`n" | Select-Object -First 3) -join "`n"
      Write-Host "  -> $resumo" -ForegroundColor DarkGray
      $historico += @{ role = "tool"; tool_call_id = [string]$call.id; content = $resultado }
    }
  }
}

if ($modoInterativo) {
  Write-Host ""
  Write-Host "Ate logo!" -ForegroundColor Cyan
}
