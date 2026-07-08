@echo off
chcp 65001 >nul
title Ornith Chat
cd /d "%~dp0"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0chat.ps1"
if errorlevel 1 pause
