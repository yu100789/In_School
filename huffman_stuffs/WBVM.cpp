#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

void calculate_weight(int LSB[], int CSB[], int MSB[], double weight[8], double weightcal[], double &min, int &select);
void flip_code(int LSB[], int CSB[], int MSB[], int select);
void calculate_state(int LSB[9], int CSB[9], int MSB[9], double STATE[8]);
void print_state(double STATE[8]);
template <class T>
void initialize_state(T STATE[], int length);
clock_t a, b;
double HBERnum = 0, totalnum = 0;
double codingtime = 0;
void generate_codeLength_data(int LSB[], int CSB[], int MSB[], fstream &file, int &readFlag);
void generate_fromFile(int buffer[], fstream &file, int &readFlag);
double codeLength = 0;
int rawData[3];
string name;
int main(int argc, char *argv[])
{
	name = string(argv[1]);
	srand(time(NULL));
	int LSB[9], CSB[9], MSB[9], readFlag;
	double STATE[8] = {0};
	int pageCodeLengthNumber, select;
	double min = 9999, weightcal[8], weight[8] = {3.0, 0.4, 5.2, 0.3, 2.0, 2.2, 8.0, 0.5}; //weight[8] = { 0.9, 0, 8.75, 0.49, 1.72, 0.7, 10.5, 0.6 };
																						   // 	  000, 001, 010,  011,  100,  101, 110 ,111
	// double min = 9999, weightcal[8], weight[8] = {0.9, 0.6, 8.75, 0.49, 1.72, 0.7, 10.5, 0};

	double recoveryCL = 0;
	fstream file;
	int fileNumber = 0;
	int kernelFileName[8] = {45, 46, 47, 60, 64, 70, 72, 101}, kernelCount = 0;

	file.open(argv[1], ios::in | ios::binary);
	//file.open("C:\\Users\\a8509\\Desktop\\input\\kernel\\(" + to_string(fileNumber) + ").0-" + to_string(kernelFileName[kernelCount]) + "-generic", ios::in | ios::binary);

	while (1)
	{
		codeLength++;
		readFlag = 1;
		generate_codeLength_data(LSB, CSB, MSB, file, readFlag);
		if (readFlag == 0)
			break;

		a = clock();
		calculate_weight(LSB, CSB, MSB, weight, weightcal, min, select);
		flip_code(LSB, CSB, MSB, select);
		b = clock();
		codingtime += b - a;

		if (select != 0)
			recoveryCL++;
		calculate_state(LSB, CSB, MSB, STATE);
	}
	file.close();
	print_state(STATE);
	printf("recovery overhead = %.3f%%", (recoveryCL * 8 * 100) / (codeLength * 8));
	cout << endl
		 << "coding time:" << codingtime / 1000 << "s" << endl
		 << "H-BER rate:" << HBERnum / totalnum * 100 << "%";
	initialize_state(STATE, 8);
	return 0;
}
void calculate_weight(int LSB[], int CSB[], int MSB[], double weight[8], double weightcal[], double &min, int &select)
{
	int pos = 0, temp = 999;
	initialize_state(weightcal, 8);
	while (pos < 8)
	{
		weightcal[0] = weightcal[0] + weight[LSB[pos] + CSB[pos] * 2 + MSB[pos] * 4];
		weightcal[1] = weightcal[1] + weight[(LSB[pos] ^ 1) + CSB[pos] * 2 + MSB[pos] * 4];
		weightcal[2] = weightcal[2] + weight[LSB[pos] + (CSB[pos] ^ 1) * 2 + MSB[pos] * 4];
		weightcal[3] = weightcal[3] + weight[(LSB[pos] ^ 1) + (CSB[pos] ^ 1) * 2 + MSB[pos] * 4];
		weightcal[4] = weightcal[4] + weight[LSB[pos] + CSB[pos] * 2 + (MSB[pos] ^ 1) * 4];
		weightcal[5] = weightcal[5] + weight[(LSB[pos] ^ 1) + CSB[pos] * 2 + (MSB[pos] ^ 1) * 4];
		weightcal[6] = weightcal[6] + weight[LSB[pos] + (CSB[pos] ^ 1) * 2 + (MSB[pos] ^ 1) * 4];
		weightcal[7] = weightcal[7] + weight[(LSB[pos] ^ 1) + (CSB[pos] ^ 1) * 2 + (MSB[pos] ^ 1) * 4];
		weightcal[pos] = weightcal[pos] + weight[pos];
		pos++;
	}
	for (int i = 0; i < 8; i++)
	{
		if (temp > weightcal[i])
		{
			temp = weightcal[i];
			min = i;
		}
	}
	select = min;
	min = 9999;
}

