REM Build script for engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Retrieve source files
Set sourceFiles=
FOR /R %%f in (*.c) do (
    SET sourceFiles=!sourceFiles! %%f
)

REM echo "Source Files:" %sourceFiles%

SET assembly=engine
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Isrc -I"%VULKAN_SDK%"/Include
SET linkerFlags=-luser32 -lvulkan-1 -L"%VULKAN_SDK%"/Lib
SET defines=-D_DEBUG -DKEXPORT -D_CRT_SECURE_NO_WARNINGS

ECHO "Building %assembly%..."
clang %sourceFiles% %compilerFlags% -o ../bin/%assembly%.dll %defines% %includeFlags% %linkerFlags%