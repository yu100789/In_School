#ifndef TESTHUFFMAN_H
#define TESTHUFFMAN_H
#include <sys/time.h>
#define KB 1024*1024
#define BUF_SIZE 16 * KB
#define SKIP_THRESHOLD 0
struct encode_info
{
    unsigned int filesize;
    float BER, CR, CellCost, BeforeCapacity, AfterCapacity, BeforeBER, BufferBER, AfterBER, score, encode_time, decode_time;
};
#endif