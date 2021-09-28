# rm -r -f *LC.csv
# rm -r -f result.csv
rm -r -f testhuffman
# rm -r -f partition_temp/*
g++ testhuffman.cpp test_8LC.cpp test_7LC.cpp test_6LC.cpp test_5LC.cpp Raw_ber.cpp After_ber.cpp -o testhuffman -lpthread
dir=$(find /bin -type f)
max_proccess=100
echo "$(ps -C testhuffman | wc -l) > $max_proccess"
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /boot -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /cdrom -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /dev -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /etc -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /lib -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /lib64 -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /lost+found -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /media -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /mnt -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /opt -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /proc -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /root -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /run -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /sbin -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /snap -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /srv -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /sys -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /tmp -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /usr -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
dir=$(find /var -type f)
for var in $dir; do
    echo "$var" && ./testhuffman $var 
done
for var in ~/Desktop/TOSHIBA_HDD/liang-ting-chen/huffman_stuffs/SSD_HuffmanDynamicCoding/Preprocess/traces/*; do
    echo "$var" && ./testhuffman $var
done