void flip_code(int LSB[], int CSB[], int MSB[], int select)
{
	int i;
	switch (select)
	{
	case 0:
		LSB[8] = 0, CSB[8] = 0, MSB[8] = 0;
		break;
	case 1:
		for (i = 0; i < 8; i++)
		{
			LSB[i] ^= 1;
		}
		LSB[8] = 1, CSB[8] = 0, MSB[8] = 0;
		break;
	case 2:
		for (i = 0; i < 8; i++)
		{
			CSB[i] ^= 1;
		}
		LSB[8] = 0, CSB[8] = 1, MSB[8] = 0;
		break;
	case 3:
		for (i = 0; i < 8; i++)
		{
			LSB[i] ^= 1;
			CSB[i] ^= 1;
		}
		LSB[8] = 1, CSB[8] = 1, MSB[8] = 0;
		break;
	case 4:
		for (i = 0; i < 8; i++)
		{
			MSB[i] ^= 1;
		}
		LSB[8] = 0, CSB[8] = 0, MSB[8] = 1;
		break;
	case 5:
		for (i = 0; i < 8; i++)
		{
			LSB[i] ^= 1;
			MSB[i] ^= 1;
		}
		LSB[8] = 1, CSB[8] = 0, MSB[8] = 1;
		break;
	case 6:
		for (i = 0; i < 8; i++)
		{
			CSB[i] ^= 1;
			MSB[i] ^= 1;
		}
		LSB[8] = 0, CSB[8] = 1, MSB[8] = 1;
		break;
	case 7:
		for (i = 0; i < 8; i++)
		{
			LSB[i] ^= 1;
			CSB[i] ^= 1;
			MSB[i] ^= 1;
		}
		LSB[8] = 1, CSB[8] = 1, MSB[8] = 1;
		break;
	}
}

void calculate_state(int LSB[9], int CSB[9], int MSB[9], double STATE[8])
{
	int pos = 0;
	while (pos < 9)
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

void print_state(double STATE[8])
{
	// printf("0 day     10 days     20 days     30 days     40 days     50 days     60 days\n");
	// printf("%f  ", ((double)STATE[0] * 0 + (double)STATE[1] * 0 + (double)STATE[2] * 0 + (double)STATE[3] * 0 + (double)STATE[4] * 0 + (double)STATE[5] * 0 + (double)STATE[6] * 0 + (double)STATE[7] * 0) / (double)(codeLength * 9));
	// printf("%f    ", ((double)STATE[0] * 0 + (double)STATE[1] * 0 + (double)STATE[2] * 0 + (double)STATE[3] * 0.31 + (double)STATE[4] * 0.55 + (double)STATE[5] * 0.32 + (double)STATE[6] * 5.3 + (double)STATE[7] * 6.2) / (double)(codeLength * 9));
	// printf("%f    ", ((double)STATE[0] * 0 + (double)STATE[1] * 0 + (double)STATE[2] * 0 + (double)STATE[3] * 0.33 + (double)STATE[4] * 0.61 + (double)STATE[5] * 0.38 + (double)STATE[6] * 6.5 + (double)STATE[7] * 7) / (double)(codeLength * 9));
	// printf("%f    ", ((double)STATE[0] * 0 + (double)STATE[1] * 0 + (double)STATE[2] * 0.45 + (double)STATE[3] * 0.5 + (double)STATE[4] * 1.31 + (double)STATE[5] * 0.55 + (double)STATE[6] * 7.3 + (double)STATE[7] * 9.2) / (double)(codeLength * 9));
	// printf("%f    ", ((double)STATE[0] * 0 + (double)STATE[1] * 0 + (double)STATE[2] * 0.5 + (double)STATE[3] * 0.54 + (double)STATE[4] * 1.52 + (double)STATE[5] * 0.69 + (double)STATE[6] * 8.2 + (double)STATE[7] * 10.2) / (double)(codeLength * 9));
	// printf("%f    ", ((double)STATE[0] * 0 + (double)STATE[1] * 0 + (double)STATE[2] * 0.55 + (double)STATE[3] * 0.6 + (double)STATE[4] * 1.53 + (double)STATE[5] * 0.73 + (double)STATE[6] * 8.45 + (double)STATE[7] * 10.4) / (double)(codeLength * 9));
	// printf("%f\n", ((double)STATE[0] * 0 + (double)STATE[1] * 0.49 + (double)STATE[2] * 0.6 + (double)STATE[3] * 0.7 + (double)STATE[4] * 1.72 + (double)STATE[5] * 0.9 + (double)STATE[6] * 8.75 + (double)STATE[7] * 10.5) / (double)(codeLength * 9));
	//36864=wordline's all cell=9*4096  1 page=36864*3
	ofstream f_total_BER;
	int pos = name.find_last_of('/')+1;
	string output_name = name.substr(pos, 50);
	// cout<<output_name<<endl;
	f_total_BER.open("total_BER_WBVM.csv", ios::app);
	// f_total_BER << "File Name" << ',' << "File Number" << ',' << "BER" << ',' << "total num" << endl;
	f_total_BER << output_name << ',';
	// f_total_BER << output_number << ',';
	f_total_BER << setprecision(3) << fixed << ((double)STATE[0] * 0.5 + (double)STATE[1] * 0.3 + (double)STATE[2] * 0.4 + (double)STATE[3] * 2.2 + (double)STATE[4] * 2.0 + (double)STATE[5] * 3.0 + (double)STATE[6] * 5.2 + (double)STATE[7] * 8.0) << ',';
	f_total_BER << setprecision(0) << fixed << totalnum << ',';
	// f_total_BER << setprecision(0) << fixed << codeLength;
	// f_total_BER << total_freq << ',';
	f_total_BER << endl;
	f_total_BER.close();
}

template <class T>
void initialize_state(T STATE[], int length)
{
	for (int i = 0; i < length; i++)
		STATE[i] = 0;
}

void generate_codeLength_data(int LSB[], int CSB[], int MSB[], fstream &file, int &readFlag)
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

void generate_fromFile(int buffer[], fstream &file, int &readFlag)
{
	char ch;
	for (int i = 0; i < sizeof(rawData) / sizeof(rawData[0]); i++)
	{
		file.get(ch);
		if (file.eof())
		{
			readFlag = 0;
		}
		buffer[i] = ch;
	}
}
