#!/bin/sh

# Check if clang-format version is 19.1.4
REQUIRED_VERSION="19.1.4"
CF="clang-format-19"
CURRENT_VERSION=$(${CF} --version | awk '{print $4}')
if [ "$CURRENT_VERSION" != "$REQUIRED_VERSION" ]; then
  echo "Error: clang-format-19 version $REQUIRED_VERSION is required. Current version is $CURRENT_VERSION."
  exit 1
fi

find TPMCmd/tpm -iname *.h -o -iname *.c | xargs $CF --verbose -i -style=file
find TPMCmd/Platform -iname *.h -o -iname *.c | xargs $CF --verbose -i -style=file
find TPMCmd/Simulator -iname *.h -o -iname *.c | xargs $CF --verbose -i -style=file
