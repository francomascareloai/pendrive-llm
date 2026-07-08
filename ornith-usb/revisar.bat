@echo off
chcp 65001 >nul
title Revisar codigo C com Ornith
cd /d "%~dp0"

REM --- revisar.bat: arraste um .c em cima (ou rode: revisar.bat programa.c) ---
REM Requer run.bat rodando (servidor na porta 8137)
REM Opcoes:  revisar.bat programa.c       (pergunta se salva por cima)
REM          revisar.bat programa.c -y     (salva por cima direto, cria .bak)

set "SALVAR="
if /i "%~2"=="-y" set "SALVAR=-Salvar"

if "%~1"=="" (
  echo.
  echo Uso: revisar.bat ^<arquivo.c^> [-y]
  echo      -y = salva por cima do original (cria backup .bak)
  echo      (ou arraste um arquivo .c para este .bat)
  echo.
  set /p ARQ="Caminho do arquivo .c: "
) else (
  set "ARQ=%~1"
)

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0revisar.ps1" -Arquivo "%ARQ%" %SALVAR%

echo.
pause
