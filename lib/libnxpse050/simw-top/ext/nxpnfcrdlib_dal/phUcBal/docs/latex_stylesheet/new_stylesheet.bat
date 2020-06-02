cd /d %~dp0

del *.bak
del orig.*.sty
del orig.*.tex
del orig.*.html
del orig.*.css

doxygen -w latex orig.header.tex orig.footer.tex orig.doxygen.sty
doxygen -w html  orig.headerFile.html orig.footerFile.html orig.styleSheetFile.css


