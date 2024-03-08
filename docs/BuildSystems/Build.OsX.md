- [Mac OS X build](#mac-os-x-build)

## Mac OS X build

As with the [Linux](Build.Linux.md) build, use `./bootstrap`, `./configure`, and `make`.
If you used Homebrew to install [a supported version of OpenSSL](BuildIntro.md#supported-crypto-libraries),
you may need to include its path in `PKG_CONFIG_PATH`.
OS X compilers treat uninitialized global variables as
[common symbols](https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/MachOTopics/1-Articles/executing_files.html),
which can be eliminated with the `-fno-common` compiler option.
Future updates to the autotools configurations may automate one or both of these steps.

```
./bootstrap
PKG_CONFIG_PATH="/usr/local/opt/openssl/lib/pkgconfig" EXTRA_CFLAGS=-fno-common ./configure
make
```
