@echo off
chcp 65001 >nul
title Oh My Pi (omp) - Ornith-1.0-9B
cd /d "%~dp0"

REM --- omp: agente de terminal MINIMO (so edita/escreve/le arquivos) ---
REM Requer o llama-server rodando (suba antes com run.bat)
REM Config enxuta: sem LSP, sem debugger, sem skills/rules/extensions
REM = system prompt menor = mais rapido em CPU
REM PORTATIL: config e sessoes ficam no pendrive (nada em %USERPROFILE%).

set "OMP=omp\omp.exe"
if not exist "%OMP%" (
  echo [ERRO] omp nao encontrado: %OMP%
  pause
  exit /b 1
)

REM --- config e sessoes portateis no pendrive ---
set "OMP_CONFIG=%~dp0config\models.yml"
set "OMP_SESSIONS=%~dp0omp\sessions"
if not exist "%OMP_SESSIONS%" mkdir "%OMP_SESSIONS%"

REM --- model local-llama/ornith = llama-server em localhost:8137 ---
REM --config: carrega models.yml do pendrive (overlay, sem tocar em %USERPROFILE%)
REM --session-dir: sessoes ficam no pendrive
REM --no-lsp / --no-extensions / --no-skills / --no-rules: cortam peso do prompt
REM --thinking off: sem raciocinio longo (mais rapido, contexto menor)
REM --auto-approve: auto-aceita permissoes (modo agente)
"%OMP%" ^
  --model local-llama/ornith ^
  --config "%OMP_CONFIG%" ^
  --session-dir "%OMP_SESSIONS%" ^
  --allow-home ^
  --no-lsp ^
  --no-extensions ^
  --no-skills ^
  --no-rules ^
  --thinking off ^
  --auto-approve ^
  %*

if errorlevel 1 pause
