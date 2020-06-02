
SETLOCAL

SET CURDIR=%CD%
cd /d %~dp0

REM SET CALL_PDFLATEX=call pdflatex refman -quiet -interaction=nonstopmode -interaction=batchmode
SET CALL_PDFLATEX=call pdflatex refman -quiet -interaction=nonstopmode -interaction=batchmode

:GEN_LATEX
    @echo Generating Latex = PASS 1
    %CALL_PDFLATEX%

    REM GOTO :GEN_LATEX_DONE

    call makeindex refman.tex

    @echo Generating Latex = PASS 2
    %CALL_PDFLATEX% > NUL
    @echo Generating Latex = PASS 3
    %CALL_PDFLATEX% > NUL
    @echo Generating Latex = PASS 4
    %CALL_PDFLATEX% > NUL
    @echo Generating Latex = PASS 5
    %CALL_PDFLATEX% > NUL
    REM @echo Generating Latex = PASS 6
    REM %CALL_PDFLATEX% > NUL
    REM @echo Generating Latex = PASS 7
    REM %CALL_PDFLATEX% > NUL

:GEN_LATEX_DONE

    cd /d %CURDIR%

ENDLOCAL
