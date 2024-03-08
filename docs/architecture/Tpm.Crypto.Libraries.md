<!-- Copyright Microsoft Corporation. All Rights Reserved
     Licensed subject to: https://github.com/microsoft/ms-tpm-20-ref/blob/main/LICENSE -->

# TPM Reference Code - TPM Crypto Library Introduction

- [TPM Reference Code - TPM Crypto Library Introduction](#tpm-reference-code---tpm-crypto-library-introduction)
  - [See Also](#see-also)
  - [Background](#background)
  - [Components of a Cryptographic Protocol](#components-of-a-cryptographic-protocol)
  - [TPM Crypto Support Library Design](#tpm-crypto-support-library-design)
    - [Implications](#implications)
  - [Specifying the libraries](#specifying-the-libraries)

## See Also

- [Introduction](Introduction.md)
- [Library Architecture](Library.Architecture.md)
- [Tpm BigNum Library](Tpm.BigNum.Library.md)
- [Tpm Crypto Libraries](Tpm.Crypto.Libraries.md)
- [Tpm Math Library](Tpm.Math.Library.md)
- [Tpm Platform API](Tpm.Platform.Api.md)


## Background

As mentioned in [Library Architecture](Library.Architecture.md), any TPM
implementation using this Core library is expected to provide several additional
libraries or components. This document describes the Crypto support libraries at
a high level.

## Components of a Cryptographic Protocol

Roughly speaking, any usable end-to-end crypto protocol breaks down into some
combination of four groups of things:

| Group | Algorithm Category              | Examples                                                                    |
| :---: | :------------------------------ | :-------------------------------------------------------------------------- |
|   1   | Symmetric primitives            | Expand AES Key Schedule<br>Encrypt a single 16-byte block                   |
|   2   | Hash primitives                 | Start Hash<br>Hash Block<br>End Hash                                        |
|   3   | Big Number (BN) math primitives | Compute a^e mod N, where a,e,N are “big” integers                           |
|   4   | Cryptographic "Protocol"        | Details building these primitives into end-to-end cryptographic operations. |

Examples may help make this clearer.  Depending on mode, encrypting the simplest
message (a single block of data) with AES involves at least two operations from
category 1:

- generating a key schedule, and
- performing the block encryption.

Similarly, hashing a message requires at least three operations from category 2:

- initializing the hash state, and
- extending the data block(s), and
- finalizing the hash.

Note: Correct Category 4 implementation is critical to security - many practical
cryptographic weaknesses stem from errors in how the primitives are applied,
even when the primitives are themselves secure.

## TPM Crypto Support Library Design

The `Core` implementation strategy has been to link with (potentially separate)
crypto libraries that provide the primitives from groups 1 & 2 and (a
_**subset**_ of) 3.  The Core TPM reference library provides the remaining Group
3 and Group 4 code to create meaningful end-to-end cryptographic operations.

The interface to external libraries is implemented in the form of C language
macros and function calls.

### Implications

This design has pros and cons:
| Pro                                                                                                              | Con                                                                    |
| :--------------------------------------------------------------------------------------------------------------- | :--------------------------------------------------------------------- |
| Minimal requirements on crypto support libraries.                                                                | Difficult to take advantage of optimized higher-level crypto libraries |
| TPM specification able to evolve to support new commands and protocols without requiring crypto library changes. | Extra data conversions between internal and support library formats    |
| When supported by the algorithm, allows blockwise/stream processing where the data would otherwise be too large. |                                                                        |
| Allows the Core library to do parent/child key derivation as described in the TPM spec.                          |                                                                        |

This means the existing `Core` is not "plug-ready" to accept another
cryptographic library that (for example) wants to provide all of groups 3 or 4
such as generating primary asymmetric keys, or an AES library that exposes only
higher-level operations like `EncryptMessageAesCbc(message, length)`.

This is true of symmetric block chaining implementations and key derivation
functions, but has the greatest impact on asymmetric crypto implementation
because the necessary API surface to support Symmetric and Hash operations is
much smaller that the generalized large-number API surface required to implement
arbitrary asymmetric crypto.  Implementations could replace higher-level pieces
of the `Core` library (e.g. the entire `src/crypt` folder) to directly provide
desired behavior.  This requires the crypto library (or a middle layer) to
effectively rewrite TPM-specific code to understand TPM constructs rather than
being an arbitrary cryptographic library.

## Specifying the libraries

Any crypto library must provide a set of header files that are the interface
between the `Core` Library and the particular crypto implementation. The `Core`
code is informed of the name of these header files by three compile-time
defines:

| Build Flag | Interface File          | Description                                                  |
| :--------- | :---------------------- | :----------------------------------------------------------- |
| HASH_LIB   | TpmTo`[HASH_LIB]`Hash.h | Hashing primitives to provide hashing and HMAC functionality |
| SYM_LIB    | TpmTo`[SYM_LIB]`Sym.h   | Symmetric (e.g. AES) primitives                              |
| MATH_LIB   | TpmTo`[MATH_LIB]`Math.h | Math Interface used to provide asymmetric implementations    |

Each library must provide a header that matches one of the patterns above.

The `TPMCmd` folder provides the following implementations for these three interfaces:

| Build Flag | Flag Value | Description                                                                                  |
| :--------- | :--------- | :------------------------------------------------------------------------------------------- |
| HASH_LIB   | Ossl       | OpenSSL Hash implementation                                                                  |
| SYM_LIB    | Ossl       | OpenSSL AES implementation                                                                   |
| MATH_LIB   | TpmBigNum  | TPM "bigNum" based asymmetric Math library.  See [TPM BigNum Library](Tpm.BigNum.Library.md) |

At run-time, a simulator provides information about its crypto libraries via
the `GetCrypto{Sym,Hash,Math}Impl` RPCs on the platform port.
