<!-- Copyright Microsoft Corporation. All Rights Reserved
     Licensed subject to: https://github.com/microsoft/ms-tpm-20-ref/blob/main/LICENSE -->

# Code Layout
- [Code Layout](#code-layout)
  - [Introduction](#introduction)
  - [Form Follows Function](#form-follows-function)
  - [Tpm Reference Code (TPMCmd folder)](#tpm-reference-code-tpmcmd-folder)
  - [Tpm Core Library (TPMCmd/tpm folder)](#tpm-core-library-tpmcmdtpm-folder)
  - [Tpm Core Library Includes (TPMCmd/tpm/include folder)](#tpm-core-library-includes-tpmcmdtpminclude-folder)
  - [Tpm Core Library CryptoLibs (TPMCmd/tpm/cryptolibs folder)](#tpm-core-library-cryptolibs-tpmcmdtpmcryptolibs-folder)
  - [Platform Library (TPMCmd/Platform folder)](#platform-library-tpmcmdplatform-folder)

## Introduction

The code layout of the repository consists of the several top-level folders.
Portions of the repository are provided for example or historical reasons and are
unmaintained.  Pull Requests to update or fix non-maintained code are considered
subject to [CONTRIBUTING.md](../../CONTRIBUTING.md)

| Folder   | Description                                          | Maintained? |
| -------- | ---------------------------------------------------- | :---------: |
| docs     | These documents                                      | YES         |
| external | WolfSSL submodule (currently not building correctly) | NO          |
| Samples  | Misc Sample TPM applications                         | NO          |
| scripts  | miscellaneous scripts                                | YES         |
| TPMCmd   | TPM Reference Code and Library                       | YES         |

## Form Follows Function

The physical directory layout of the reference code is based on achieving two
simultaneous goals:
1. to build the TCG Reference Code, Simulator, and Platform library.
2. to allow library consumers to consume only library components while injecting
changes at multiple customization points. Items designated Replaceable can be
replaced with user components as described in other documents to construct a
different TPM application.

In order to achieve these goals in a way that can be consumed by the greatest
variety of downstream build systems, the Core Library CMake build system
outputs an "install tree" consisting of headers and libraries that can be
consumed by various downstream build systems as well as a set of CMake Packages
that can be consumed via CMake `find_package` if the downstream system uses CMake.

The folder layout of the `TPMCmd` source tree reflects these CMake build
decisions as described in [CMake Build](Build.CMake.md).

The remainder of this document outlines what is in the top-level folders of the
repository.

## Tpm Reference Code (TPMCmd folder)

The structure of `TPMCmd` is:

| Folder           | Description                               | Replaceable? |
| ---------------- | ----------------------------------------- | :----------: |
| Platform         | The Reference example platform library    | YES          |
| Simulator        | The Reference test application            | YES          |
| tpm              | The TPM Core Library                      | NO           |
| TpmConfiguration | The TPM Configuration files for Simulator | YES          |

<!-- New names
| Tpm_PlatformLib   | The Reference example platform library    | YES          |
| Tpm_Simulator     | The Reference test application            | YES          |
| Tpm_Core          | The TPM Core Library                      | NO           |
| Tpm_Configuration | The TPM Configuration files for Simulator | YES          |
-->

## Tpm Core Library (TPMCmd/tpm folder)

The structure of `TPMCmd/tpm`:

| Folder     | Description                                  | Maintained? | Replaceable? |
| ---------- | -------------------------------------------- | :---------: | :----------: |
| cmake      | CMake build scripts and helpers              | YES         | NO           |
| cryptolibs | Core and Sample crypto libraries (see below) | VARIES      | VARIES       |
| include    | Various TPM  Header libraries (see below)    | YES         | NO           |
| src        | The TPM Core Library                         | YES         | NO           |

## Tpm Core Library Includes (TPMCmd/tpm/include folder)

The structure of `TPMCmd/tpm/include`:

| Folder             | Description                                  | Maintained? | Replaceable? |
| ------------------ | -------------------------------------------- | :---------: | :----------: |
| platform_interface | CMake build scripts                          | YES         | NO           |
| private            | Core and Sample crypto libraries (see below) | YES         | NO           |
| public             | Various TPM  Header libraries (see below)    | YES         | NO           |


## Tpm Core Library CryptoLibs (TPMCmd/tpm/cryptolibs folder)

The cryptolibs folder contains both maintained code and legacy code that is kept
for illustration but is not currently maintained.  In particular wolf crypto
implementations do not correctly build at present.

The structure of `TPMCmd/tpm/cryptolibs`:

| Folder    | Description                           | Maintained? | Replaceable? | Requires TpmBigNum? |
| --------- | ------------------------------------- | :---------: | :----------: | :-----------------: |
| common    | Core TPM Crypto Interface definitions | YES         | NO           | N/A                 |
| Ossl      | OpenSSL Crypto Interface layer        | YES         | YES          | YES                 |
| TpmBigNum | The TPM "Big Number" Math Library     | YES         | YES          | N/A                 |
| wolf      | WolfSSL Crypto Interface layer        | NO          | YES          | YES                 |

More details about the Crypto design and customization can be found under [Architecture](../architecture/Introduction.md):

## Platform Library (TPMCmd/Platform folder)

| Folder  | Description                    | Maintained? | Replaceable? |
| ------- | ------------------------------ | :---------: | :----------: |
| include | Tpm_PlatformLib headers        | YES         | YES          |
| src     | Tpm_PlatformLib source         | YES         | YES          |

