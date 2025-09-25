#!/bin/bash

# build+install the TPM using CMake with
# three build domains: Core Library; Platform; Simulator
# don't configure, which makes for fast partial builds, but
# means this script can't be used from a completely empty build environment,
# use cmake_parts.sh first.
# pass D for debug, or N/blank for not
CMD_CMAKE_DEBUG_REQUEST=$1

# ./scripts/nix/cmake_gen.sh   1 0 0 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_build.sh 1 0 0 $CMD_CMAKE_DEBUG_REQUEST
# ./scripts/nix/cmake_gen.sh   0 1 0 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_build.sh 0 1 0 $CMD_CMAKE_DEBUG_REQUEST
# ./scripts/nix/cmake_gen.sh   0 0 1 $CMD_CMAKE_DEBUG_REQUEST
./scripts/nix/cmake_build.sh 0 0 1 $CMD_CMAKE_DEBUG_REQUEST
# part 4 = tests - not implemented yet
#./scripts/nix/cmake_build.sh 0 0 0 1 $CMD_CMAKE_DEBUG_REQUEST
