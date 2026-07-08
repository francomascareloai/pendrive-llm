@echo off
setlocal
chcp 65001 >nul
title MiniCPM5-1B (portatil, CPU - auto-config)
cd /d "%~dp0"

echo ============================================
echo   MiniCPM5-1B  (1.1B, CPU-only, auto-config)
echo   SOTA 1B: coding agent + tool-use
echo ============================================
echo.

REM --- le configs salvas (config.bat), com defaults seguros pra 16GB ---
set "CTX=8192"
if exist "config\contexto.txt" set /p CTX=<"config\contexto.txt"

REM --- protecao contra OOM: se o PC tem pouca RAM, reduz o contexto automaticamente ---
REM O MiniCPM5-1B usa ~700MB fixos (modelo) + KV cache por token de contexto.
REM Em PC com pouca RAM livre, 8192 tokens podem estourar a RAM e o Windows mata
REM o servidor sem avisar (crash silencioso). Se o usuario nao definiu contexto
REM explicitamente (arquivo ausente), detecta RAM e ajusta.
set "RAM_MB=16384"
for /f "delims=" %%R in ('powershell -NoProfile -Command "[math]::Floor((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory/1MB)" 2^>nul') do set "RAM_MB=%%R"
if "%RAM_MB%"=="" set "RAM_MB=16384"
if not exist "config\contexto.txt" (
  if %RAM_MB% LSS 8000 (
    set "CTX=4096"
    echo [AVISO] PC com %RAM_MB% MB de RAM: usando contexto 4096 (seguro).
    echo         Para mais contexto, feche outros programas e rode config.bat.
  ) else if %RAM_MB% LSS 12000 (
    set "CTX=6144"
    echo [INFO] PC com %RAM_MB% MB de RAM: usando contexto 6144.
  ) else (
    set "CTX=8192"
  )
)
set "THREADS=%NUMBER_OF_PROCESSORS%"
if "%THREADS%"=="" set "THREADS=8"
if exist "config\threads.txt" set /p THREADS=<"config\threads.txt"
REM threads: se vier numero par alto, usa metade (cores fisicos = ideal).
REM 16 threads causa thrashing (cai pra 1 t/s). 8 e o ponto otimo na maioria.
set /a "THR_TEST=%THREADS%+0" 2>nul
if not defined THR_TEST set "THREADS=8"
if %THR_TEST% GTR 12 set /a "THREADS=%THREADS%/2"
if %THR_TEST% LSS 2 set "THREADS=4"

REM --- quant: Q4_K_M (rapido, default) ou Q8_0 (mais qualidade) ---
set "QUANT=Q4_K_M"
if exist "config\quant.txt" set /p QUANT=<"config\quant.txt"
set "MODEL=model\MiniCPM5-1B-%QUANT%.gguf"
if not exist "%MODEL%" (
  echo [AVISO] %MODEL% nao encontrado. Tentando Q4_K_M...
  set "MODEL=model\MiniCPM5-1B-Q4_K_M.gguf"
)
if not exist "%MODEL%" (
  echo [AVISO] Q4_K_M nao encontrado. Tentando Q8_0...
  set "MODEL=model\MiniCPM5-1B-Q8_0.gguf"
)
if not exist "%MODEL%" (
  echo [ERRO] Nenhum modelo encontrado em model\
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

REM --- porta 8138 (Ornith usa 8137; rodam juntos) ---
set "PORT=8138"

echo Modelo:    %MODEL%
echo Porta:     %PORT%
echo Contexto:  %CTX% tokens
echo Threads:   %THREADS%  (auto: %NUMBER_OF_PROCESSORS% logicos -> %THREADS% fisicos)
echo Endpoint:  http://localhost:%PORT%/v1/chat/completions
echo.
echo No crush:  provider minicpm5 / modelo minicpm5-1b
echo Feche esta janela para parar o servidor.
echo.

REM --- MiniCPM5: template ChatML nativo embutido no GGUF (--jinja) ---
REM No-Think via env var: o MiniCPM5 em modo Think gasta muitos tokens
REM "pensando" (lento em CPU). Forcar enable_thinking=false via template kwargs
REM faz o modelo responder direto (~3x mais rapido, sem desperdicar tokens).
REM   NOTA: --reasoning off tem o MESMO efeito mas CRASHA em geracoes longas
REM   neste build. O kwargs via env var e estavel.
REM -fa on: flash attention (ajuda em CPU com AVX2).
REM --timeout 600: libera slots presos apos 10min sem atividade.
set "LLAMA_ARG_CHAT_TEMPLATE_KWARGS={"enable_thinking":false}"
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
  --repeat-penalty 1.1 ^
  --timeout 600

pause
