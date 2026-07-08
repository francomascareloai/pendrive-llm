@echo off
chcp 65001 >nul
title Ornith Agente - 1.0-9B

REM --- agente: loop de tool calling direto na API (ler/editar/compilar/rodar) ---
REM Requer o llama-server rodando (suba antes com run.bat)
REM Portatil: nada em %USERPROFILE%. Tudo no pendrive.
REM Suporta CPU lenta (sem timeout agressivo como o crush/omp).
REM IMPORTANTE: NAO faz cd pro pendrive. O agente opera na pasta ATUAL do usuario
REM (onde estao os .c dele). Rode este .bat de dentro da pasta do seu projeto,
REM ou chame com caminho completo: G:\ornith-usb\agente.bat

powershell -ExecutionPolicy Bypass -NoProfile -File "%~dp0agente.ps1" %*

if errorlevel 1 pause
