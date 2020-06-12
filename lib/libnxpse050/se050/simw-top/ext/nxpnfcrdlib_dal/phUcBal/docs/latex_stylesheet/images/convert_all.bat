@echo off

@REM: Use ImageMagic to convert PNGs into PDFs and EPS

SETLOCAL

cd /d %~dp0..\..\images

SET PATH=C:\opt\ImageMagick-6.7.7-7-Q16-windows\ImageMagick-6.7.7-7;%PATH%

@REM: Don't know why, but front page is not able to include the eps directly.
convert.exe nxplogo.png nxplogo.pdf

for %%i in (*.png) do (
    @echo %%i
    convert.exe %%i %%~ni.eps
)

ENDLOCAL
cd /d %~dp0
