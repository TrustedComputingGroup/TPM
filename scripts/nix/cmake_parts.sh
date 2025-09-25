#!/bin/bash

# configure, then build+install the TPM using CMake with
# three build domains: Core Library; Platform; Simulator
# pass D for debug, or N/blank for not
CMD_CMAKE_DEBUG_REQUEST=$1

./scripts/nix/cmake_gen.sh 1 0 0 0 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_build.sh 1 0 0 0 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_gen.sh 0 1 0 0 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_build.sh 0 1 0 0 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_gen.sh 0 0 1 0 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_build.sh 0 0 1 0 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_gen.sh 0 0 0 1 $CMD_CMAKE_DEBUG_REQUEST
# notimpl ./scripts/nix/cmake_build.sh 0 0 0 1 $CMD_CMAKE_DEBUG_REQUEST
