@echo off
chcp 65001 >nul
title Crush - Ornith 9B / MiniCPM5-1B (portatil)
cd /d "%~dp0"

REM --- crush: agente de terminal (Go binario unico, ~80MB) ---
REM Requer um llama-server rodando:
REM   - Ornith 9B  -> run.bat (porta 8137)        [default, SOTA em codigo]
REM   - MiniCPM5-1B-> run-minicpm.bat (porta 8138) [leve, rapido em CPU fraca]
REM Config enxuta: sem LSP, sem MCP, sem providers cloud.
REM Tools: view/ls/grep/edit/write/bash (auto-aprovadas via allowed_tools).

set "CRUSH=crush\crush.exe"
if not exist "%CRUSH%" (
  echo [ERRO] crush nao encontrado: %CRUSH%
  pause
  exit /b 1
)

REM --- tudo portatil: config e data ficam no pendrive ---
set "CRUSH_DATA=%~dp0crush\data"

REM --- config portatil: crush le crush.json de XDG_CONFIG_HOME\crush ---
REM Sem isso, crush usa config em C:\Users\... (nao existe no PC da faculdade)
REM e cai no provider Anthropic padrao (401 unauthorized).
set "XDG_CONFIG_HOME=%~dp0crush\config-xdg"
if not exist "%XDG_CONFIG_HOME%\crush\crush.json" (
  echo [ERRO] config nao encontrado: %XDG_CONFIG_HOME%\crush\crush.json
  pause
  exit /b 1
)

REM --- detecta se primeiro arg e "run" (modo nao-interativo) ---
REM modo interativo: usa --yolo (auto-aceita permissoes)
REM modo run: nao usa --yolo (incompativel com subcomando run)
set "FIRST=%~1"
set "YOLO_FLAG=--yolo"
if /i "%FIRST%"=="run" set "YOLO_FLAG="

REM --- roda o crush ---
REM --data-dir: sessoes/config ficam no pendrive (nao em ~)
REM --yolo: auto-aceita permissoes (modo interativo)
REM model default vem do crush.json (models.large = minicpm5/minicpm5-1b, porta 8138)
REM Para usar o Ornith 9B: crush.bat run -m local-llama/ornith "sua tarefa"
"%CRUSH%" ^
  --data-dir "%CRUSH_DATA%" ^
  %YOLO_FLAG% ^
  %*

if errorlevel 1 pause
