@echo off
chcp 65001 >nul
title MiniCPM5-1B Agente

REM --- agente: loop de tool calling direto na API (ler/editar/compilar/rodar) ---
REM Requer o llama-server rodando (suba antes com run-minicpm.bat)
REM Portatil: nada em %USERPROFILE%. Tudo no pendrive.
REM Suporta CPU lenta (sem timeout agressivo como o crush).
REM IMPORTANTE: NAO faz cd pro pendrive. O agente opera na pasta ATUAL do usuario
REM (onde estao os .c dele). Rode este .bat de dentro da pasta do seu projeto,
REM ou chame com caminho completo: G:\minicpm5-1b\agente.bat

powershell -ExecutionPolicy Bypass -NoProfile -File "%~dp0agente.ps1" %*

if errorlevel 1 pause
