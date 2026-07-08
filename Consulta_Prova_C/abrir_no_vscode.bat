@echo off
chcp 65001 >nul
set "ROOT=%~dp0"

where code >nul 2>nul
if errorlevel 1 (
  echo VS Code nao encontrado no PATH.
  echo Abrindo a pasta no Explorer.
  start "" "%ROOT%"
  pause
  exit /b 0
)

code "%ROOT%"
