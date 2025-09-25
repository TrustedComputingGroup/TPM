
rem Configure environment expected by other CMake scripts.
rem Usage: cmake_env.cmd buildDir [D]
rem D if extra CMake Trace output desired
set BUILD_SUB_DIR=%1
set DEBUG_CMAKE=%2

rem these possibilitises based on the chosen VS2019 generator:
set BUILD_CONFIG=Debug
set BUILD_ARCH=x64
set BUILD_ARCH=Win32


set NATIVE_BUILD_OPTIONS=
rem must use single quotes for this since it's put inside quotes to be passed to
rem powershell -Command
rem VS Build generator with cmake_parts takes ~68-70s multi-pass, 51s in one pass.
set BUILD_GENERATOR='Visual Studio 16 2019'  -A %BUILD_ARCH%
rem use this for MSBUILD FULL DIAG
rem set NATIVE_BUILD_OPTIONS=-- '-v:diag'
rem NMake Makefiles takes ~88s multi-pass, 78s in one pass
rem set BUILD_GENERATOR='NMake Makefiles'
rem set NATIVE_BUILD_OPTIONS=-- /U
rem Ninja Makefiles takes ~36s multi-pass, 25s in one pass
rem uses the first compiler on the path, which can affect the perf gain somewhat
set BUILD_GENERATOR='Ninja'
rem set NATIVE_BUILD_OPTIONS=

rem use default crypto options
set CMAKE_TPM_CRYPTO_OPTIONS=

@REM combine options
set BUILD_NAME=%BUILD_CONFIG%_Ossl_%BUILD_ARCH%
set BUILD_ROOT=%BUILD_SUB_DIR%\%BUILD_NAME%
set INSTALL_ROOT=%BUILD_ROOT%\install
set LOG_ROOT=%BUILD_ROOT%

rem options for all CMake commands
set CMAKE_OPTS=

rem options for CMake generate
set CMAKE_CONFIG_DEBUG_OPTIONS=
if [%DEBUG_CMAKE%] equ [D] (
    set CMAKE_CONFIG_DEBUG_OPTIONS=--trace-expand --debug-find --debug-find-pkg TpmConfiguration,Tpm_CryptoLib_Common,Tpm_CryptoLib_Math_TpmBigNum_Headers,Tpm_CryptoLib_BnMath_Ossl,Tpm_CryptoLib_Math_TpmBigNum,Tpm_CoreLib
)

set CMAKE_GEN_OPTIONS_BASE=-G %BUILD_GENERATOR%  -Wdev --log-level=TRACE
set CMAKE_GEN_OPTIONS_BASE=%CMAKE_GEN_OPTIONS_BASE% %CMAKE_CONFIG_DEBUG_OPTIONS%
set CMAKE_GEN_OPTIONS_BASE=%CMAKE_GEN_OPTIONS_BASE% %CMAKE_TPM_CRYPTO_OPTIONS%

rem options CMake --build
set CONFIG_OPTS=--config %BUILD_CONFIG%
set CMAKE_BUILD_OPTS=--parallel 4 -v %CONFIG_OPTS%
set CMAKE_BUILD_OPTS=%CMAKE_BUILD_OPTS% %NATIVE_BUILD_OPTIONS%
rem options CMake --install
set CMAKE_INSTALL_OPTS=%CONFIG_OPTS%
