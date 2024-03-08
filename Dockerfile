# Build the TPM simulator using autotools from a regular Ubuntu container
FROM ubuntu:22.04
ARG DEBIAN_FRONTEND=noninteractive

# Install some basic tools we're going to need to build the simulator.
# Put this first so local docker will cache this step before copying in the code that
# will get built.  This saves a lot of time on Windows development hosts.
RUN apt-get update && apt-get install -y git autoconf-archive pkg-config build-essential automake gcc libssl-dev wget && rm -rf /var/lib/apt/lists/*

# Install .NET 6
# apt-get supports x64 but not arm yet. So, we use the script.
RUN wget https://dotnet.microsoft.com/download/dotnet/scripts/v1/dotnet-install.sh && chmod +x ./dotnet-install.sh && ./dotnet-install.sh --install-dir /dotnet6
ENV PATH="${PATH}:/dotnet6"

# copy the source tree into a temporary build tree.
WORKDIR /build
COPY TPMCmd ./TPMCmd

# OpenSSL-based simulators
# Use the standard autotools build
# After the build, delete everything but the files needed for coverage analysis

# Ubuntu 22.04 comes with OpenSSL 3
RUN mkdir -p /simulators/openssl3 && \
  cd /simulators/openssl3 && \
  cp -r /build/TPMCmd/* . && \
  ./bootstrap && \
  EXTRA_CFLAGS="--coverage" ./configure && \
  make && \
  find . -type f -not -regex '.*tpm2\-simulator\|.*\.gcno\|.*\.c\|.*\.h' -exec rm {} \; && \
  ln -s ./Simulator/src/tpm2-simulator tpm2-simulator

# Roll back to OpenSSL 1.1 and build again
RUN if [ $(arch) = "x86_64" ]; then \
    LIBSSL_DEB_PATH=http://security.ubuntu.com/ubuntu/pool/main/o/openssl/libssl1.1_1.1.1-1ubuntu2.1~18.04.23_amd64.deb; \
    LIBSSL_DEV_DEB_PATH=http://security.ubuntu.com/ubuntu/pool/main/o/openssl/libssl-dev_1.1.1-1ubuntu2.1~18.04.23_amd64.deb; \
  else \
    LIBSSL_DEB_PATH=http://ports.ubuntu.com/pool/main/o/openssl/libssl1.1_1.1.1-1ubuntu2.1~18.04.23_arm64.deb; \
    LIBSSL_DEV_DEB_PATH=http://ports.ubuntu.com/pool/main/o/openssl/libssl-dev_1.1.1-1ubuntu2.1~18.04.23_arm64.deb; \
  fi && \
  mkdir -p /ossl1 && \
  cd /ossl1 && \
  wget ${LIBSSL_DEB_PATH} -O libssl.deb && \
  dpkg -i libssl.deb && \
  wget ${LIBSSL_DEV_DEB_PATH} -O libssl-dev.deb && \
  dpkg -i libssl-dev.deb

RUN mkdir -p /simulators/openssl1 && \
  cd /simulators/openssl1 && \
  cp -r /build/TPMCmd/* . && \
  ./bootstrap && \
  EXTRA_CFLAGS="--coverage" ./configure && \
  make && \
  find . -type f -not -regex '.*tpm2\-simulator\|.*\.gcno\|.*\.c\|.*\.h' -exec rm {} \; && \
  ln -s ./Simulator/src/tpm2-simulator tpm2-simulator

# Install OpenSSH (useful for GitHub Actions that need to authenticate to
# private Git repositories) and gcovr (for generating code coverage reports)
# Install zip so we can zip up coverage reports.
RUN apt-get update && apt-get install -y openssh-client python3-pip zip && rm -rf /var/lib/apt/lists/*
RUN pip install --no-cache-dir gcovr

# Symlink /tpm2-simulator to the openssl1 one, for convenience of users who just want "a" simulator.
RUN ln -s /simulators/openssl1/Simulator/src/tpm2-simulator /tpm2-simulator
