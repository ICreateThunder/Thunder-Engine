@ECHO OFF
REM Build Everything

ECHO "Building everything..."

@REM PUSHD engine
@REM CALL build.bat
@REM POPD
@REM if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

@REM PUSHD sandbox
@REM CALL build.bat
@REM POPD
@REM if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Engine
make -f "Makefile.engine.windows.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Sandbox
make -f "Makefile.sandbox.windows.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Sandbox
make -f "Makefile.tests.windows.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies built successfully"