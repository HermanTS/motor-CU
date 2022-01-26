#! /bin/bash

git submodule init
mkdir -p ../.build-motor-control-unit/
cmake.exe -S . -B ../.build-motor-control-unit/ -D TOOLCHAIN_PREFIX=/opt/tools/cortexm/ -G Eclipse CDT4 - Unix Makefiles
