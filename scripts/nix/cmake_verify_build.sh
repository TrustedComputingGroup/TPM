#!/bin/bash

DEBUG_CMAKE=$1

./scripts/nix/cmake_onepass.sh
if [ $? -ne 0 ]; then
    echo "build or install failed"
    exit 1
fi

./scripts/nix/cmake_parts.sh
if [ $? -ne 0 ]; then
    echo "build or install failed"
    exit 1
fi

echo "all builds succeeded"
exit 0
