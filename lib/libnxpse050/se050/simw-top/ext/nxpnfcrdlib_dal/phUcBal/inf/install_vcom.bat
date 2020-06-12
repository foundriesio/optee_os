REM @echo off

@REM

@REM This file has to be run as Adminstrator
cd /d %~dp0

@echo Installing VCOM Driver

call %SystemRoot%\system32\PnPutil.exe -i -a NxpNfcCockpit_VCOM.inf

pause
