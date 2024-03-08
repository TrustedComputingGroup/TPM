# CMake Quick Start - TPM2 Reference Library

- [CMake Quick Start - TPM2 Reference Library](#cmake-quick-start---tpm2-reference-library)
  - [- Alternative Approach](#--alternative-approach)
- [Known Issues](#known-issues)
- [Prerequisites](#prerequisites)
  - [Install CMake & put it on your path](#install-cmake--put-it-on-your-path)
  - [Obtain the OpenSSL library](#obtain-the-openssl-library)
  - [Install Powershell 7 and put it on your path.](#install-powershell-7-and-put-it-on-your-path)
- [Build it](#build-it)
- [Alternative Approach](#alternative-approach)
---
NOTE: ** CMake support is experimental and does not yet support Linux. **

These instructions are under development and subject to change.

---

# Known Issues
1. Does not search for packages (such as OpenSSL); OpenSSL must be installed as
   system (on Linux) or as described in [Windows Build](Build.Windows.md).
2. Build failure with WOLF crypto libraries.

# Prerequisites

## Install CMake & put it on your path

This is performed in an OS specific way, possibilities include:
* using apt-get on Linux,
* Installing the MSI or Zip version of CMake directly from [CMake.org](https://cmake.org/download/)
* Using the CMake tools from a Visual Studio Installation

## Obtain the OpenSSL library

Follow the relevant [Build Instructions](../README.md#build-instructions) for your OS.

## Install Powershell 7 and put it on your path.

This is optional, but assumed by the cmake_* scripts because of occassional issued
with the `tee` command to capture and display logs simultaneously.

if you don't want this functionality you can edit the `cmake_*.cmd` scripts to call
powershell and ignore the resulting warnings, or remove `tee` entirely.

# Build it

After configuring your environment, you can configure the Tpm Core Library, platform
library, and simulator in a single CMake build system and build it.

Run `scripts\cmake_onepass.cmd`

# Alternative Approach

1. Install Visual Studio 2019 or 2022 and include the CMake recommended components.
2. Open the TPMCmd folder in Visual Studio as a "Folder" (do NOT open the `TpmSimulator.sln` solution)

Visual Studio should detect the CMakeLists.txt file and load and configure the CMake
environment automatically.  You will still need to configure OpenSSL.
