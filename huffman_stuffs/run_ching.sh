#!/usr/bin/env bash
g++ ching.cpp -o ching
if [ "${1}" == "all" ]; then
    echo "do 5~8LC"
    LC_type="*"
else
    echo "do ${1}LC"
    LC_type=$1LC_Output
fi
for var in SSD_HuffmanDynamicCoding/HuffmanOutput/$LC_type/*/*/*/*/*_compressed*; do
    # Will print */ if no directories are available
    echo "$var"
    ./ching $var
done
