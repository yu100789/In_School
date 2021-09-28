#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

void calculate_weight(int LSB[], int CSB[], int MSB[], double weight[8], double state_number[], double &min, int &select);
void calculate_state(int LSB[8], int CSB[8], int MSB[8], double STATE[8]);
void print_state(double STATE[8]);
clock_t a, b;
double HBERnum = 0, totalnum = 0;
double codingtime = 0;
void generate_codeLength_data(int LSB[], int CSB[], int MSB[], fstream &file, int &readFlag);
double codeLength = 0;
int rawData[3];
void print_allcell(int LSB[], int CSB[], int MSB[]);
unsigned long read_cell = 0;
int main(int argc, char *argv[])
{
	srand(time(NULL));
	int LSB[9], CSB[9], MSB[9], readFlag;
	double STATE[8] = {0};
	int pageCodeLengthNumber, select;
	double min = 9999, state_number[4], weight[8] = {3.0, 0.4, 5.2, 0.3, 2.0, 2.2, 8.0, 0.5}; //weight[8] = { 0.9, 0, 8.75, 0.49, 1.72, 0.7, 10.5, 0.6 };
																							  // 	  000, 001, 010,  011,  100,  101, 110 ,111
	double recoveryCL = 0;
	fstream file;
	int fileNumber = 0;

	// file.open("dickens\\(" + to_string(fileNumber) + ")", ios::in | ios::binary);argv[1]

	file.open(argv[1], ios::in | ios::binary);
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
		//	print_allcell(LSB,CSB,MSB);

		calculate_state(LSB, CSB, MSB, STATE);
	}
	file.close();
	fileNumber++;
	// file.open("dickens\\(" + to_string(fileNumber) + ")", ios::in | ios::binary);
	print_state(STATE);
	cout << "total cells: " << totalnum << endl
		 << "coding time:" << codingtime / 1000 << "s" << endl
		 << "H-BER rate:" << HBERnum / totalnum / 8 * 100 << "%" << endl;
	return 0;
}
void print_allcell(int LSB[], int CSB[], int MSB[])
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
void calculate_state(int LSB[8], int CSB[8], int MSB[8], double STATE[8])
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

void print_state(double STATE[8])
{
		printf("BER:%f\n", ((double)STATE[0] * 0.5 + (double)STATE[1] * 0.3 + (double)STATE[2] * 0.4 + (double)STATE[3] * 2.2 + (double)STATE[4] * 2.0 + (double)STATE[5] * 3.0 + (double)STATE[6] * 5.2 + (double)STATE[7] * 8.0));
		printf("STATE[0]:%f  STATE[1]:%f  STATE[2]:%f  STATE[3]:%f  STATE[4]:%f  STATE[5]:%f  STATE[6]:%f  STATE[7]:%f\n",STATE[0],STATE[1],STATE[2],STATE[3],STATE[4],STATE[5],STATE[6],STATE[7]);
		printf("%f\n",STATE[0]+STATE[1]+STATE[2]+STATE[3]+STATE[4]+STATE[5]+STATE[6]+STATE[7]);
		printf("%f\n",totalnum);
		printf("BER:%f\n", ((double)STATE[0] * 0.5 + (double)STATE[1] * 0.3 + (double)STATE[2] * 0.4 + (double)STATE[3] * 2.2 + (double)STATE[4] * 2.0 + (double)STATE[5] * 3.0 + (double)STATE[6] * 5.2 + (double)STATE[7] * 8.0) / (double)(totalnum));
}

void generate_codeLength_data(int LSB[], int CSB[], int MSB[], fstream &file, int &readFlag)
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
			read_cell--;
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
		read_cell++;
	}
}
