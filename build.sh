#! /bin/bash

set -e

CommonCompilerFlags="-Os -g -ggdb -std=c++11 -msse4.1 -ffast-math -Wno-braced-scalar-init -Wno-format -Wno-writable-strings -Wno-switch"
CommonDefines="-DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=0 -DHANDMADE_LINUX=1"
CommonLinkerFlags="-pthread -lX11 -ldl"

clang $CommonCompilerFlahs $CommonDefines linux_main.cpp $CommonLinkerFlags