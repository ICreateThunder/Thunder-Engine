#!/bin/sh
echo echo on

mkdir -p ../bin

sourceFiles=$(find . -type f -name "*.c")

assembly="sandbox"
compilerFlags="-g -fdeclspec -fPIC"
includeFlags="=Isrc -I../engine/src/"
linkerFlags="-../bin/ -lengine -Wl,-rpath,."
defines="-D_DEBUG -DKEXPORT"

echo "Building $assembly
clan $sourceFiles $compilerFlags -o ../bin/$assembly $defines $includeFlags $linkerFlags