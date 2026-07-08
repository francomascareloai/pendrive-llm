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
powershell -NoProfile -Command "try { $r = Invoke-WebRequest -Uri 'http://127.0.0.1:8137/health' -TimeoutSec 3 -UseBasicParsing; if ($r.StatusCode -eq 200) { Write-Host '[OK] Servidor rodando.' -ForegroundColor Green } } catch { $sc = $_.Exception.Response.StatusCode.value__; if ($sc -eq 503) { Write-Host '[CARREGANDO] Servidor subindo o modelo, aguarde 30-60s...' -ForegroundColor Yellow } else { Write-Host '[OFF] Servidor nao esta rodando. Suba com a opcao 1.' -ForegroundColor Red } }"
echo.
pause
goto menu

:parar
echo.
REM Para APENAS o servidor da porta 8137 (Ornith), preservando o MiniCPM5 na 8138.
REM taskkill /im llama-server.exe mataria TODOS os servidores por nome de imagem.
powershell -NoProfile -Command "$c = Get-NetTCPConnection -LocalPort 8137 -State Listen -ErrorAction SilentlyContinue; if ($c) { Stop-Process -Id $c.OwningProcess -Force; Write-Host '[OK] Servidor Ornith (porta 8137) parado.' -ForegroundColor Green } else { Write-Host '[INFO] Nenhum servidor na porta 8137.' -ForegroundColor Yellow }"
echo.
pause
goto menu
