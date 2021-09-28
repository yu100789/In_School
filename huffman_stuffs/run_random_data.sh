#!/usr/bin/env bash
g++ random_data.cpp -o random_data
for var in ${1}/*; do
    # Will print */ if no directories are available
    echo "$var"
    # ./random_data $var
done
