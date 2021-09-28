
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Raw_ber.h"
using namespace std;

double BER_before::raw_ber(const char *filepath)
{
	rawData[0] = 0;
	rawData[1] = 0;
	rawData[2] = 0;
	codeLength = 0;
	read_count = 0;
	totalnum = 0;
	srand(time(NULL));
	int LSB[9], CSB[9], MSB[9], readFlag;
	double STATE[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int pageCodeLengthNumber, select;
	double min = 9999, state_number[4], weight[8] = {3.0, 0.4, 5.2, 0.3, 2.0, 2.2, 8.0, 0.5}; //weight[8] = { 0.9, 0, 8.75, 0.49, 1.72, 0.7, 10.5, 0.6 };
																							  // 	  000, 001, 010,  011,  100,  101, 110 ,111
	double recoveryCL = 0;
	fstream file;
	int fileNumber = 0;
	int kernelFileName[8] = {45, 46, 47, 60, 64, 70, 72, 101}, kernelCount = 0;
	file.open(filepath, ios::in | ios::binary);
	// cout <<"open file : " << filepath <<endl;
	//file.open("C:\\Users\\a8509\\Desktop\\input\\kernel\\(" + to_string(fileNumber) + ").0-" + to_string(kernelFileName[kernelCount]) + "-generic", ios::in | ios::binary);
	while (1)
	{
		codeLength++;
		readFlag = 1;
		generate_codeLength_data(LSB, CSB, MSB, file, readFlag);
		if (readFlag == 0)
			break;

		a = clock();
		b = clock();
		codingtime += b - a;

		calculate_state(LSB, CSB, MSB, STATE);
	}
	file.close();
	fileNumber++;
	// file.open(argv[1], ios::in | ios::binary);
	//kernelCount++; file.open("C:\\Users\\a8509\\Desktop\\input\\kernel\\(" + to_string(fileNumber) + ").0-" + to_string(kernelFileName[kernelCount]) + "-generic", ios::in | ios::binary);
	// cout << "file size: " << read_count << " Bytes" << endl
	// 	 << "coding time: " << codingtime / 1000 << "s" << endl
	// 	 << "H-BER rate: " << HBERnum / totalnum * 100 << "%" << endl
	// 	 << "total num: " << totalnum << endl;
	return print_state(STATE);
}
double BER_before::raw_cells(void)
{
	return totalnum;
}
void BER_before::calculate_state(int LSB[8], int CSB[8], int MSB[8], double STATE[8])
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
			HBERnum++;
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
			HBERnum++;
		}
		if (LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4 == 7)
			STATE[0]++;
		pos++;
		totalnum++;
	}
}

double BER_before::print_state(double STATE[8])
{
	// printf("BER:%f\n", ((double)STATE[0] * 0.5 + (double)STATE[1] * 0.3 + (double)STATE[2] * 0.4 + (double)STATE[3] * 2.2 + (double)STATE[4] * 2.0 + (double)STATE[5] * 3.0 + (double)STATE[6] * 5.2 + (double)STATE[7] * 8.0) / (double)(totalnum));
	return ((double)STATE[0] * 0.5 + (double)STATE[1] * 0.3 + (double)STATE[2] * 0.4 + (double)STATE[3] * 2.2 + (double)STATE[4] * 2.0 + (double)STATE[5] * 3.0 + (double)STATE[6] * 5.2 + (double)STATE[7] * 8.0) / (double)(totalnum);
}

void BER_before::generate_codeLength_data(int LSB[], int CSB[], int MSB[], fstream &file, int &readFlag)
{
	generate_fromFile(rawData, file, readFlag);
	int codeLengthPosition = 0, bitPosition = 0;
	int pos = 0, pagePosition = 0;
	while (pos < 8 * 3)
	{
		if (pos < 8)
		{
			LSB[pagePosition] = (rawData[codeLengthPosition]) >> (7 - bitPosition) & 1;
			bitPosition++;
			pagePosition++;
			if (bitPosition >= 8)
			{
				bitPosition = 0;
				codeLengthPosition++;
			}
			if (pagePosition >= 8)
				pagePosition = 0;
		}
		if (pos >= 8 && pos < 16)
		{
			CSB[pagePosition] = (rawData[codeLengthPosition]) >> (7 - bitPosition) & 1;
			bitPosition++;
			pagePosition++;
			if (bitPosition >= 8)
			{
				bitPosition = 0;
				codeLengthPosition++;
			}
			if (pagePosition >= 8)
				pagePosition = 0;
		}
		if (pos >= 16 && pos < 24)
		{
			MSB[pagePosition] = (rawData[codeLengthPosition]) >> (7 - bitPosition) & 1;
			bitPosition++;
			pagePosition++;
			if (bitPosition >= 8)
			{
				bitPosition = 0;
				codeLengthPosition++;
			}
			if (pagePosition >= 8)
				pagePosition = 0;
		}
		pos++;
	}
}
void BER_before::generate_fromFile(int buffer[], fstream &file, int &readFlag)
{
	char ch;
	for (int i = 0; i < sizeof(rawData) / sizeof(rawData[0]); i++)
	{
		file.get(ch);
		if (file.eof())
		{
			readFlag = 0;
			read_count--;
		}
		buffer[i] = ch;
		read_count++;
	}
}
