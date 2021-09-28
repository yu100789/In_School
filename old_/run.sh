clear
rm -r -f tmp
rm -r -f dump
rm -r -f direct_io_read
rm -r -f write_direct_io
g++ write_direct_io.cpp -o write_direct_io
./write_direct_io
g++ direct_io_read.cpp -o direct_io_read
./direct_io_read