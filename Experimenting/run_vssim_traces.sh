rm -r -f testhuffman
rm -r -f partition_temp/*
g++ testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman -lpthread
dir=$(find /home/ecalab/Desktop/TOSHIBA_HDD/liang-ting-chen/VSSIM/dump_lba -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var
done
