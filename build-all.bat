@ECHO OFF
REM Build Everything

ECHO "Building everything..."

PUSHD engine
CALL build.bat
POPD
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

PUSHD sandbox
CALL build.bat
POPD
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies built successfully"