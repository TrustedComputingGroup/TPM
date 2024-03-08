<!-- Copyright Microsoft Corporation. All Rights Reserved
     Licensed subject to: https://github.com/microsoft/ms-tpm-20-ref/blob/main/LICENSE -->

# TPM Reference Code - Introduction

- [TPM Reference Code - Introduction](#tpm-reference-code---introduction)
  - [See Also](#see-also)
  - [Introduction](#introduction)
  - [Status of this documentation](#status-of-this-documentation)
  - [Cryptographic Prerequisites](#cryptographic-prerequisites)
  - [Notations](#notations)
  - [Abbreviations & Terms](#abbreviations--terms)
  - [References](#references)

## See Also

- [Introduction](Introduction.md)
- [Library Architecture](Library.Architecture.md)
- [Tpm BigNum Library](Tpm.BigNum.Library.md)
- [Tpm Crypto Libraries](Tpm.Crypto.Libraries.md)
- [Tpm Math Library](Tpm.Math.Library.md)
- [Tpm Platform API](Tpm.Platform.Api.md)

## Introduction

This documentation describes the TPM Reference Code contained in this repository.
It describes the code's implementation in terms of its technical architecture and layout.

## Status of this documentation

Certain parts of the Reference code, as published in the Trusted Computing Group TPM2.0 specification (see [References](#References)) are _NORMATIVE_ to the TPM 2.0 Specification.
As detailed in the Specification, it is the _behavior_ of the code (vis-a-vis the _implementation_) that is _normative_.

However, this documentation is a living document whose goal is to describe the implementation of this particular repository. Therefore all of this documentation, and much of the code, in this repository is _informative_ and should not be considered part of the TCG TPM 2.0 specification.

All documentation in this repository should be considered a combined whole; thus when the text refers to "this document", the reference is to the combined documentation contained
in this code repository unless context requires otherwise.

## Cryptographic Prerequisites

This documentation assumes the reader is well versed in the relevant cryptographic concepts and terminology and will not necessarily explain common terms or abbreviations.

## Notations

Mathematical formulae in this document use the caret `^` to indicate exponentiation operations unless otherwise noted.  For example: 2^3 = 8.  This differs from the XOR meaning of `^` in C code.

## Abbreviations & Terms
| Term | Meaning                                                   |
| :--- | :-------------------------------------------------------- |
| ECC  | Elliptic Curve Cryptography (not Error Correction Coding) |
| RSA  | The RSA cryptographic algorithm and related topics        |
| TCG  | Trusted Computing Group.                                  |

## References

These references were links to the current versions at the time of this writing, but may be be the most current now. All references in any part of this documentation will refer to this list by number.

1. [Latest TCG TPM 2.0 Specification](https://trustedcomputinggroup.org/resource/tpm-library-specification)
2. [TPM2 Specification Part 1 - Architecture v1.59](https://trustedcomputinggroup.org/wp-content/uploads/TCG_TPM2_r1p59_Part1_Architecture_pub.pdf)
3. [TPM2 Specification Part 2 - Structures v1.59](https://trustedcomputinggroup.org/wp-content/uploads/TCG_TPM2_r1p59_Part2_Structures_pub.pdf)
4. [TPM2 Specification Part 3 - Commands v1.59](https://trustedcomputinggroup.org/wp-content/uploads/TCG_TPM2_r1p59_Part3_Commands_pub.pdf)
5. [TPM2 Specification Part 3 - Commands with Code v1.59](https://trustedcomputinggroup.org/wp-content/uploads/TCG_TPM2_r1p59_Part3_Commands_code_pub.pdf)
6. [TPM2 Specification Part 4 - Supporting Routines v1.59](https://trustedcomputinggroup.org/wp-content/uploads/TCG_TPM2_r1p59_Part4_SuppRoutines_pub.pdf)
7. [TPM2 Specification Part 4 - Supporting Routines with Code v1.59](https://trustedcomputinggroup.org/wp-content/uploads/TCG_TPM2_r1p59_Part4_SuppRoutines_code_pub.pdf)
8. [TCG PC Client Platform Firmware Profile Specification v1.05 r23](https://trustedcomputinggroup.org/wp-content/uploads/TCG_PCClient_PFP_r1p05_v23_pub.pdf)
9. [TCG PC Client Platform TPM Profile v1.05 r14](https://trustedcomputinggroup.org/wp-content/uploads/PC-Client-Specific-Platform-TPM-Profile-for-TPM-2p0-v1p05p_r14_pub.pdf)