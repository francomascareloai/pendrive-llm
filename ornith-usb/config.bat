@echo off
chcp 65001 >nul
title Configurar janela de contexto
cd /d "%~dp0"

REM --- config.bat: ajusta o tamanho da janela de contexto (em tokens) ---
REM Salva em config\contexto.txt — o run.bat le esse valor ao subir.
REM Menor contexto = menos RAM e mais rapido. Maior = aceita programas maiores.

if not exist "config" mkdir "config"
set "CFG=config\contexto.txt"

REM le valor atual (default 32768)
set "ATUAL=32768"
if exist "%CFG%" set /p ATUAL=<"%CFG%"

echo ============================================
echo   Janela de contexto (tokens)
echo ============================================
echo.
echo   Atual: %ATUAL% tokens
echo.
echo   Valores sugeridos:
echo     4096   - minimo, mais rapido, so programas pequenos
echo     8192   - bom pra maioria dos exercicios
echo     16384  - programas medios
echo     32768  - padrao (recomendado, programas de prova grandes)
echo     65536  - maximo (so se tiver RAM sobrando)
echo.
echo   Quanto maior, mais RAM o modelo usa (~128 bytes por token).
echo   Num PC de 16GB, 32768 e seguro (~9GB total). 65536 so com RAM sobrando.
echo.
set /p NOVO="Novo valor (Enter para manter %ATUAL%): "

if "%NOVO%"=="" (
  echo Mantendo %ATUAL%.
  goto fim
)

REM valida se e numero: set /a converte, se virar 0 e o original nao era 0, e invalido
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
