
g++ direct_io_read.cpp -o direct_io_read
dir=$(find $1 -type f)
for var in $dir; do
    echo "$var" && ./direct_io_read $var
done
