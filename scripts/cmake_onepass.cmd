@echo off
setlocal ENABLEDELAYEDEXPANSION
set DEBUG_CMAKE=%1
set MIN_BUILD=%2

set DO_NOT_CLEAN=0
if [%MIN_BUILD%]==[1] (
    set DO_NOT_CLEAN=1
)

if NOT EXIST %cd%\scripts\cmake_env.cmd (
    echo expect to run script from root of tpm repo
    exit /b 1
)

call %cd%\scripts\cmake_env.cmd %cd%\build\solo %DEBUG_CMAKE%
if [] equ [%BUILD_ROOT%] (
    echo cmake_env did not set a sensible root
    exit /b 1
)

if [%DO_NOT_CLEAN%] == [0] (
    rem full reset of the build folder
    if EXIST %BUILD_ROOT% (
        rd /s /q %BUILD_ROOT%
    )
    mkdir %BUILD_ROOT%
    rem LOG_ROOT must exist or TEE fails below
    mkdir %LOG_ROOT%
)

rem configure, build, install for all components in a single CMake pass
set CMAKE_GEN_OPTIONS=%CMAKE_GEN_OPTIONS_BASE% --install-prefix %INSTALL_ROOT%\combined
set SOURCE_DIR=TPMCmd\.
set CMAKE_BUILD_SUBDIR=tpm
call :RunTuple
echo Combined Build succeeded
goto :EOF

:RunTuple
rem run CMake configure, build, install
rem powershell 7 needed to fix some redirection issues with TEE.  can be replaced with
rem powershell by removing 2>&1 | tee part of command lines.
pwsh -Command "cmake -B %BUILD_ROOT%\%CMAKE_BUILD_SUBDIR% -S %SOURCE_DIR% %CMAKE_OPTS% %CMAKE_GEN_OPTIONS%  2>&1 | tee %LOG_ROOT%\gen_%CMAKE_BUILD_SUBDIR%.log"
if errorlevel 1 goto :FAIL
pwsh -Command "cmake --build %BUILD_ROOT%\%CMAKE_BUILD_SUBDIR% %CMAKE_OPTS% %CMAKE_BUILD_OPTS%  2>&1 | tee %LOG_ROOT%\build_%CMAKE_BUILD_SUBDIR%.log"
if errorlevel 1 goto :FAIL
pwsh -Command "cmake --install %BUILD_ROOT%\%CMAKE_BUILD_SUBDIR% %CMAKE_OPTS% %CMAKE_INSTALL_OPTS%   2>&1 | tee %LOG_ROOT%\install_%CMAKE_BUILD_SUBDIR%.log"
if errorlevel 1 goto :FAIL

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


