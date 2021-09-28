#!/usr/bin/env bash
if [ "${1}" == "all" ]; then
    echo "do 5~8LC"
    LC_type="*"
else
    echo "do ${1}LC"
    LC_type=$1LC_Output
fi
# for var in SSD_HuffmanDynamicCoding/Preprocess/output/file/*/*; do
#     # Will print */ if no directories are available
#     echo "$var"
#     if [ "${1}" == "all" ]; then
#         for var2 in SSD_HuffmanDynamicCoding/Huffman/*LC_Huffman; do
#             ./$var2 $var
#         done
#     else
#         ./SSD_HuffmanDynamicCoding/Huffman/$1LC_Huffman $var
#     fi
# done
g++ WBVM_compressed.cpp -o WBVM_compressed
for var in SSD_HuffmanDynamicCoding/HuffmanOutput/$LC_type/*/*/*/*/*_compressed*; do
    # Will print */ if no directories are available
    echo "$var"
    ./WBVM_compressed $var
done
g++ WBVM.cpp -o WBVM
for var in SSD_HuffmanDynamicCoding/Preprocess/traces/file/*; do
    # Will print */ if no directories are available
    echo "$var"
    ./WBVM $var
done
