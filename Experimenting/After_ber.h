#ifndef AFTER_BER_H
#define AFTER_BER_H
#include <fstream>
class BER_after
{
private:
	double totalnum_;
	unsigned long read_cells;
	void calculate_state(int LSB[8], int CSB[8], int MSB[8], double STATE[8]);
	void generate_codeLength_data(int LSB[], int CSB[], int MSB[], std::fstream &file, int &readFlag);

public:
	double after_ber(const char *filepath);
	double after_cells(void);
	void print_allcell(int LSB[], int CSB[], int MSB[]);
	double print_state(double STATE[8]);
};
#endif