@echo off
chcp 65001 >nul
title Criar programa C com Ornith
cd /d "%~dp0"

REM --- criar.bat: descreva o programa e ele gera + salva num .c ---
REM Requer run.bat rodando (servidor na porta 8137)
REM Uso:  criar.bat                          (pergunta descricao e nome)
REM       criar.bat programa.c               (pergunta so a descricao)
REM       criar.bat programa.c "lista ligada com 3 nos"

set "SAIDA=%~1"
set "DESC=%~2"
set "THINKING="

REM --- flag -thinking: mostra o raciocinio do modelo ---
:parse_args
if /i "%~3"=="-thinking" set "THINKING=-Thinking"
if /i "%~4"=="-thinking" set "THINKING=-Thinking"
if /i "%~5"=="-thinking" set "THINKING=-Thinking"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0criar.ps1" -Saida "%SAIDA%" -Desc "%DESC%" %THINKING%

echo.
pause
