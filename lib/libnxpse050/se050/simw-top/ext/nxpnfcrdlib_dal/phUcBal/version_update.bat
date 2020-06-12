cd /d %~dp0

call ..\phUcBal_Docs\version_update.py . src\NNC_uC_VCOM_Ver.h
IF EXIST ..\..\PN7462AU_NNC\PN7462AU_libNNC\inc\PN7462AU_libNNC_Ver.h ^
call ..\phUcBal_Docs\version_update.py . ..\..\PN7462AU_NNC\PN7462AU_libNNC\inc\PN7462AU_libNNC_Ver.h
call ..\phUcBal_Docs\version_update.py . docs\_version.sh
call ..\phUcBal_Docs\version_update.py . ..\phUcBal_Docs\_version.doxyfile

