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

REM --- threads: detecta cores logicos e ajusta pra fisicos (otimo em CPU) ---
REM NUMBER_OF_PROCESSORS e sempre definido no Windows (cores logicos).
REM Em CPUs com muitos threads (16+), usar TODOS os logicos causa thrashing
REM (cai pra ~1 tok/s). Cores FISICOS (metade dos logicos) e o ponto otimo.
REM Se vier vazio/invalido, cai pra 4.
set "NPROC=%NUMBER_OF_PROCESSORS%"
if "%NPROC%"=="" set "NPROC=4"
set /a "NPROC_TEST=%NPROC%+0" 2>nul
if not defined NPROC_TEST set "NPROC=4"
if "%NPROC_TEST%"=="0" set "NPROC=4"
if "%NPROC_TEST%"=="" set "NPROC=4"
set "THREADS=%NPROC%"
REM mais de 12 logicos: usa metade (cores fisicos). Evita thrashing em Ryzen/i7.
if %NPROC_TEST% GTR 12 set /a "THREADS=%NPROC%/2"
if %NPROC_TEST% LSS 2 set "THREADS=4"

REM --- porta especifica p/ nao colidir com servicos da faculdade ---
set "PORT=8137"

REM --- tamanho da janela de contexto (tokens) ---
REM Le de config\contexto.txt (definido por config.bat). Default: 32768 (igual ao config.bat e ao LEIA-ME).
set "CTX=32768"
if exist "config\contexto.txt" set /p CTX=<"config\contexto.txt"

REM --- protecao contra OOM: se o PC tem pouca RAM, reduz o contexto automaticamente ---
REM O Ornith-9B usa ~6GB fixos + ~128 bytes por token de contexto. Em PC com
REM pouca RAM livre, 32768 tokens podem estourar a RAM e o Windows mata o
REM servidor sem avisar (crash silencioso no meio da geracao). Se o usuario nao
REM definiu contexto explicitamente (arquivo ausente), detecta RAM e ajusta.
set "RAM_MB=16384"
for /f "delims=" %%R in ('powershell -NoProfile -Command "[math]::Floor((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory/1MB)" 2^>nul') do set "RAM_MB=%%R"
if "%RAM_MB%"=="" set "RAM_MB=16384"
if not exist "config\contexto.txt" (
  if %RAM_MB% LSS 12000 (
    set "CTX=4096"
    echo [AVISO] PC com %RAM_MB% MB de RAM: usando contexto 4096 (seguro).
    echo         Para mais contexto, feche outros programas e rode config.bat.
  ) else if %RAM_MB% LSS 20000 (
    set "CTX=16384"
    echo [INFO] PC com %RAM_MB% MB de RAM: usando contexto 16384.
  ) else (
    set "CTX=32768"
  )
)

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
