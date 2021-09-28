
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "After_ber.h"
using namespace std;
#if ACCESS_TYPE == cWcR
	#define ER 0.5
	#define A 0.3
	#define B 0.4
	#define C 2.2
	#define D 2.0
	#define E 3.0
	#define F 5.2
	#define G 8.0
#elif ACCESS_TYPE == cWhR   // B,C,D,A,E,F,G for hot read data
	#define ER 6.5
	#define A 2.0
	#define B 0.4
	#define C 2.2
	#define D 2.0
	#define E 3.0
	#define F 5.2
	#define G 7.0
#elif ACCESS_TYPE == hWcR
	#define ER ((float)0.5/(float)1)
	#define A ((float)0.3/(float)1)
	#define B ((float)0.4/(float)1)
	#define C ((float)2.2/(float)2) // 0.7
	#define D ((float)2.0/(float)2) // 0.65
	#define E ((float)3.0/(float)2) // 1
	#define F ((float)5.2/(float)2) // 1.7
	#define G ((float)8.0/(float)2) // 2.65
#elif ACCESS_TYPE == hWhR
	#define ER ((float)6.5/(float)1)
	#define A ((float)2.0/(float)1)
	#define B ((float)0.4/(float)10)
	#define C ((float)2.2/(float)10)
	#define D ((float)2.0/(float)10)
	#define E ((float)3.0/(float)10)
	#define F ((float)5.2/(float)10)
	#define G ((float)7.0/(float)10)
#else
	#error "No define access type."
#endif
void BER_after::calculate_state(int LSB[8], int CSB[8], int MSB[8], double STATE[8])
{
	int pos = 0;
	while (pos < 8)
	{
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 0)
			STATE[5]++;
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 1)
			STATE[2]++;
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 2)
		{
			STATE[6]++;
		}
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 3)
			STATE[1]++;
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 4)
			STATE[4]++;
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 5)
			STATE[3]++;
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 6)
		{
			STATE[7]++;
		}
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 7)
			STATE[0]++;
		pos++;
		totalnum_++;
	}
}
void BER_after::generate_codeLength_data(int LSB[], int CSB[], int MSB[], fstream &file, int &readFlag)
{
	int codeLengthPosition = 0, bitPosition = 0;
	int pos = 0, pagePosition = 0;
	char ch;

	for (int i = 0; i < 8; i++)
	{
		file.get(ch);
		if (file.eof())
		{
			readFlag = 0;
			read_cells--;
		}
		if (ch == 0x30)
		{
			LSB[i] = 1;
			CSB[i] = 1;
			MSB[i] = 1;
		}
		if (ch == 0x31)
		{
			LSB[i] = 1;
			CSB[i] = 1;
			MSB[i] = 0;
		}
		if (ch == 0x32)
		{
			LSB[i] = 1;
			CSB[i] = 0;
			MSB[i] = 0;
		}
		if (ch == 0x33)
		{
			LSB[i] = 1;
			CSB[i] = 0;
			MSB[i] = 1;
		}
		if (ch == 0x34)
		{
			LSB[i] = 0;
			CSB[i] = 0;
			MSB[i] = 1;
		}
		if (ch == 0x35)
		{
			LSB[i] = 0;
			CSB[i] = 0;
			MSB[i] = 0;
		}
		if (ch == 0x36)
		{
			LSB[i] = 0;
			CSB[i] = 1;
			MSB[i] = 0;
		}
		if (ch == 0x37)
		{
			LSB[i] = 0;
			CSB[i] = 1;
			MSB[i] = 1;
		}
		read_cells++;
	}
}

double BER_after::after_cells(void)
{
	return totalnum_;
}
void BER_after::print_allcell(int LSB[], int CSB[], int MSB[])
{
	for (int i = 0; i < 8; i++)
		cout << LSB[i];
	cout << endl;
	for (int i = 0; i < 8; i++)
		cout << CSB[i];
	cout << endl;
	for (int i = 0; i < 8; i++)
		cout << MSB[i];
	cout << endl
		 << endl;
}

double BER_after::print_state(double STATE[8])
{
	// printf("BER:%f\n", ((double)STATE[0] * 0.5 + (double)STATE[1] * 0.3 + (double)STATE[2] * 0.4 + (double)STATE[3] * 2.2 + (double)STATE[4] * 2.0 + (double)STATE[5] * 3.0 + (double)STATE[6] * 5.2 + (double)STATE[7] * 8.0));
	// printf("STATE[0]:%f  STATE[1]:%f  STATE[2]:%f  STATE[3]:%f  STATE[4]:%f  STATE[5]:%f  STATE[6]:%f  STATE[7]:%f\n",STATE[0],STATE[1],STATE[2],STATE[3],STATE[4],STATE[5],STATE[6],STATE[7]);
	// printf("%f\n",STATE[0]+STATE[1]+STATE[2]+STATE[3]+STATE[4]+STATE[5]+STATE[6]+STATE[7]);
	// printf("%f\n",totalnum_);
	// printf("BER:%f\n", ((double)STATE[0] * 0.5 + (double)STATE[1] * 0.3 + (double)STATE[2] * 0.4 + (double)STATE[3] * 2.2 + (double)STATE[4] * 2.0 + (double)STATE[5] * 3.0 + (double)STATE[6] * 5.2 + (double)STATE[7] * 8.0) / (double)(totalnum_));
	return ((double)STATE[0] * ER + (double)STATE[1] * A + (double)STATE[2] * B + (double)STATE[3] * C + (double)STATE[4] * D + (double)STATE[5] * E + (double)STATE[6] * F + (double)STATE[7] * G) / (double)(totalnum_);
}
double BER_after::return_state(int state)
{
	return STATE[state];
}
double BER_after::after_ber(const char *filepath)
{
	read_cells = 0;
	totalnum_ = 0;
	srand(time(NULL));
	int LSB[9], CSB[9], MSB[9], readFlag;
	// double STATE[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int pageCodeLengthNumber, select;
	double min = 9999, state_number[4], weight[8] = {3.0, 0.4, 5.2, 0.3, 2.0, 2.2, 8.0, 0.5}; //weight[8] = { 0.9, 0, 8.75, 0.49, 1.72, 0.7, 10.5, 0.6 };
																							  // 	  000, 001, 010,  011,  100,  101, 110 ,111
	double recoveryCL = 0;
	fstream file;
	int fileNumber = 0;

	// file.open("dickens\\(" + to_string(fileNumber) + ")", ios::in | ios::binary);argv[1]

	file.open(filepath, ios::in | ios::binary);
	while (1)
	{
		readFlag = 1;
		generate_codeLength_data(LSB, CSB, MSB, file, readFlag);
		if (readFlag == 0)
			break;
		//	print_allcell(LSB,CSB,MSB);

		calculate_state(LSB, CSB, MSB, STATE);
	}
	file.close();
	fileNumber++;
	// file.open("dickens\\(" + to_string(fileNumber) + ")", ios::in | ios::binary);
	// print_state(STATE);
	// cout << "total cells: " << read_cell << endl
	// 	 << "coding time:" << codingtime / 1000 << "s" << endl
	// 	 << "H-BER rate:" << HBERnum / totalnum_ / 8 * 100 << "%" << endl;
	return print_state(STATE);
}