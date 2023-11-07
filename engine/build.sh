#!/bin/bash
echo echo on

mkdir -p ../bin

sourceFiles=$(find . -type f -name "*.c")

assembly="engine"
compilerFlags="-g -shared -fdeclspec -fPIC"
includeFlags="-Isrc -I$VULKAN_SDK/include"
linkerFlags="-lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -L$VULKAN_SDK/lib -L/usr/X11R6/lib"
defines="-D_DEBUG -DKEXPORT"

echo "Building $assembly"
clang $sourceFiles $compilerFlags -o ../bin/lib$assembly.so $defines $includeFlags $linkerFlags
