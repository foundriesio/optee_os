@echo off


if "%1"=="" goto :usage

FOR %%i IN (%1\*MALS_*.txt) DO (
	call prot_to_bin1.bat %%i
	)
FOR %%i IN (%1\*_proto*.text) DO (
	call rename_ext_to_txt.bat %%i
	)

:usage
    echo "usage : "
	echo %0 "<Folder containing the encrypted .txt files>"
