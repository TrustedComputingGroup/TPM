
@echo off
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

goto :SkipUsage

:Usage

    echo    usage: getossl.cmd arch folder
    echo examples: getossl.cmd x86 c:\openssl
    echo           getossl.cmd x64 c:\opensslx64
    echo.
    echo WARNING: This command is destructive - it will force-delete the existing local Openssl copy.
    echo.
    goto :ExitBatch

::------------------------------------
:: use Ctrl+C exit code to cleanly leave batch file
:ExitBatch
echo Y > %temp%\yes.txt
call :CtrlC < %temp%\yes.txt
color

:CtrlC
cmd /c exit -1073741510
::exit /B -1073741510

:SkipUsage

set FLAVOR=%1
rem contains absolute path
set OPENSSL_DIR=%2

if [%2] equ [] (
    echo.
    echo ERROR: must specify directory of already compiled openssl library.
    goto :Usage
)
if NOT EXIST %2 (
    echo.
    echo OpenSSL folder [%2] not found.
    goto :Usage
)

if NOT EXIST TpmCmd\ (
    echo.
    echo ERROR: TpmCmd folder not found
    echo must be run above the TpmCmd folder in the TPM source tree.
    goto :Usage
)

if [%FLAVOR%] neq [x86] (
    if [%FLAVOR%] neq [x64] (
        echo.
        echo ERROR: architecture is invalid or unsupported
        echo FLAVOR must be x86 or x64, was [%FLAVOR%]
        goto :Usage
    )
)

rem contains path relative to TpmCmd
set TARGET_LIB_FOLDER_ROOT=lib
set TARGET_LIB_FOLDER=
set TARGET_INC_FOLDER_ROOT=OsslInclude
set TARGET_INC_FOLDER=

if [%FLAVOR%] equ [x86] (
    set TARGET_LIB_FOLDER=%TARGET_LIB_FOLDER_ROOT%
    set TARGET_INC_FOLDER=%TARGET_INC_FOLDER_ROOT%
) else (
    if [%FLAVOR%] equ [x64] (
        set TARGET_LIB_FOLDER=%TARGET_LIB_FOLDER_ROOT%\x64
        set TARGET_INC_FOLDER=%TARGET_INC_FOLDER_ROOT%\x64
    )
)

rem *** DO COPIES ***

pushd TpmCmd\
rd /s /q %TARGET_LIB_FOLDER_ROOT%
rd /s /q %TARGET_INC_FOLDER_ROOT%

mkdir %TARGET_LIB_FOLDER_ROOT%
mkdir %TARGET_INC_FOLDER_ROOT%

if NOT EXIST %TARGET_LIB_FOLDER% (
    mkdir %TARGET_LIB_FOLDER%
)

if NOT EXIST %TARGET_INC_FOLDER% (
    mkdir %TARGET_INC_FOLDER%
)

copy %OPENSSL_DIR%\libcrypto.lib %TARGET_LIB_FOLDER%
copy %OPENSSL_DIR%\lib\libcrypto.lib %TARGET_LIB_FOLDER%
copy %OPENSSL_DIR%\libcrypto-1_1*.dll %TARGET_LIB_FOLDER%
copy %OPENSSL_DIR%\bin\libcrypto-1_1*.dll %TARGET_LIB_FOLDER%
@echo on
xcopy %OPENSSL_DIR%\include\openssl\*  %TARGET_INC_FOLDER%\openssl\*
