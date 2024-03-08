rem configure, then build+install the TPM using CMake with
rem three build domains: Core Library; Platform; Simulator
rem pass D for debug, or N/blank for not
set CMD_CMAKE_DEBUG_REQUEST=%1

call scripts\cmake_gen.cmd   1 0 0 %CMD_CMAKE_DEBUG_REQUEST%
call scripts\cmake_build.cmd 1 0 0 %CMD_CMAKE_DEBUG_REQUEST%
call scripts\cmake_gen.cmd   0 1 0 %CMD_CMAKE_DEBUG_REQUEST%
call scripts\cmake_build.cmd 0 1 0 %CMD_CMAKE_DEBUG_REQUEST%
call scripts\cmake_gen.cmd   0 0 1 %CMD_CMAKE_DEBUG_REQUEST%
call scripts\cmake_build.cmd 0 0 1 %CMD_CMAKE_DEBUG_REQUEST%
