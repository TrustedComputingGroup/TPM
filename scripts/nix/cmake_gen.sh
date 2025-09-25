#!/bin/bash

# generate cmake environment for building CoreLib, Platform, Simulator as
# separate components.
# Usage: cmake_gen.sh 0 0 0 [D]
# Set the desired component to 1.
# since configure requires the previous builds to complete
# and be installed, only one item should be a 1 on any call.
# This is intended as an interactive/dev script, not for
# use in a CI pipeline - it's use of $PWD is likely unsafe for
# such an environment

set -e
#set -x
DO_TPMLIB=${1:-0}
DO_PLATLIB=${2:-0}
DO_SIMLIB=${3:-0}
DO_TESTS=${4:-0}
DEBUG_CMAKE=${5:-0}

if [ "$DEBUG_CMAKE" == "D" ]; then
    set -x
fi

TOTAL=$((DO_TPMLIB + DO_PLATLIB + DO_SIMLIB + DO_TESTS))
if [ "$TOTAL" -ne 1 ]; then
    echo "only one option may be set"
    exit 1
fi

if [ ! -f "$PWD/scripts/nix/cmake_env.sh" ]; then
    echo "expect to run script from root of tpm repo"
    exit 1
fi

source "$PWD/scripts/nix/cmake_env.sh" "$PWD/build/parts" "$DEBUG_CMAKE"
if [ -z "$BUILD_ROOT" ]; then
    echo "cmake_env did not set a sensible root"
    exit 1
fi

CMAKE_GEN_OPTIONS="$CMAKE_GEN_OPTIONS_BASE --install-prefix $INSTALL_ROOT/parts"

# full reset of the build folder if starting with the library
# also, the library configuration requires the LibOnly option
if [ "$DO_TPMLIB" -eq 1 ]; then
    if [ -d "$BUILD_ROOT" ]; then
        rm -rf "$BUILD_ROOT"
        mkdir -p "$LOG_ROOT"
    fi
    SOURCE_DIR="TPMCmd/."
    CMAKE_BUILD_SUBDIR="tpmlib"
    CMAKE_GEN_OPTIONS="$CMAKE_GEN_OPTIONS -D Tpm_BuildOption_LibOnly=1"
fi

if [ "$DO_PLATLIB" -eq 1 ]; then
    SOURCE_DIR="TPMCmd/Platform"
    CMAKE_BUILD_SUBDIR="platform"
fi

if [ "$DO_SIMLIB" -eq 1 ]; then
    SOURCE_DIR="TPMCmd/Simulator"
    CMAKE_BUILD_SUBDIR="simulator"
fi

if [ "$DO_TESTS" -eq 1 ]; then
    SOURCE_DIR="TPMCmd/tpm_tests"
    CMAKE_BUILD_SUBDIR="tpm_tests"
fi

cmake -B $BUILD_ROOT/$CMAKE_BUILD_SUBDIR -S $SOURCE_DIR $CMAKE_OPTS $CMAKE_GEN_OPTIONS 2>&1 | tee "$LOG_ROOT/gen_$CMAKE_BUILD_SUBDIR.log"
echo "generated via:"
echo "cmake -B $BUILD_ROOT/$CMAKE_BUILD_SUBDIR -S $SOURCE_DIR $CMAKE_OPTS $CMAKE_GEN_OPTIONS"
if [ $? -ne 0 ]; then
    echo "configure failed"
    exit 1
fi

echo "Generate succeeded"
exit 0
