REM Build script for sandbox
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Retrieve source files
Set sourceFiles=
FOR /R %%f in (*.c) do (
    SET sourceFiles=!sourceFiles! %%f
)

REM echo "Source Files:" %sourceFiles%

SET assembly=sandbox
SET compilerFlags=-g
SET includeFlags=-Isrc -I../engine/src
SET linkerFlags=-L../bin/ -lengine.lib
SET defines=-D_DEBUG -DKIMPORT

ECHO "Building %assembly%..."
clang %sourceFiles% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%
