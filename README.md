# Official TPM 2.0 Reference Implementation #


- [Official TPM 2.0 Reference Implementation (by Microsoft)](#official-tpm-20-reference-implementation-by-microsoft)
  - [Build Status](#build-status)
  - [Introduction](#introduction)
  - [Architecture Introduction](#architecture-introduction)
  - [Build Instructions](#build-instructions)

## Introduction
This is the official TCG reference implementation of the [TPM 2.0 Specification](https://trustedcomputinggroup.org/tpm-library-specification). The project contains complete source code of the reference implementation with various [Build Options](#build-instructions).

This repository includes a  [TPM 2.0 simulator](TPMCmd/Simulator) that emulates a TPM 2.0 device and can be accessed via a custom TCP based protocol.  This allows experimentation and testing of the reference code.  The simplest way to work with the simulator is to use a [TSS library](https://github.com/Microsoft/TSS.MSR) for the programming language of your choice - C#/.Net, C++, Java, Python, JavaScript/Node.js are currently supported. The C language TSS implementing the TCG's TSS API specification is available [here](https://github.com/tpm2-software/tpm2-tss).

## Architecture Introduction
An explanation of the architecture for the TPM Reference Code.

See [Architecture Intro](docs/architecture/Introduction.md)

## Build Instructions
The supported build environments are not guaranteed and subject to change.

See [Build Intro](docs/BuildSystems/BuildIntro.md)
