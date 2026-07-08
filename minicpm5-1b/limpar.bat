@echo off
chcp 65001 >nul
title Limpar rastros e cache
cd /d "%~dp0"

echo ============================================
echo   Limpar rastros e cache (MiniCPM5-1B)
echo ============================================
echo.
echo Apaga:
echo   - sessoes/db do crush no pendrive (mantem o crush.json de config)
echo.

REM --- sessoes/db do crush no pendrive (mantem o crush.json de config) ---
if exist "crush\data\crush.db" (
  del /q "crush\data\crush.db" 2>nul
  del /q "crush\data\crush.db-shm" 2>nul
  del /q "crush\data\crush.db-wal" 2>nul
  echo [OK] Banco de sessoes do crush limpo.
) else (
  echo [OK] Sem sessoes do crush.
)

echo.
echo [OK] Limpeza concluida. O modelo e as configs NAO foram tocados.
echo.
pause
