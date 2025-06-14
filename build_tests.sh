#!/bin/bash

mkdir -p build/tests/

find src/ -name "*.c" -not -name "main.c" -exec basename '{}' \; |
    while read -r file; do
        gcc "src/$file" -g3 -c -o "build/${file/.c/.o}"
    done

find tests/ -name "*.c" -not -name "main.c" -exec basename '{}' \; |
    while read -r file; do
        gcc "tests/$file" -g3 -c -o "build/tests/${file/.c/.o}"
    done

gcc tests/main.c build/*.o build/tests/*.o -g3 -o build/test
