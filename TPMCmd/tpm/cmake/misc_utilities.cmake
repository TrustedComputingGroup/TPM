# Microsoft Reference Implementation for TPM 2.0
# Copyright (c) Microsoft Corporation
# This software is being made available under certain license terms, as detailed at
# https://github.com/microsoft/ms-tpm-20-ref/blob/main/LICENSE
#

function(print_project_info)
    message("${PROJECT_NAME} start")
    message("GENERATOR: ${CMAKE_GENERATOR}")
    message("- VS: ${CMAKE_VS_PLATFORM_NAME}; VS_TOOLSET: ${CMAKE_VS_PLATFORM_TOOLSET}")
endfunction()

function(print_architecture_size)
    math(EXPR PlatformArchitecture "8 * ${CMAKE_SIZEOF_VOID_P}")
    message("- PlatformArchitecture: ${PlatformArchitecture}bit")
endfunction()
