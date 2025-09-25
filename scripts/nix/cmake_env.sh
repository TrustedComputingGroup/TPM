#!/bin/bash

# Configure environment expected by other CMake scripts.
# Usage: cmake_env.sh buildDir [D]
# D if extra CMake Trace output desired

BUILD_SUB_DIR=$1
DEBUG_CMAKE=$2

# these possibilities based on the chosen VS2019 generator:
BUILD_CONFIG=Debug
BUILD_ARCH=x64
# BUILD_ARCH=Win32

NATIVE_BUILD_OPTIONS=""
# must use single quotes for this since it's put inside quotes to be passed to
# powershell -Command
# VS Build generator with cmake_parts takes ~68-70s multi-pass, 51s in one pass.
#BUILD_GENERATOR="Visual Studio 16 2019 -A $BUILD_ARCH"
# use this for MSBUILD FULL DIAG
# NATIVE_BUILD_OPTIONS="-- '-v:diag'"
# NMake Makefiles takes ~88s multi-pass, 78s in one pass
# BUILD_GENERATOR="NMake Makefiles"
# NATIVE_BUILD_OPTIONS="-- /U"
# Ninja Makefiles takes ~36s multi-pass, 25s in one pass
# uses the first compiler on the path, which can affect the perf gain somewhat
BUILD_GENERATOR="Ninja"
# NATIVE_BUILD_OPTIONS=""

# use default crypto options
CMAKE_TPM_CRYPTO_OPTIONS=""

# combine options
BUILD_NAME="${BUILD_CONFIG}_Ossl_${BUILD_ARCH}"
BUILD_ROOT="${BUILD_SUB_DIR}/${BUILD_NAME}"
INSTALL_ROOT="${BUILD_ROOT}/install"
LOG_ROOT="${BUILD_ROOT}"

# options for all CMake commands
CMAKE_OPTS=""

# options for CMake generate
CMAKE_CONFIG_DEBUG_OPTIONS=""
if [ "$DEBUG_CMAKE" == "D" ]; then
    #CMAKE_CONFIG_DEBUG_OPTIONS="--trace-expand --debug-find --debug-find-pkg TpmConfiguration,Tpm_CryptoLib_Common,Tpm_CryptoLib_Math_TpmBigNum_Headers,Tpm_CryptoLib_BnMath_Ossl,Tpm_CryptoLib_Math_TpmBigNum,Tpm_CoreLib"
    CMAKE_CONFIG_DEBUG_OPTIONS="--trace-expand --debug-find --debug-output"
fi

CMAKE_GEN_OPTIONS_BASE="-G $BUILD_GENERATOR -Wdev --log-level=TRACE"
CMAKE_GEN_OPTIONS_BASE="$CMAKE_GEN_OPTIONS_BASE $CMAKE_CONFIG_DEBUG_OPTIONS"
CMAKE_GEN_OPTIONS_BASE="$CMAKE_GEN_OPTIONS_BASE $CMAKE_TPM_CRYPTO_OPTIONS"

# options CMake --build
#CONFIG_OPTS="--config $BUILD_CONFIG"
CONFIG_OPTS=
CMAKE_BUILD_OPTS="--parallel 4 -v $CONFIG_OPTS"
CMAKE_BUILD_OPTS="$CMAKE_BUILD_OPTS $NATIVE_BUILD_OPTIONS"
# options CMake --install
CMAKE_INSTALL_OPTS="$CONFIG_OPTS"
