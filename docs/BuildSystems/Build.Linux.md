
# Contents

* [Contents](#contents)
  * [Linux build](#linux-build)
  * [OpenSSL](#openssl)

## Linux build

Use [CMake](Build.CMake.md).

Short Version:

```bash
sudo apt-get update
sudo apt-get install pkg-config build-essential ninja-build cmake libssl-dev clang
```

## OpenSSL

You will need a [supported version of OpenSSL](BuildIntro.md#supported-crypto-libraries)
installed in order to build the TPM simulator.

Similarly to the Windows build, if you enable SM{2,3,4} algorithms in `TpmProfile_Common.h`,
the build may fail because of missing `SM{2,3,4}.h` headers. In this case you will
need to manually copy them over from OpenSSL's `include/crypt` folder.

## Cross-Compiling for Big-Endian Architectures

NOTE: These intructions are not optimized, they are the notes of one successful
attempt at cross-compiling for ppc64.

REMINDER: Set `BIG_ENDIAN_TPM` to `YES` in `TpmProfile_Common.h`!

REMINDER: NVChip is not compatible between different endiannesses of the TPM.
Delete NVChip if it's left over from a previous test.

You can use cmake's cross-compilation features to generate a 64-bit PowerPC
binary TPM simulator for testing, even from a little-endian-architecture Linux
environment.

You will need the following packages:

* `gcc-powerpc64-linux-gnu`
* `binutils-powerpc64-linux-gnu`
* `libssl-dev` (ppc64)
  * You can build this from openssl source with `CROSS_COMPILE=powerpc64-linux-gnu- ./Configure linux-ppc64`, then
    * running `make` / `make install`
  * If you build and install openssl from source for cross-compilation, it is
    * recommended to specify `--prefix=/usr/local/ssl/ppc64/` and
    * `--openssldir=/usr/local/ssl/ppc64/` to ensure it does not
    * interfere with any other installations of openssl. See [OpenSSL Installation]
      for more
    * information about building and installing OpenSSL from source.
* `libc` (ppc64)
  * You can build this from glibc source with:

    ```bash
    configure CC=powerpc64-linux-gnu-gcc CXX=powerpc64-linux-gnu-g++ --target=powerpc64-linux-gnu --host=powerpc64-linux-gnu
    make -j
    make install
    ```

NOTE: The rest of these instructions assume that the PowerPC64 version of
openssl got installed at /usr/local/ssl/ppc64/.

Run the binary using `qemu-ppc64-static ./Simulator/src/tpm2-simulator`.
Problems you may run into:

* You may need to add the path to the PowerPC64 OpenSSL lib directory to `LD_LIBRARY_PATH`.
* You may need to add a symlink to your PowerPC64 libc at /lib64/ld64.so.1

[OpenSSL Installation]: https://wiki.openssl.org/index.php/Compilation_and_Installation
