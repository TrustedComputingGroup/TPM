@echo off
setlocal ENABLEDELAYEDEXPANSION
set DEBUG_CMAKE=%1

call scripts\cmake_onepass.cmd
if errorlevel 1 goto :FAIL

call scripts\cmake_parts.cmd
if errorlevel 1 goto :FAIL

@echo all builds succeeded
exit /B 0

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


