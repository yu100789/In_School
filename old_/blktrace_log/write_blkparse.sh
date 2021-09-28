./clean.sh
sudo blktrace -d /dev/sdb1
sudo blkparse -i sdb1 -o output_original.txt 
sudo blkparse -i sdb1 -o output_remap.txt -F A,"%D %2c %8s %5T.%9t %5p %2a %3d [%C] %k + %n \n"  -F C,"" -F D,"" -F I,"" -F Q,"" -F B,"" -F M,"" -F F,"" -F G,"" -F S,"" -F P,"" -F U,"" -F T,"" -F X,"" 
