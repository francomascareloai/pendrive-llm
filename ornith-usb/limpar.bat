@echo off
chcp 65001 >nul
title Limpar rastros e cache
cd /d "%~dp0"

echo ============================================
echo   Limpar rastros e cache
echo ============================================
echo.
echo Apaga:
echo   - sessoes antigas do omp/crush (no pendrive)
echo   - pasta .omp legacy em %USERPROFILE% (se existir de versao antiga)
echo.

REM --- sessoes do omp no pendrive ---
if exist "omp\sessions" (
  rmdir /s /q "omp\sessions" 2>nul
  mkdir "omp\sessions" 2>nul
  echo [OK] Sessoes do omp limpas.
) else (
  echo [OK] Sem sessoes do omp.
)

REM --- sessoes/db do crush no pendrive (mantem o crush.json de config) ---
if exist "crush\data\crush.db" (
  del /q "crush\data\crush.db" 2>nul
  echo [OK] Banco de sessoes do crush limpo.
)

REM --- pasta .omp legacy no PC (versao antiga do projeto) ---
if exist "%USERPROFILE%\.omp" (
  rmdir /s /q "%USERPROFILE%\.omp"
  if exist "%USERPROFILE%\.omp" (
    echo [AVISO] Nao foi possivel apagar %USERPROFILE%\.omp. Feche o omp e tente de novo.
  ) else (
    echo [OK] Pasta legacy %USERPROFILE%\.omp apagada.
  )
)

echo.
echo [OK] Limpeza concluida. O modelo e as configs NAO foram tocados.
echo.
pause
