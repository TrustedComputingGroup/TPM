
# Windows build
- [Windows build](#windows-build)
  - [Introduction](#introduction)
  - [OpenSSL library](#openssl-library)
  - [Wolfcrypt library (wolfSSL)](#wolfcrypt-library-wolfssl)

## Introduction
Windows build is implemented as a Visual Studio 2017 solution.

Before building it, setup a
[supported version](BuildIntro.md#supported-crypto-libraries) of one or more of
the following underlying cryptographic libraries:

## OpenSSL library ###

   1. Create `TPMCmd/lib` folder and place a static OpenSSL library (`libcrypto.lib`)
      built for the `x86` architecture there. For the `x64` architecture use the
      `TPMCmd/lib/x64` folder.

        The static libs can be either static libraries proper, or import libraries
        accompanying the corresponding DLLs. In the latter case you'll need to ensure
        that ther is a matching copy of the OpenSSL DLL in the standard Windows search
        path, so that it is available when you run the simulator executable (e.g. copy
        it into the same folder where `simulator.exe` is located).

        Recommended version of OpenSSL is `1.1.1d` or higher.

   2. Create `TPMCmd/OsslInclude/openssl` folder and copy there the contents of the
      `openssl/include/openssl` folder in the OpenSSL source tree used to build the
      OpenSSL library.

      If you enable SM{2,3,4} algorithms in `TpmProfile.h`, the build may fail because
      of missing `SM{2,3,4}.h` headers. In this case you will need to manually copy
      them over from OpenSSL's `include/crypt` folder.

     ***NOTE: The .h files are bit specific***

Notes:
```
The two OpenSSL resources are:
- The compiled libcrypto.lib and .dll
- Header files from openssl/include/openssl

Location of libcrypto: 64Bit
...TPMCmd\lib\x64\libcrypto.lib
...TPMCmd\lib\x64\libcrypto-1_1-x64.dll

Location of libcrypto: 32Bit
...TPMCmd\lib\x86\libcrypto.lib
...TPMCmd\lib\x86\libcrypto-1_1.dll

OpenSSL Header file location:
...TPMCmd/OsslInclude/x86/openssl/
...TPMCmd/OsslInclude/x64/openssl/
```

   3. Build the solution with either Debug or Release as the active configuration.

## Wolfcrypt library (wolfSSL) ###

WARNING: CURRENTLY UNSUPPORTED

   1. WolfSSL is included as a submodule. Initialize and update the submodule to fetch
      the project and checkout the appropriate commit.

        > git submodule init
        > git submodule update

        The current commit will point the minimum recommended version of wolfSSL.
        Moving to a more recent tag or commit should also be supported but might not
        be tested.

   2. Build the solution with either WolfDebug or WolfRelease as the active
      configuration, either from inside the Visual Studio or with the following
      command line:

        > msbuild TPMCmd\simulator.sln /p:Configuration=WolfDebug
