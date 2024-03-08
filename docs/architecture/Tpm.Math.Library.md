<!-- Copyright Microsoft Corporation. All Rights Reserved
     Licensed subject to: https://github.com/microsoft/ms-tpm-20-ref/blob/main/LICENSE -->

# TPM Reference Code - Math Library Design

- [TPM Reference Code - Math Library Design](#tpm-reference-code---math-library-design)
  - [See Also](#see-also)

## See Also

- [Introduction](Introduction.md)
- [Library Architecture](Library.Architecture.md)
- [Tpm BigNum Library](Tpm.BigNum.Library.md)
- [Tpm Crypto Libraries](Tpm.Crypto.Libraries.md)
- [Tpm Math Library](Tpm.Math.Library.md)
- [Tpm Platform API](Tpm.Platform.Api.md)

## Overview

The goal of this design is that the "Core" library has no internal knowledge or
dependency on the formats used by a specific math/encryption library.  The Core
library depends on allocation macros defined by the provider to create
stack-allocated buffers and then refers to values only by opaque pointer.  The
internal format of the allocated space is entirely encapsulated within the math
library.

## Math Library Code Layers

To keep the boundaries in the code as clear as possible, the `Core` library
specifies function naming prefixes.  These take the place of namespaces in other
languages.

CAUTION: Currently, this naming convention is currently applied ONLY TO
FUNCTIONS, and ONLY in the area of the Math Library.  In particular, different
conventions apply to data types, file names, and so forth due to pre-existing
naming in the reference code.

At the highest level there are three _layers_, which are provided by separate
static libraries:

| Layer Prefix | Provider | Meaning                                                                                                                         |
| ------------ | -------- | ------------------------------------------------------------------------------------------------------------------------------- |
| Crypt        | Core     | Highest layer Cryptographic operations, operating on TPM Specific types.  E.g. CryptEccEncrypt.                                 |
| Tpm          | Core     | Crypto implementation provided by the Core TPM Library, typically operating on raw numbers.  e.g. TpmEcc_ValidateSignatureEcdsa |
| Ext          | varies   | Math and ECC implementation provided by the External Math library(-ies)                                                         |

Within the `Tpm` and `Ext` layer there are multiple _sublayers_.  The `Crypt` layer has no sublayer, but calls the `Tpm` or `Ext` layers directly.

| SubLayer | Meaning                                             |
| :------- | :-------------------------------------------------- |
| Math     | Generic math functionality used by both ECC and RSA |
| Ecc      | ECC Related functionality (points, curves, etc.)    |
| Rsa      | RSA related functionality (primes, exponents, etc.) |

As a shorthand, the rest of this file will frequently refer to the `Ext` library
as `MATH_LIB` to distinguish it from the `SYM_LIB`  or `HASH_LIB` libraries.  In
practice the implementer may decide to provide the `Ext` asymmetric layers
(Math, Ecc, Rsa) as multiple static libraries, but this documentation refers to
them being in a single library for convenience and does not always differentiate
the sub-layers.

## Math Library Interface

The primary interface file for the Math Library is contained in
[MathLibraryInterface.h](../TPMCmd/tpm/../../../TPMCmd/tpm/cryptolibs/common/include/MathLibraryInterface.h)
This document is accessory documentation to that file, which should be
considered the relevant source of truth.

## Function Naming

The name of each Function will consist of several parts:

- a prefix+sublayer combination, followed by
- an underscore, followed by
- function name

### Function Examples

| Function Name                 | Layer                                              |
| :---------------------------- | :------------------------------------------------- |
| `ExtMath_SizeInBits`          | External Asymmetric Support Library, Math Sublayer |
| `TpmMath_IsProbablyPrime`     | Core TPM library, Math Sublayer                    |

`Tpm*` functions expect to be implemented in terms of `Ext*` functions.

# Data Types

The Core library contains a number of TPM specific types defined in Part 2 of
the TPM specification, and used in Part 3, such as:

- `TPM2B_DIGEST`
- `TPM2B_PUBLIC_KEY_RSA`
- `TPMU_ASYM_SCHEME`
- etc.

A design goal for the interface between the `Core` library and `MATH_LIB`
library is to minimize the number of TPM specific types that are used.

The concrete data types used between the `Core` and the `Ext` layers
fall into these categories:

- standard types from `<stdint.h>`
  - e.g. uint16_t
- some simple typedefs and enum types from the TPM specification to avoid
  duplication.
  - e.g. NUMBYTES, TPM_ECC_CURVE
- typedefs that are provided by the external library, _**according to the rules
  below**_.

The `Core` library uses `const` correctly in the Math Interface and may use any
`<stdint.h>` type.  At the time of writing this, the TPM library uses these
types to interface to the `MATH_LIB` Library:

| NewType                  | Pointer Type | Layer Defined By | Notes                                                |
| ------------------------ | :----------: | :--------------: | ---------------------------------------------------- |
| Crypt_Int                |      Y       |     ExtMath      |                                                      |
| Crypt_Point              |      Y       |      ExtEcc      |                                                      |
| Crypt_EccCurve           |      Y       |      ExtEcc      |                                                      |
| TPM_ECC_CURVE_ID         |      N       |       TPM        | Enum value                                           |
| NUMBYTES (uint16_t)      |      N       |       TPM        | Size from TPM2B (cannot be larger)                   |
| crypt_uword_t            |      N       |       TPM        | either uint32_t or uint64_t based on compile options |
| crypt_word_t             |      N       |       TPM        | either int32_t or int64_t based on compile options   |
| uint32_t                 |      N       |      stdint      |                                                      |
| uint8_t                  |      N       |      stdint      |                                                      |

## Reserved for future use

| NewType                  | Pointer Type | Layer Defined By | Notes                                                |
| ------------------------ | :----------: | :--------------: | ---------------------------------------------------- |
| Crypt_Divisor            |      Y       |     ExtMath      | May be typedef'd to Crypt_Int                        |
| Crypt_ModElement         |      Y       |     ExtMath      | May be typedef'd to Crypt_Int                        |
| Crypt_Modulus            |      Y       |     ExtMath      | May be typedef'd to Crypt_Int                        |

## Requirements for Externally Defined Types

As indicated in the table above, the `ExtMath` library is responsible for
defining the main mathematical types. However, the TPM Library does place some
requirements on these new types:

1. Non-pointer types may be used as value types or pointers to input/return
   values (`NUMBYTES` or `NUMBYTES*`).
2. `ExtMath` Pointer types are always used as pointers (`Crypt_Int*`), or
   pointers-to-const (`const Crypt_Int*`).
3. `ExtMath` types must be **STACK ALLOCATED** and must provide [STACK
   ALLOCATION macros](#stack-allocationtype-macros).
4. `ExtMath` types must be able to be initialized from raw byte arrays in
   Most-Significant-Byte first format.
5. Initialization functions must be able to accept single word values (like 1 or
   0) also.

**WARNING**: The TPM Library uses `longjmp` in FAILURE MODE and multiple early
return paths and does not guarantee that dynamically allocated memory is cleaned
up.  The Core library will not provide any notice when a library defined type
(such as `Crypt_Int*`) goes out of scope.  This is the reason for stack
allocation described below.

## Stack Allocation/Type Macros

The `ExtMath` library must provide type allocation macros that allow the size of
various number objects to be specified in the call to the macro so the compiler
can expand the definition to the correct size for each usage, such as the
following:

```lang-c
#define CRYPT_INT_BUF(bitsize) struct {    \
    crypt_uword_t       allocated;          \
    crypt_uword_t       size;               \
    crypt_uword_t       d[(bitsize+7)/8];   \
    }
```

Note that this **MUST** be a macro in order to be both stack allocated and
dynamically sized.

The following Allocation/Type Macros must be defined by the `ExtMath` and
`ExtEcc` layers:

```lang-c
// max_size_in_bits is guaranteed to be constexpr (compile-time constant) and
// indicates the largest value this object will hold during it's lifetime - it may
// contain a smaller value.
//
// NOTE: The `ExtMath` library must be able to perform the correct **COMPILE TIME**
// conversion from bits based on their internal representation.
//
// The typename parameters can be used to form typenames for structures using
// preprocessor token pasting.  This can be helpful if the compiler does not support
// anonymous structure types, and/or for getting better compiler error messages.

#define CRYPT_INT_BUF(typename, max_size_in_bits)
#define CRYPT_POINT_BUF(typename, max_size_in_bits)
#define CRYPT_CURVE_BUF(typename, max_size_in_bits)

//Reserved for future use:
#define CRYPT_DIVISOR_BUF(typename, max_size_in_bits)
#define CRYPT_MODELEMENT_BUF(typename, max_size_in_bits)
#define CRYPT_MODULUS_BUF(typename, max_size_in_bits)
```

**IMPORTANT**: Type macros SHOULD leave the object uninitialized or with a value
of zero.  Value initialization will occur separately.

## Type Initialization

In the context of the Math library, object initialization happens in two steps.
First, the `Initialization` step refers precisely to informing the
stack-allocated `ExtMath` object how large a value it may contain. All
CRYPT_*_BUF types will be initalized according to the following pattern.
Initialization in this context does NOT! convey any meaning of _value_ merely
_size_.

Second, after `Initialization` a value will be set by the `Core` library using
either the relevant `FromBytes` API or will call with a fixed `crypt_uword_t`
value such as zero or 1.

```lang-c
    // a fixed size, stack-based object holding the actual data
    CRYPT_INT_BUF(size_bits)  someVarData;

    // a pointer to the buffer above used for all actual operations.
    // This function is the opportunity for the ExtMath library to
    // record the actual size inside the object, so it can correctly
    // handle the object's size when it receives the pointer later.
    Crypt_Int* pSomeVar = ExtMath_Initialize_Int(&someVarData, bits);
```

Notice that the `ExtMath_Initialize_Int` function takes a pointer to the result
of the `CRYPT_INT_BUF` macro and the Core library will pass the same allocation
size into the Initialize function.  The `ExtMath` library is expected to
initialize the object in the space allocated by the `_BUF` macro and to store its
own size.

This implies that the size in the macro declaration must be tracked inside the
`Crypt_Int` because the `Core` library assumes that `ExtMath` will handle
overflow checking and be able to report the max(allocated) size later. Also,
observe the code expects the initialization to return the &someVarData pointer
to the `Crypt_Int*` type. Finally, notice how this implies that `Crypt_Int*` is
actually polymorphic on the size.

Other `_BUF` types are initialized similarly.

**WARNING**: It is NOT possible to get the correct in-memory size from
`sizeof(*pSomeVar)` for some `Crypt_Int* pSomeVar`.  Generally it will be
understated.

## `ExtMath` Number Typedefs

As mentioned above, number types are polymorphic with respect to size.
Therefore, there must be a single typedef for each number type which allows the
`Core` library to operate on the variable sized structure.

The `Core` library defines all the relevant typedefs in
`MathLibraryInterfaceTypes.h` using the relevant `_BUF` macros.

## Binary Number Formats

The `Core` Library can be compiled as 32-bit or 64-bit code.  That compile
decision determines the size of the smallest word size the TPM Code supports.
This is referred to as TPM_RADIX_BITS and is either 32 or 64.  The TPM Library
defines special type aliases `tpm_crypt_uword_t` and `tpm_crypt_word_t` which
are the size of TPM_RADIX_BITS.  Operations on "word" values (such as
`ExtMath_AddWord`) refer to these types.

The TPM communicates to the `ExtMath` Library using the native TPM format,
Most-Significant-Byte (MSB) first.  The Core always refers to buffer sizes as a
number of bytes and value sizes as a number of bits.  Bits within bytes have the
usual memory numbering and significance; bit 0 is the least-significant (value
1) bit in the least-significant byte, bit 7 is the most significant (value 128)
bit in the least significant byte, etc.

Note that not all cryptographic numbers are convenient sizes - some are
downright annoying.  Take ECC 521-bit curves for example, it is not an even
number of TPM_RADIX_BITS words or even bytes.  When given in a byte buffer,
values are always given (and expected) in a buffer that is an integral number of
bytes, but the values may be a different number of bits.

For example, the NIST P-521 curve uses the following generator coordinates:

```lang-text
   bits:     5        4        4        4        3        3        3        2        2        2        1        1        1
   bits:     1        8        4        1        8        5        2        8        5        2        9        6        2        9        6        3
   bits:     2        0        8        6        4        2        0        8        6        4        2        0        8        6        4        2      0
P521 generator coordinates
Gx(MSB):  c6 858e06b7 0404e9cd 9e3ecb66 2395b442 9c648139 053fb521 f828af60 6b4d3dba a14b5e77 efe75928 fe1dc127 a2ffa8de 3348b3c1 856a429b f97e7e31 c2e5bd66
Gy(MSB): 118 39296a78 9a3bc004 5c8a5fb4 2c7d1bd9 98f54449 579b4468 17afbd17 273e662c 97ee7299 5ef42640 c550b901 3fad0761 353c7086 a272c240 88be9476 9fd16650
```

The highest-order bytes here are left-padded with zero bits to the number of
bytes required to store a 521 bit number. (i.e. `Gx` is padded to the same size
as `Gy`.

Thus, viewed as a **byte** array in LSB order, Gy becomes:

```lang-text
bits           32       64       96      128                                 256                                 384                                 512  528
Gy (LE): 5066d19f 7694be88 40c272a2 86703c35 6107ad3f 01b950c5 4026f45e 9972ee97 2c663e27 17bdaf17 68449b57 4944f598 d91b7d2c b45f8a5c 04c03b9a 786a2939 1801
```

Hence, a 521-bit value is rounded up to 66 bytes (528 bits).

The actual storage size for a `Crypt_Int` is left to the `ExtMath` library, but
may be rounded up further based on an internal word size used by the library.

## Side Channel Considerations

It is expected that `ExtMath` functions operate in a side-channel safe way. That
is to say the performance of these functions may be `O(1)` or
`O(f(allocated_bits))` for some function `f`, but MUST NOT be
`O(size_of_value)`, unless noted on the function description (in the headers).


# Function List

The official list of required functions and parameters are contained in the
`MathLibraryInterface.h` header file.
