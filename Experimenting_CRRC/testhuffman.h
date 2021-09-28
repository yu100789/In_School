#ifndef TESTHUFFMAN_H
#define TESTHUFFMAN_H
#include <sys/time.h>
#define KB 1024
#define MB 1024*KB
#define BUF_SIZE 64*MB
#define SKIP_THRESHOLD 0

#define cWcR 0
#define cWhR 1
#define hWhR 2
#define hWcR 3

#define ACCESS_TYPE 0
#define FILENAME "uncompress"

struct encode_info
{
    unsigned int filesize;
    float BER, CR, CellCost, BeforeCapacity, AfterCapacity, BeforeBER, BufferBER, AfterBER, score, encode_time, decode_time;
    unsigned long Before_ER,Before_A,Before_B,Before_C,Before_D,Before_E,Before_F,Before_G;
    unsigned long After_ER,After_A,After_B,After_C,After_D,After_E,After_F,After_G;
};
#endif