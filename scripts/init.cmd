@echo off
@REM -- Configure build environment variables
@REM -- Usage: init.cmd CMakePath <developerDir>
@REM -- CMakePath: Local path to CMake runtime to be used.
@REM --            May be -- to skip and allow developer directory to set it up.
@REM -- DeveloperDir: Optional folder outside the source tree with a setenv.cmd
@REM --               script that can customize local dev tools.
@REM --
@REM -- This script exits with the following environment variables set:
@REM -- CMAKEPATH = CMake path given on command line
@REM -- DEVDIR = developer directory given on command line
@REM -- ROOT = Root directory of the working directory, parent of
@REM           folder containing init.cmd
@REM -- PATH = original PATH + CMAKEPATH + customizations from DEVDIR/setenv.cmd

@REM -- get root folder (containing init.cmd)
FOR %%I IN (%~dp0\..) DO @SET ROOT=%%~fI
echo ROOT=%ROOT%
cd %ROOT%

@REM -- capture CMAKEPATH
set CMAKEPATH=
if "%1" NEQ "--" (
    set CMAKEPATH=%1
)
shift
@REM -- Capture developer dir if given
set INIT=
if "%1" NEQ "" (
    set DEVDIR=%1
)

if [%DEVDIR%] NEQ [] (
    echo captured developer dir [%DEVDIR%]
) else (
    echo no developer dir given
)

echo post root %CD%
@REM -- add cmake to path, if given
if [%CMAKEPATH%] NEQ [] (
    echo adding %CMAKEPATH% to path
    path %PATH%;%CMAKEPATH%
) else (
    echo CMake path not given on command line, expect it setup externally.
)

@REM -- call developer setenv.cmd if provided
if EXIST %DEVDIR%\Setenv.cmd (
    echo Setting environment from %DEVDIR%\Setenv.cmd
    call %DEVDIR%\Setenv.cmd
)

@REM -- fini
