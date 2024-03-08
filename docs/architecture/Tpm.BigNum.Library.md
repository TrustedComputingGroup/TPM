<!-- Copyright Microsoft Corporation. All Rights Reserved
     Licensed subject to: https://github.com/microsoft/ms-tpm-20-ref/blob/main/LICENSE -->

# TPM Reference Code - TpmBigNum Library Design

- [TPM Reference Code - TpmBigNum Library Design](#tpm-reference-code---tpmbignum-library-design)
  - [See Also](#see-also)
  - [Background](#background)
  - [Design](#design)
  - [Specifying the sub-libraries](#specifying-the-sub-libraries)

## See Also

- [Introduction](Introduction.md)
- [Library Architecture](Library.Architecture.md)
- [Tpm BigNum Library](Tpm.BigNum.Library.md)
- [Tpm Crypto Libraries](Tpm.Crypto.Libraries.md)
- [Tpm Math Library](Tpm.Math.Library.md)
- [Tpm Platform API](Tpm.Platform.Api.md)


## Background

This document describes the `TpmBigNum` Math library, which is the Reference
Code implementation of the MATH_LIB interface. The detailed MATH_LIB interface
is described in [TPM Math Library](Tpm.Math.Library.md).  This document
describes the internal implementation of `TpmBigNum`.

In particular, `TpmBigNum` consumes and requires a helper asymmetric math
library to provide complex big-number math operations.  The reference code
currently supports wolfssl and OpenSSL libraries as sub-providers of large
number crypto operations.

## Design

`TpmBigNum` uses the `bigNum`, and `bigPoint` types to hold numeric information.
`TpmBigNum` respects the BIG_ENDIAN_TPM macro to reflect how data is stored in
memory. `TpmBigNum` provides all the stubs required by the Tpm Math Library
interface, and some of the (constant and linear time) `bigNum` math operations.
More complex operations such as exponentiation, division, multiplication, and
modular operations are provided by the sub-library.  All sub-libraries must use
the same `bigNum` format for the underlying memory and satisfy the
`BnSupport_interface.h` interface. The historical strategy is to provide wrapper
functions that convert in and out of the library-native format into the bigNum
library format automatically.  If a library wishes to avoid the overhead of
these conversions then they can target the entire MATH_LIB interface instead.

## Specifying the sub-libraries

Similar to the `Core` library's use of `MATH_LIB`, `TpmBigNum` also uses a
preprocessor macro to indicate the sub-library it will use.

| Build Flag  | Header Name               | Description                                           |
| :---------- | :------------------------ | :---------------------------------------------------- |
| BN_MATH_LIB | BnTo`[BN_MATH_LIB]`Math.h | Math Interface used to provide bigNum implementations |
