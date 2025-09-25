# Build Intro

* [Build Intro](#build-intro)
  * [Supported Crypto Libraries](#supported-crypto-libraries)
  * [Build Options](#build-options)
  * [Future Work](#future-work)
  * [Code Layout](#code-layout)
  * [Build Instructions](#build-instructions)

## Supported Crypto Libraries

A compatible crypto library is required in order to build the TPM simulator.

| Crypto library | Supported Minimum Version |
| -------------- | ------------------------- |
| OpenSSL 1.x    | `1.1.1n` (March 2022)     |
| OpenSSL 3.x    | `3.0.10` (August 2023)    |
| WolfSSL        | Not supported             |

Bindings for WolfSSL are provided on a best-effort basis, but WolfSSL is not
currently tested or supported by any of the build documentation.

## Build Options

The TPM Reference Core Library is intended to be portable across any
environment.  The Simulator Test application requires a full operating system
with networking support.

This repo currently supports building with CMake for Windows, Linux, and OS X.
In addition, certain versions of Visual Studio are supported on Windows.

Pull requests to fix build issues will be entertained from the community
subject to the [Contributing Guidelines](../../CONTRIBUTING.md)

| Build Tool         | Operating System | Supported?               |
| ------------------ | ---------------- | ------------------------ |
| CMake              | Windows          | YES                      |
| CMake              | Linux            | YES                      |
| CMake              | OS X             | YES                      |
| Visual Studio 2017 | Windows          | YES, Deprecation planned |
| Visual Studio 2022 | Windows          | NO, Future plan          |

## Code Layout

The contents and high-level layout of the source code is described by
[CodeLayout](CodeLayout.md)

## Build Instructions

* [CMake](Build.CMake.md)
* [Windows](Build.Windows.md)
* [Linux](Build.Linux.md)
* [MacOsX](Build.OsX.md)
