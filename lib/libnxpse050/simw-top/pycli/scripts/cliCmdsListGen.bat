@REM Copyright 2019 NXP
@REM
@REM This software is owned or controlled by NXP and may only be used
@REM strictly in accordance with the applicable license terms.  By expressly
@REM accepting such terms or by downloading, installing, activating and/or
@REM otherwise using the software, you are agreeing that you have read, and
@REM that you agree to comply with and are bound by, such license terms.  If
@REM you do not agree to be bound by the applicable license terms, then you
@REM may not retain, install, activate or otherwise use the software.
@REM
@ECHO OFF
SETLOCAL
    call ..\venv\Scripts\activate.bat
    python cli_commands_list_gen.py
    call ..\venv\Scripts\deactivate.bat
ENDLOCAL
