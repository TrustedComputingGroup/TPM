rem build+install the TPM using CMake with
rem three build domains: Core Library; Platform; Simulator
rem don't configure, which makes for fast partial builds, but
rem means this script can't be used from a completely empty build environment,
rem use cmake_parts.cmd first.
rem pass D for debug, or N/blank for not
set CMD_CMAKE_DEBUG_REQUEST=%1

rem call scripts\cmake_gen.cmd   1 0 0 %CMD_CMAKE_DEBUG_REQUEST%
call scripts\cmake_build.cmd 1 0 0 %CMD_CMAKE_DEBUG_REQUEST%
rem call scripts\cmake_gen.cmd   0 1 0 %CMD_CMAKE_DEBUG_REQUEST%
call scripts\cmake_build.cmd 0 1 0 %CMD_CMAKE_DEBUG_REQUEST%
rem call scripts\cmake_gen.cmd   0 0 1 %CMD_CMAKE_DEBUG_REQUEST%
call scripts\cmake_build.cmd 0 0 1 %CMD_CMAKE_DEBUG_REQUEST%
