@echo off
rem generate cmake environent for building CoreLib, Platform, Simulator as
rem separate components.
rem Usage cmake_gen.cmd 0 0 0 [D]
rem Set the desired component to 1.
rem since configure requires the previous builds to complete
rem and be installed, only one item should be a 1 on any call.
rem This is intended as an interactive/dev script, not for
rem use in a CI pipeline - it's use of %CD% is likely unsafe for
rem such an environment

setlocal ENABLEDELAYEDEXPANSION
set DO_TPMLIB=%1
set DO_PLATLIB=%2
set DO_SIMLIB=%3
set DEBUG_CMAKE=%4

if [%DO_TPMLIB%]==[] set DO_TPMLIB=0
if [%DO_PLATLIB%]==[] set DO_PLATLIB=0
if [%DO_SIMLIB%]==[] set DO_SIMLIB=0

set /A _TOTAL=%DO_TPMLIB% + %DO_PLATLIB% + %DO_SIMLIB%
if %_TOTAL% NEQ 1 (
    echo only one option may be Set
    exit /B 1
)

if NOT EXIST %cd%\scripts\cmake_env.cmd (
    echo expect to run script from root of tpm repo
    exit /b 1
)

call %cd%\scripts\cmake_env.cmd %cd%\build\parts %DEBUG_CMAKE%
if [] equ [%BUILD_ROOT%] (
    echo cmake_env did not set a sensible root
    exit /b 1
)

set CMAKE_GEN_OPTIONS=%CMAKE_GEN_OPTIONS_BASE% --install-prefix %INSTALL_ROOT%\parts

rem full reset of the build folder if starting with the library
rem also, the library configuration requires the LibOnly option
if [%DO_TPMLIB%] equ [1]  (
    if EXIST %BUILD_ROOT% (
        rd /s /q %BUILD_ROOT%
        mkdir %LOG_ROOT%
    )
    set SOURCE_DIR=TPMCmd\.
    set CMAKE_BUILD_SUBDIR=tpmlib
    set CMAKE_GEN_OPTIONS=%CMAKE_GEN_OPTIONS% -D Tpm_BuildOption_LibOnly=1
)

if [%DO_PLATLIB%] equ [1] (
    set SOURCE_DIR=TPMCmd\Platform
    set CMAKE_BUILD_SUBDIR=platform
)

if [%DO_SIMLIB%] equ [1] (
    set SOURCE_DIR=TPMCmd\Simulator
    set CMAKE_BUILD_SUBDIR=simulator
)

pwsh -Command "cmake -B %BUILD_ROOT%\%CMAKE_BUILD_SUBDIR% -S %SOURCE_DIR% %CMAKE_OPTS% %CMAKE_GEN_OPTIONS%  2>&1 | tee %LOG_ROOT%\gen_%CMAKE_BUILD_SUBDIR%.log"
if errorlevel 1 goto :FAIL
echo Generate succeeded
goto :EOF

:FAIL
@echo configure failed
rem fallthrough to ExitBatch

:ExitBatch
rem use Ctrl+C exit code to cleanly leave batch file
rem from any nesting level
echo Y > %temp%\yes.txt
call :CtrlC < %temp%\yes.txt
color

:CtrlC
cmd /c exit -1073741510
rem exit /B -1073741510

