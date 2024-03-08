@echo off
rem generate cmake environent for building CoreLib, Platform, Simulator as
rem separate components.
rem Usage cmake_build.cmd 0 0 0 [D]
rem Set the desired component to 1.
rem since configure requires the previous buils to complete
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
rem first param must match cmake_gen.cmd
call %cd%\scripts\cmake_env.cmd %cd%\build\parts %DEBUG_CMAKE%
if [] equ [%BUILD_ROOT%] (
    echo cmake_env did not set a sensible root
    exit /b 1
)

if [%DO_TPMLIB%] equ [1]  (
    set SOURCE_DIR=TPMCmd\.
    set CMAKE_BUILD_SUBDIR=tpmlib
)

if [%DO_PLATLIB%] equ [1] (
    set SOURCE_DIR=TPMCmd\Platform
    set CMAKE_BUILD_SUBDIR=platform
)
if [%DO_SIMLIB%] equ [1] (
    set SOURCE_DIR=TPMCmd\Simulator
    set CMAKE_BUILD_SUBDIR=simulator
)

pwsh -Command "cmake --build %BUILD_ROOT%\%CMAKE_BUILD_SUBDIR% %CMAKE_OPTS% %CMAKE_BUILD_OPTS%  2>&1 | tee %LOG_ROOT%\build_%CMAKE_BUILD_SUBDIR%.log"
if errorlevel 1 goto :FAIL
pwsh -Command "cmake --install %BUILD_ROOT%\%CMAKE_BUILD_SUBDIR% %CMAKE_OPTS% %CMAKE_INSTALL_OPTS%   2>&1 | tee %LOG_ROOT%\install_%CMAKE_BUILD_SUBDIR%.log"
if errorlevel 1 goto :FAIL
echo Build and install succeeded

goto :EOF

:FAIL
@echo build or install failed
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
