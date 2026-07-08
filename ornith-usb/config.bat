@echo off
chcp 65001 >nul
title Configurar janela de contexto
cd /d "%~dp0"

REM --- config.bat: ajusta o tamanho da janela de contexto (em tokens) ---
REM Salva em config\contexto.txt — o run.bat le esse valor ao subir.
REM Menor contexto = menos RAM e mais rapido. Maior = aceita programas maiores.
REM Auto-detecta RAM pra sugerir um valor seguro pro PC atual.

if not exist "config" mkdir "config"
set "CFG=config\contexto.txt"

REM --- detecta RAM total (MB) via PowerShell ---
REM wmic foi REMOVIDO no Windows 11 24H2+ e set/a estoura int32 com RAM em bytes.
REM Get-CimInstance funciona em todo Windows 8+ e a divisao em .NET nao estoura.
set "RAM_MB=16384"
for /f "delims=" %%R in ('powershell -NoProfile -Command "[math]::Floor((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory/1MB)" 2^>nul') do set "RAM_MB=%%R"
if "%RAM_MB%"=="" set "RAM_MB=16384"

REM le valor atual (default 32768)
set "ATUAL=32768"
if exist "%CFG%" set /p ATUAL=<"%CFG%"

echo ============================================
echo   Janela de contexto (tokens)
echo ============================================
echo.
echo   PC detectado:  %RAM_MB% MB de RAM
echo   Atual:         %ATUAL% tokens
echo.
echo   Valores sugeridos:
echo     4096   - minimo, mais rapido, so programas pequenos
echo     8192   - bom pra maioria dos exercicios
echo     16384  - programas medios
echo     32768  - padrao (recomendado, programas de prova grandes)
echo     65536  - maximo (so se tiver RAM sobrando)
echo.
echo   Quanto maior, mais RAM o modelo usa (~128 bytes por token).
if %RAM_MB% LSS 12000 (
  echo   [PC FRACO] %RAM_MB% MB detectado: use 4096-8192. 32768+ pode estourar a RAM.
) else if %RAM_MB% LSS 20000 (
  echo   [16GB] %RAM_MB% MB: 32768 e seguro (~9GB total). 65536 so com RAM sobrando.
) else (
  echo   [24GB+] %RAM_MB% MB: 65536 e confortavel. Use 32768 se quiser sobrar RAM.
)
echo.
set /p NOVO="Novo valor (Enter para manter %ATUAL%): "

if "%NOVO%"=="" (
  echo Mantendo %ATUAL%.
  goto fim
)

REM valida se e numero decimal puro (sem leading zero, sem hex/octal).
REM set /a interpreta 010000 como octal (vira 4096) e 0x1000 como hex (vira 4096),
REM silenciosamente. findstr ^[1-9][0-9]*$ so aceita decimal sem ambiguidade.
echo %NOVO%| findstr /r /c:"^[1-9][0-9]*$" >nul
if errorlevel 1 (
  if not "%NOVO%"=="0" (
    echo [ERRO] Valor invalido - use so digitos decimais (sem zero a esquerda, sem 0x).
    goto fim
  )
)
set /a VAL=%NOVO% 2>nul
if "%VAL%"=="0" if not "%NOVO%"=="0" (
  echo [ERRO] Valor invalido - use so numeros.
  goto fim
)
if %VAL% LSS 1024 (
  echo [ERRO] Muito baixo - minimo 1024.
  goto fim
)
if %VAL% GTR 65536 (
  echo [ERRO] Muito alto - maximo 65536.
  goto fim
)
set "NOVO=%VAL%"

echo %NOVO%> "%CFG%"
echo.
echo [OK] Contexto definido para %NOVO% tokens.
echo     Reinicie o servidor (feche e abra o run.bat) para aplicar.

:fim
echo.
pause
