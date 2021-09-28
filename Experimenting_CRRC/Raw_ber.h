#ifndef RAW_BER_H
#define RAW_BER_H
#include <fstream>
#include "testhuffman.h"
class BER_before
{
private:
    clock_t a, b;
    double HBERnum = 0, totalnum = 0;
    double codingtime = 0;
    double codeLength = 0;
    double STATE[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int rawData[3];
    unsigned long read_count = 0;
    void calculate_state(int LSB[8], int CSB[8], int MSB[8], double STATE[8]);
    void generate_codeLength_data(int LSB[], int CSB[], int MSB[], std::fstream &file, int &readFlag);
    void generate_fromFile(int buffer[], std::fstream &file, int &readFlag);

public:
    double print_state(double STATE[8]);
    double raw_ber(const char *filepath);
    double raw_cells(void);
    double return_state(int state);
};
#endif