@echo off
chcp 65001 >nul
title Compilando c-answer...

echo ========================================
echo  Instalando dependencias...
echo ========================================
pip install -r requirements.txt
pip install pyinstaller

echo.
echo ========================================
echo  Compilando (single EXE, sem console)...
echo ========================================
pyinstaller --onefile --noconsole --name "c-answer" ^
  --hidden-import mss ^
  --hidden-import PIL._tkinter_finder ^
  --collect-data mss ^
  --clean --noconfirm ^
  main.py

echo.
echo ========================================
echo  OK!
echo ========================================
echo.
echo  Executavel: dist\c-answer.exe
echo  Tamanho:
for %%A in ("dist\c-answer.exe") do echo   %%~zA bytes (%%~zA/1048576:.2f MB)
echo.
echo  Coloque no pendrive:
echo    dist\c-answer.exe
echo    config.json
echo.
echo.
