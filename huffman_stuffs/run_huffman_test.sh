#!/usr/bin/env bash
for var in SSD_HuffmanDynamicCoding/Preprocess/output/file/dickens/*; do
    # Will print */ if no directories are available
    echo "$var"
    ./a.out $var
done
