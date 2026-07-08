@echo off
setlocal
chcp 65001 >nul
title Configurar MiniCPM5-1B (contexto + threads + quant)
cd /d "%~dp0"

REM --- config.bat: ajusta contexto, threads e versao do modelo ---
REM Salva em config\ (contexto.txt, threads.txt, quant.txt). O run-minicpm.bat le.
REM Auto-detecta RAM e CPU pra sugerir valores seguros.

if not exist "config" mkdir "config"

REM --- detecta RAM total (MB) via PowerShell ---
REM wmic foi REMOVIDO no Windows 11 24H2+ e set/a estoura int32 com RAM em bytes.
REM Get-CimInstance funciona em todo Windows 8+ e a divisao em .NET nao estoura.
set "RAM_MB=16384"
for /f "delims=" %%R in ('powershell -NoProfile -Command "[math]::Floor((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory/1MB)" 2^>nul') do set "RAM_MB=%%R"
if "%RAM_MB%"=="" set "RAM_MB=16384"

REM --- detecta nucleos logicos ---
set "NPROC=%NUMBER_OF_PROCESSORS%"
if "%NPROC%"=="" set "NPROC=8"

REM --- le configs atuais (defaults seguros pra 16GB) ---
set "CTX=8192"
if exist "config\contexto.txt" set /p CTX=<"config\contexto.txt"
set "THR=8"
if exist "config\threads.txt" set /p THR=<"config\threads.txt"
set "QUANT=Q4_K_M"
if exist "config\quant.txt" set /p QUANT=<"config\quant.txt"

echo ============================================
echo   Configurar MiniCPM5-1B
echo ============================================
echo.
echo   PC detectado:
echo     RAM total:   %RAM_MB% MB
echo     Nucleos:      %NPROC%  (logicos)
echo.
echo   Config atual:
echo     Contexto:     %CTX% tokens
echo     Threads:      %THR%
echo     Quantizacao:   %QUANT%   (Q4_K_M=rapido, Q8_0=mais qualidade)
echo.
echo   --- Sugestoes por RAM ---
if %RAM_MB% LSS 12000 (
echo     12GB ou menos:  contexto 4096, threads 4, Q4_K_M  (PC fraco)
) else if %RAM_MB% LSS 20000 (
echo     16GB:           contexto 8192, threads 8, Q4_K_M  (faculdade, recomendado)
) else (
echo     24GB+:          contexto 16384, threads 8, Q4_K_M ou Q8_0
)
echo.
echo   Threads: use o numero de CORES FISICOS (metade dos logicos).
echo   16 threads = LENTO (thrashing). 8 = ideal na maioria.
echo.

set /p NOVO_CTX="Contexto em tokens (Enter=%CTX%): "
if not "%NOVO_CTX%"=="" (
  set /a VAL=%NOVO_CTX% 2>nul
  if not "%VAL%"=="0" echo %VAL%> "config\contexto.txt"
)

set /p NOVO_THR="Threads (Enter=%THR%): "
if not "%NOVO_THR%"=="" (
  set /a VAL=%NOVO_THR% 2>nul
  if not "%VAL%"=="0" echo %VAL%> "config\threads.txt"
)

set /p NOVO_Q="Quant: Q4_K_M ou Q8_0 (Enter=%QUANT%): "
if not "%NOVO_Q%"=="" echo %NOVO_Q%> "config\quant.txt"

echo.
echo [OK] Config salvo. Reinicie o servidor (feche e abra run-minicpm.bat).
echo.
endlocal
pause
