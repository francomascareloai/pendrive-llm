@echo off
chcp 65001 >nul
title Ornith-1.0-9B - Q5_K_M (portatil)
cd /d "%~dp0"

echo ============================================
echo   Ornith-1.0-9B  -  Q5_K_M
echo   (9B dense, Qwen 3.5, modelo de codigo)
echo ============================================
echo.

REM --- acha o modelo ---
set "MODEL=model\ornith-1.0-9b-Q5_K_M.gguf"
if not exist "%MODEL%" (
  echo [ERRO] Modelo nao encontrado: %MODEL%
  echo Verifique se a pasta 'model' esta junto no pendrive.
  pause
  exit /b 1
)

REM --- acha o llama-server ---
set "SERVER=llama\llama-server.exe"
if not exist "%SERVER%" (
  echo [ERRO] llama-server nao encontrado: %SERVER%
  pause
  exit /b 1
)

REM --- threads: detecta o maximo disponivel no PC, com fallback seguro ---
REM NUMBER_OF_PROCESSORS e sempre definido no Windows (cores logicos).
REM Se vier vazio ou invalido, cai pra 4. Nunca divide cegamente: usa o que tem.
set "NPROC=%NUMBER_OF_PROCESSORS%"
if "%NPROC%"=="" set "NPROC=4"
set /a "NPROC_TEST=%NPROC%+0" 2>nul
if not defined NPROC_TEST set "NPROC=4"
if "%NPROC_TEST%"=="0" set "NPROC=4"
if "%NPROC_TEST%"=="" set "NPROC=4"
set "THREADS=%NPROC%"

REM --- porta especifica p/ nao colidir com servicos da faculdade ---
set "PORT=8137"

REM --- tamanho da janela de contexto (tokens) ---
REM Le de config\contexto.txt (definido por config.bat). Default: 32768 (igual ao config.bat e ao LEIA-ME).
set "CTX=32768"
if exist "config\contexto.txt" set /p CTX=<"config\contexto.txt"

echo Iniciando servidor (modo terminal / VS Code)...
echo Modelo:    %MODEL%
echo Porta:     %PORT%
echo Contexto:  %CTX% tokens  (mude com config.bat)
echo Endpoint:  http://localhost:%PORT%/v1/chat/completions
echo Threads:   %THREADS%  (detectado automaticamente)
echo.
echo No VS Code (extensao Continue/Cline/etc), configure:
echo   baseURL: http://localhost:%PORT%/v1
echo   model:   ornith
echo.
echo Feche esta janela para parar o servidor.
echo.

REM --- Ornith-1.0-9B (Qwen 3.5): template ChatML nativo embutido no GGUF ---
REM --jinja usa o template do proprio modelo (NAO forcar --chat-template).
REM --reasoning-format deepseek: o modelo gera<think>...</think> antes da resposta;
REM o servidor separa o raciocinio no campo reasoning_content e so devolve a resposta final.
"%SERVER%" ^
  -m "%MODEL%" ^
  --host 127.0.0.1 ^
  --port %PORT% ^
  -c %CTX% ^
  -t %THREADS% ^
  --threads-batch %THREADS% ^
  -b 512 ^
  -ub 512 ^
  -fa on ^
  -np 1 ^
  --jinja ^
  --reasoning-format deepseek

pause
