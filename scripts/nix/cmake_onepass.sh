#!/bin/bash

RunTuple() {
    # run CMake configure, build, install
    # powershell 7 needed to fix some redirection issues with TEE. can be replaced with
    # powershell by removing 2>&1 | tee part of command lines.
    cmake -B $BUILD_ROOT/$CMAKE_BUILD_SUBDIR -S $SOURCE_DIR $CMAKE_OPTS $CMAKE_GEN_OPTIONS  2>&1 | tee $LOG_ROOT/gen_$CMAKE_BUILD_SUBDIR.log
    if [ $? -ne 0 ]; then
        FAIL
    fi
    cmake --build $BUILD_ROOT/$CMAKE_BUILD_SUBDIR $CMAKE_OPTS $CMAKE_BUILD_OPTS  2>&1 | tee $LOG_ROOT/build_$CMAKE_BUILD_SUBDIR.log
    if [ $? -ne 0 ]; then
        FAIL
    fi
    cmake --install $BUILD_ROOT/$CMAKE_BUILD_SUBDIR $CMAKE_OPTS $CMAKE_INSTALL_OPTS  2>&1 | tee $LOG_ROOT/install_$CMAKE_BUILD_SUBDIR.log
    if [ $? -ne 0 ]; then
        FAIL
    fi
}

FAIL() {
    echo "build or install failed"
    ExitBatch
}

ExitBatch() {
    # use Ctrl+C exit code to cleanly leave batch file
    # from any nesting level
    echo "Y" > /tmp/yes.txt
    CtrlC < /tmp/yes.txt
    tput setaf 7
}

CtrlC() {
    exit 130
}

DEBUG_CMAKE=$1
MIN_BUILD=$2

DO_NOT_CLEAN=0
if [ "$MIN_BUILD" == "1" ]; then
    DO_NOT_CLEAN=1
fi

if [ ! -f "$(pwd)/scripts/nix/cmake_env.sh" ]; then
    echo "expect to run script from root of tpm repo"
    exit 1
fi

source "$(pwd)/scripts/nix/cmake_env.sh" "$(pwd)/build/solo" "$DEBUG_CMAKE"
if [ -z "$BUILD_ROOT" ]; then
    echo "cmake_env did not set a sensible root"
    exit 1
fi

if [ "$DO_NOT_CLEAN" == "0" ]; then
    # full reset of the build folder
    if [ -d "$BUILD_ROOT" ]; then
        rm -rf "$BUILD_ROOT"
    fi
    mkdir -p "$BUILD_ROOT"
    # LOG_ROOT must exist or TEE fails below
    mkdir -p "$LOG_ROOT"
fi

# configure, build, install for all components in a single CMake pass
CMAKE_GEN_OPTIONS="$CMAKE_GEN_OPTIONS_BASE --install-prefix $INSTALL_ROOT/combined"
SOURCE_DIR="TPMCmd/."
CMAKE_BUILD_SUBDIR="tpm"
RunTuple
echo "Combined Build succeeded"
exit 0
