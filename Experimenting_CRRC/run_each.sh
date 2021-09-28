#!/bin/bash
echo "First arg: $1"
echo "Run $1LC "
rm -r -f $1LC.csv
rm -r -f result.csv
rm -r -f partition_temp/$1LC/
g++ -g testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman_$1 -lpthread
dir=$(find /home/ecalab/Desktop/TOSHIBA_HDD/liang-ting-chen/VSSIM/TRACES/INSTALL/5LC/ -type f)
for var in $dir; do
    # if [ "$(tr -d '\0' < $var | wc -c)" -eq 0 ]; then     
        
    #     echo "file : $var , All zeroes." && continue
    # fi 
    echo "$var" && ./testhuffman_$1 $var
done