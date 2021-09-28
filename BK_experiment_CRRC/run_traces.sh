rm -r -f *LC.csv
rm -r -f result.csv
# rm -r -f testhuffman
# rm -r -f partition_temp/*

# g++ -g testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman -lpthread
# dir=$(find /home/ecalab/Desktop/TOSHIBA_HDD/liang-ting-chen/huffman_stuffs/SSD_HuffmanDynamicCoding/Preprocess/traces/file/ -type f)
# for var in $dir; do
#     echo "$var" && ./testhuffman $var
# done

rm -r -f partition_temp/5LC/*
g++ -g testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman_5 -lpthread
dir=$(find /home/ecalab/Desktop/TOSHIBA_HDD/liang-ting-chen/VSSIM/TRACES/TPCC/5LC/ -type f)
for var in $dir; do
    echo "$var" && ./testhuffman_5 $var
done

# rm -r -f partition_temp/6LC/*
# g++ -g testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman_6 -lpthread
# dir=$(find /home/ecalab/Desktop/TOSHIBA_HDD/liang-ting-chen/VSSIM/TRACES/TPCC/6LC -type f)
# for var in $dir; do
#     echo "$var" && ./testhuffman_6 $var
# done

# rm -r -f partition_temp/7LC/*
# g++ -g testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman_7 -lpthread
# dir=$(find /home/ecalab/Desktop/TOSHIBA_HDD/liang-ting-chen/VSSIM/TRACES/TPCC/7LC -type f)
# for var in $dir; do
#     echo "$var" && ./testhuffman_7 $var
# done

# rm -r -f partition_temp/8LC/*
# g++ -g testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman_8 -lpthread
# dir=$(find /home/ecalab/Desktop/TOSHIBA_HDD/liang-ting-chen/VSSIM/TRACES/TPCC/8LC -type f)
# for var in $dir; do
#     echo "$var" && ./testhuffman_8 $var
# done