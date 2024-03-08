@echo off
@REM -- this script provides some environment initialization
@REM -- it sets the environment variable ROOT to the folder containing
@REM -- init.cmd.  It also sets INIT to the first command line parameter
@REM -- and calls a personal initialization script if it exists.
set INIT=
if "%1" NEQ "" (
   set INIT=%1
)

if [%INIT%] NEQ [] (
   echo captured developer dir [%INIT%]
 else (
   echo no developer dir given
)

echo -- get root folder (containing init.cmd)
FOR %%I IN (%~dp0\TPMCmd\..) DO @SET ROOT=%%~fI
cd %ROOT%

@REM call developer setenv.cmd if provided
if EXIST %INIT%\Setenv.cmd (
    echo Setting environment from %INIT%\Setenv.cmd
    call %INIT%\Setenv.cmd
)
