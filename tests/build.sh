#!/bin/bash
echo echo on

mkdir -p ../bin

sourceFiles=$(find . -type f -name "*.c")

assembly="tests"
compilerFlags="-g -fdeclspec -fPIC"
includeFlags="-Isrc -I../engine/src/"
linkerFlags="-L../bin/ -lengine -Wl,-rpath,."
defines="-D_DEBUG -DKIMPORT"

echo "Building $assembly"
clang $sourceFiles $compilerFlags -o ../bin/$assembly $defines $includeFlags $linkerFlags
