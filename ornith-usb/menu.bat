@echo off
chcp 65001 >nul
title Ornith-USB - Menu
cd /d "%~dp0"

:menu
cls
echo ============================================
echo            ORNITH-USB  -  MENU
echo      LLM de programacao em C (portatil)
echo ============================================
echo.
echo   1.  Subir servidor        (run.bat)
echo   2.  Criar programa C      (criar.bat)
echo   3.  Revisar arquivo C     (revisar.bat)
echo   4.  Chat interativo       (chat.bat)
echo   5.  Agente (corrige sozinho)  (agente.bat)
echo   6.  Configurar contexto   (config.bat)
echo   7.  Ver status do servidor
echo   8.  Parar servidor
echo   9.  Limpar rastros/cache  (limpar.bat)
echo   0.  Sair
echo.
set "OP="
set /p OP="Escolha: "

if "%OP%"=="1" goto run
if "%OP%"=="2" goto criar
if "%OP%"=="3" goto revisar
if "%OP%"=="4" goto chat
if "%OP%"=="5" goto agente
if "%OP%"=="6" goto config
if "%OP%"=="7" goto status
if "%OP%"=="8" goto parar
if "%OP%"=="9" goto limpar
if "%OP%"=="0" exit /b 0
goto menu

:run
call run.bat
goto menu

:criar
call criar.bat
goto menu

:revisar
call revisar.bat
goto menu

:chat
call chat.bat
goto menu

:agente
call agente.bat
goto menu

:config
call config.bat
goto menu

:limpar
call limpar.bat
goto menu

:status
echo.
echo Verificando servidor na porta 8137...
powershell -NoProfile -Command "try { $h = Invoke-RestMethod -Uri 'http://127.0.0.1:8137/health' -TimeoutSec 3; Write-Host '[OK] Servidor rodando:' $h.status -ForegroundColor Green } catch { Write-Host '[OFF] Servidor nao esta rodando. Suba com a opcao 1.' -ForegroundColor Red }"
echo.
pause
goto menu

:parar
echo.
taskkill /f /im llama-server.exe >nul 2>&1
if %errorlevel%==0 (
  echo [OK] Servidor parado.
) else (
  echo [INFO] Nao havia servidor rodando.
)
echo.
pause
goto menu
