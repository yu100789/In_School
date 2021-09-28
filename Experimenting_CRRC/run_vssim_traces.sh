# rm -r -f testhuffman
# rm -r -f partition_temp/*
g++ -g testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman_3 -lpthread
dir=$(find /home/ecalab/Desktop/TOSHIBA_HDD/liang-ting-chen/VSSIM/TRACES/KERNEL -type f)
for var in $dir; do
    echo "$var" && ./testhuffman_3 $var
done
