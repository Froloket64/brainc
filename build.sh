#!/bin/bash

mkdir -p build

find src/ -name "*.c" -exec basename '{}' \; |
    while read -r file; do
        gcc "src/$file" -c -o "build/${file/.c/.o}"
    done

gcc build/*.o -o build/brainc
