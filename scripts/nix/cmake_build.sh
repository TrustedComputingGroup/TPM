#!/bin/bash

# generate cmake environment for building CoreLib, Platform, Simulator as
# separate components.
# Usage: cmake_build.sh 0 0 0 [D]
# Set the desired component to 1.
# since configure requires the previous builds to complete
# and be installed, only one item should be a 1 on any call.
# This is intended as an interactive/dev script, not for
# use in a CI pipeline - it's use of $PWD is likely unsafe for
# such an environment

set -e

DO_TPMLIB=${1:-0}
DO_PLATLIB=${2:-0}
DO_SIMLIB=${3:-0}
DO_TESTS=${4:-0}
DEBUG_CMAKE=${5:-}

TOTAL=$((DO_TPMLIB + DO_PLATLIB + DO_SIMLIB + DO_TESTS))
if [ "$TOTAL" -ne 1 ]; then
    echo "only one option may be set"
    exit 1
fi

if [ ! -f "$PWD/scripts/nix/cmake_env.sh" ]; then
    echo "expect to run script from root of tpm repo"
    exit 1
fi

# first param must match cmake_gen.sh
source "$PWD/scripts/nix/cmake_env.sh" "$PWD/build/parts" "$DEBUG_CMAKE"
if [ -z "$BUILD_ROOT" ]; then
    echo "cmake_env did not set a sensible root"
    exit 1
fi

if [ "$DO_TPMLIB" -eq 1 ]; then
    SOURCE_DIR="TPMCmd/."
    CMAKE_BUILD_SUBDIR="tpmlib"
elif [ "$DO_PLATLIB" -eq 1 ]; then
    SOURCE_DIR="TPMCmd/Platform"
    CMAKE_BUILD_SUBDIR="platform"
elif [ "$DO_SIMLIB" -eq 1 ]; then
    SOURCE_DIR="TPMCmd/Simulator"
    CMAKE_BUILD_SUBDIR="simulator"
elif [ "$DO_TESTS" -eq 1 ]; then
    SOURCE_DIR="TPMCmd/tpm_tests"
    CMAKE_BUILD_SUBDIR="tpm_tests"
fi

cmake --build "$BUILD_ROOT/$CMAKE_BUILD_SUBDIR" $CMAKE_OPTS $CMAKE_BUILD_OPTS 2>&1 | tee "$LOG_ROOT/build_$CMAKE_BUILD_SUBDIR.log"
if [ "${PIPESTATUS[0]}" -ne 0 ]; then
    echo "build failed"
    exit 1
fi

cmake --install "$BUILD_ROOT/$CMAKE_BUILD_SUBDIR" $CMAKE_OPTS $CMAKE_INSTALL_OPTS 2>&1 | tee "$LOG_ROOT/install_$CMAKE_BUILD_SUBDIR.log"
if [ "${PIPESTATUS[0]}" -ne 0 ]; then
    echo "install failed"
    exit 1
fi

echo "Build and install succeeded"
