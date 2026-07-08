@echo off
chcp 65001 >nul
title Revisar codigo C com Ornith
cd /d "%~dp0"

REM --- revisar.bat: arraste um .c em cima (ou rode: revisar.bat programa.c) ---
REM Requer run.bat rodando (servidor na porta 8137)
REM Opcoes:  revisar.bat programa.c       (pergunta se salva por cima)
REM          revisar.bat programa.c -y     (salva por cima direto, cria .bak)

set "SALVAR="
set "THINKING="
if /i "%~2"=="-y" set "SALVAR=-Salvar"
if /i "%~3"=="-y" set "SALVAR=-Salvar"
if /i "%~2"=="-thinking" set "THINKING=-Thinking"
if /i "%~3"=="-thinking" set "THINKING=-Thinking"

if "%~1"=="" (
  echo.
  echo Uso: revisar.bat ^<arquivo.c^> [-y] [-thinking]
  echo      -y = salva por cima do original (cria backup .bak)
  echo      -thinking = mostra o raciocinio do modelo
  echo      (ou arraste um arquivo .c para este .bat)
  echo.
  set /p ARQ="Caminho do arquivo .c: "
) else (
  set "ARQ=%~1"
)

REM so passa -Salvar/-Thinking se foram definidos (string vazia quebraria o switch param)
set "FLAGS=%SALVAR% %THINKING%"
if defined FLAGS (
  powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0revisar.ps1" -Arquivo "%ARQ%" %FLAGS%
) else (
  powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0revisar.ps1" -Arquivo "%ARQ%"
)

echo.
pause
