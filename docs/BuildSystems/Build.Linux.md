- [Linux build](#linux-build)

## Linux build

Follows the common `./bootstrap && ./configure && make` convention.

Note that autotools scripts require the following prerequisite packages:
`autoconf-archive`, `pkg-config`, and sometimes `build-essential` and `automake`.
Their absence is not automatically detected. The build also needs `gcc` and
`libssl-dev` packages.

You will need a [supported version of OpenSSL](BuildIntro.md#supported-crypto-libraries)
installed in order to build the TPM simulator.

Similarly to the Windows build, if you enable SM{2,3,4} algorithms in `TpmProfile.h`,
the build may fail because of missing `SM{2,3,4}.h` headers. In this case you will
need to manually copy them over from OpenSSL's `include/crypt` folder.

## Cross-Compiling for Big-Endian Architectures

NOTE: These intructions are not optimized, they are the notes of one successful
attempt at cross-compiling for ppc64.

REMINDER: Set `BIG_ENDIAN_TPM` to `YES` in `TpmProfile_Common.h`!

REMINDER: NVChip is not compatible between different endiannesses of the TPM.
Delete NVChip if it's left over from a previous test.

You can use `make`'s cross-compilation features to generate a 64-bit PowerPC
binary TPM simulator for testing, even from a little-endian-architecture Linux
environment.

You will need the following packages:
* `gcc-powerpc64-linux-gnu`
* `binutils-powerpc64-linux-gnu`
* `libssl-dev` (ppc64)
  * You can build this from openssl source with `CROSS_COMPILE=powerpc64-linux-gnu- ./Configure linux-ppc64`, then
    running `make` / `make install`
  * If you build and install openssl from source for cross-compilation, it is
    recommended to specify `--prefix=/usr/local/ssl/ppc64/` and
    `--openssldir=/usr/local/ssl/ppc64/` to ensure it does not
    interfere with any other installations of openssl. See
    https://wiki.openssl.org/index.php/Compilation_and_Installation for more
    information about building and installing OpenSSL from source.
* `libc` (ppc64)
  * You can build this from glibc source with:
    ```
    configure CC=powerpc64-linux-gnu-gcc CXX=powerpc64-linux-gnu-g++ --target=powerpc64-linux-gnu --host=powerpc64-linux-gnu
    make -j
    make install
    ```

NOTE: The rest of these instructions assume that the PowerPC64 version of
openssl got installed at /usr/local/ssl/ppc64/.

Use `./bootstrap` normally, as above.

Configure using:

```
PKG_CONFIG_PATH="/usr/local/ssl/ppc64/lib64/pkgconfig" ./configure --host=powerpc64-linux-gnu
```

The bootstrap script doesn't fully support cross compilation yet.
Fix the Makefile by replacing `-lcrypto` in `LIBCRYPTO_LIBS` with
`-L/usr/local/ssl/ppc64/lib64 -lcrypto`. You may also need to add
`-L/path/to/your/installed/ppc-libc` here also.

Compile using:

```
INCLUDES=-I/usr/local/ssl/ppc64/include make
```

Run the binary using `qemu-ppc64-static ./Simulator/src/tpm2-simulator`.

Problems you may run into:

* You may need to add the path to the PowerPC64 OpenSSL lib directory to `LD_LIBRARY_PATH`.
* You may need to add a symlink to your PowerPC64 libc at /lib64/ld64.so.1
