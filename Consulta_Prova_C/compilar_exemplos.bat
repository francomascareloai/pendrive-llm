@echo off
chcp 65001 >nul
set "ROOT=%~dp0"
set "OUT=%ROOT%bin"
mkdir "%OUT%" 2>nul

where gcc >nul 2>nul
if errorlevel 1 (
  echo gcc nao encontrado no Windows.
  echo Se estiver na faculdade, talvez o Code::Blocks/MinGW precise estar no PATH.
  pause
  exit /b 1
)

echo Compilando exemplos...
for %%F in ("%ROOT%exemplos\*.c" "%ROOT%gabaritos\*.c") do (
  echo %%~nxF
  gcc -Wall -Wextra "%%F" -o "%OUT%\%%~nF.exe"
)
echo.
echo Pronto. Executaveis em:
echo %OUT%
pause
