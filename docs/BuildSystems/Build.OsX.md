# Mac OS X build

- [Mac OS X build](#mac-os-x-build)

---

As with the [Linux](Build.Linux.md) build, use [CMake](Build.CMake.md).

If you used Homebrew to install
[a supported version of OpenSSL](BuildIntro.md#supported-crypto-libraries), you
need to tell CMake where to find it with `OPENSSL_ROOT_DIR`. You'll also need to
tell the C compiler to add the openssl include path.

```sh
export OPENSSL_ROOT_DIR=`brew --prefix openssl`
export CFLAGS="-I$(brew --prefix openssl)/include"
```

OS X compilers treat uninitialized global variables as
[common symbols](https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/MachOTopics/1-Articles/executing_files.html),
which can be eliminated with the `-fno-common` compiler option.

```sh
export CFLAGS="${CFLAGS} -fno-common"
```

Initialize the CMake build normally:

```sh
cmake -S . -B DebugMacOSOpenSSL64
```

Then build:

```sh
cmake --build DebugMacOSOpenSSL64
```

## Common Issues

You might encounter an error where the compiler can't locate the standard
library headers:

```plain
fatal error: 'cstddef' file not found
   52 | #include <cstddef>
      |          ^~~~~~~~~
[  9%] Linking C static library libTpm_PlatformLib.a
1 error generated.
```

This may occur if the OS X developer command-line tools were upgraded from an
older version. One workaround is to perform a clean reinstall of the OS X
developer command line tools.

For more information see: <https://github.com/nodejs/node/issues/55023>
