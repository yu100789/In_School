// C program for Huffman Coding
#include <time.h>
#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <vector>
#include <cmath>
#include <fstream>
#include <deque>
#include <functional>
#include <sys/stat.h>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <pthread.h>
#include <string>
#include "test_5LC.h"
#include "test_6LC.h"
#include "test_7LC.h"
#include "test_8LC.h"
#define ACCUMULATE 70
#define alpha 0.6

struct Thread_Data
{
	char *filepath;
	int Thread_ID;
};
static encode_info *encode_info_8LC = NULL;
static encode_info *encode_info_7LC = NULL;
static encode_info *encode_info_6LC = NULL;
static encode_info *encode_info_5LC = NULL;
unsigned int get_filenumber(const char *path);
void *pthread_entry(void *arg);
std::vector<std::pair<unsigned char, int>> Freq;
int main(int argc, char *argv[])
{
	Thread_Data Multi_Thread_Data[4];
	pthread_t pthread_Thread[4];
	unsigned int num = get_filenumber(argv[1]);
	for (int i = 0; i < 4; i++)
	{
		Multi_Thread_Data[i].filepath = argv[1];
		Multi_Thread_Data[i].Thread_ID = i;
	}
	std::ofstream fout;
	std::string csv_name;
	switch(ACCESS_TYPE){
		case cWcR:
			csv_name = "cWcR_"+(std::string)FILENAME+".csv";
			break;
		case cWhR:
			csv_name = "cWhR_"+(std::string)FILENAME+".csv";
			break;
		case hWcR:
			csv_name = "hWcR_"+(std::string)FILENAME+".csv";
			break;
		case hWhR:
			csv_name = "hWhR_"+(std::string)FILENAME+".csv";
			break;
		default:
			csv_name = "cWcR_"+(std::string)FILENAME+".csv";
			break;
	}
	csv_name = "Cell_"+(std::string)FILENAME+".csv";
	std::fstream file(csv_name, std::ios::in);
	fout.open(csv_name, std::ios::app);
	if (!fout)
	{
		std::cout << "檔案無法開啟\n";
		exit(1);
	}
	if (!file)
		fout << "filename"
			 << ";"
			 << "filesize"
			 << ";"
			 << "encode_time"
			 << ";"
			 << "decode_time"
			 << ";"
			 << "average_symbol"
			 << ";"
			 << "totalpattern"
			 << ";"
			 << "5LC_BER_Reduction"
			 << ";"
			 << "6LC_BER_Reduction"
			 << ";"
			 << "7LC_BER_Reduction"
			 << ";"
			 << "8LC_BER_Reduction"
			 << ";"
			 << "5LC_CompressRatio"
			 << ";"
			 << "6LC_CompressRatio"
			 << ";"
			 << "7LC_CompressRatio"
			 << ";"
			 << "8LC_CompressRatio"
			 << ";"
			 << "Best Score"
			 << ";"
			 << "actual_symbol_5LC"
			 << ";"
			 << "actual_symbol_6LC"
			 << ";"
			 << "actual_symbol_7LC"
			 << ";"
			 << "actual_symbol_8LC"
			 << ";"
			 << "accumulatepattern"
			 << ";"
			 << "5LC_score"
			 << ";"
			 << "6LC_score"
			 << ";"
			 << "7LC_score"
			 << ";"
			 << "8LC_score"
			 << ";"
			 << "Original_BER"
			 << ";"
			 << "5LC_Compressed_BER"
			 << ";"
			 << "6LC_Compressed_BER"
			 << ";"
			 << "7LC_Compressed_BER"
			 << ";"
			 << "8LC_Compressed_BER"
			 << ";"
			 << "Original_Cells"
			 << ";"
			 << "5LC_Compressed_Cells"
			 << ";"
			 << "6LC_Compressed_Cells"
			 << ";"
			 << "7LC_Compressed_Cells"
			 << ";"
			 << "8LC_Compressed_Cells"
			 << ";"
			 << "Before_ER"
			 << ";"
			 << "Before_A"
			 << ";"
			 << "Before_B"
			 << ";"
			 << "Before_C"
			 << ";"
			 << "Before_D"
			 << ";"
			 << "Before_E"
			 << ";"
			 << "Before_F"
			 << ";"
			 << "Before_G"
			 << ";"
			 << "5LC_After_ER"
			 << ";"
			 << "5LC_After_A"
			 << ";"
			 << "5LC_After_B"
			 << ";"
			 << "5LC_After_C"
			 << ";"
			 << "5LC_After_D"
			 << ";"
			 << "5LC_After_E"
			 << ";"
			 << "5LC_After_F"
			 << ";"
			 << "5LC_After_G"
			 << ";"
			 << "6LC_After_ER"
			 << ";"
			 << "6LC_After_A"
			 << ";"
			 << "6LC_After_B"
			 << ";"
			 << "6LC_After_C"
			 << ";"
			 << "6LC_After_D"
			 << ";"
			 << "6LC_After_E"
			 << ";"
			 << "6LC_After_F"
			 << ";"
			 << "6LC_After_G"
			 << ";"
			 << "7LC_After_ER"
			 << ";"
			 << "7LC_After_A"
			 << ";"
			 << "7LC_After_B"
			 << ";"
			 << "7LC_After_C"
			 << ";"
			 << "7LC_After_D"
			 << ";"
			 << "7LC_After_E"
			 << ";"
			 << "7LC_After_F"
			 << ";"
			 << "7LC_After_G"
			 << ";"
			 << "8LC_After_ER"
			 << ";"
			 << "8LC_After_A"
			 << ";"
			 << "8LC_After_B"
			 << ";"
			 << "8LC_After_C"
			 << ";"
			 << "8LC_After_D"
			 << ";"
			 << "8LC_After_E"
			 << ";"
			 << "8LC_After_F"
			 << ";"
			 << "8LC_After_G"
			 << ";"
			 << std::endl;	 
	for (int i = 3; i >= 0; i--)
		pthread_create(&pthread_Thread[i], NULL, pthread_entry, &Multi_Thread_Data[i]);
	for (int i = 3; i >= 0; i--)
		pthread_join(pthread_Thread[i], NULL);
	// encode_info_5LC = score_5LC(argv[1], ACCUMULATE, alpha);
	// encode_info_6LC = score_6LC(argv[1], ACCUMULATE, alpha);
	// encode_info_7LC = score_7LC(argv[1], ACCUMULATE, alpha);
	// encode_info_8LC = score_8LC(argv[1], ACCUMULATE, alpha);
	std::string dir = argv[1];
	int pos = dir.find_last_of('/');
	std::string filename = dir.substr(pos + 1);
	unsigned int *totalpattern = NULL, *accumulatepattern = NULL;
	float *average_symbol = NULL, *actual_symbol_8LC = NULL, *actual_symbol_7LC = NULL, *actual_symbol_6LC = NULL, *actual_symbol_5LC = NULL;
	totalpattern = get_total_pattern();
	accumulatepattern = get_accumulate_pattern();
	average_symbol = _get_average_symbol();
	actual_symbol_8LC = _get_actual_symbol_8LC();
	actual_symbol_7LC = _get_actual_symbol_7LC();
	actual_symbol_6LC = _get_actual_symbol_6LC();
	actual_symbol_5LC = _get_actual_symbol_5LC();
	std::cout.unsetf(std::ios::scientific); // 取消 10 進位顯示
	fout.unsetf(std::ios::scientific); // 取消 10 進位顯示
    std::cout.setf(std::ios::fixed);
	fout.setf(std::ios::fixed); 
	for (int i = 0; i < num; i++)
	{
		if (encode_info_8LC[i].score != 0 && encode_info_7LC[i].score != 0 && encode_info_6LC[i].score != 0 && encode_info_5LC[i].score != 0)
		{
			std::cout << filename << "_" << i << ";"
					  << encode_info_8LC[i].filesize << ";"
					  << encode_info_8LC[i].encode_time << ";"
					  << encode_info_8LC[i].decode_time << ";"
					  << abs(average_symbol[i] / log10(2)) << ";"
					  << totalpattern[i] << ";"
					  << (encode_info_5LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << ";"
					  << (encode_info_6LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << ";"
					  << (encode_info_7LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << ";"
					  << (encode_info_8LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << ";"
					  << encode_info_5LC[i].AfterCapacity / encode_info_5LC[i].BeforeCapacity * 100 << ";"
					  << encode_info_6LC[i].AfterCapacity / encode_info_6LC[i].BeforeCapacity * 100 << ";"
					  << encode_info_7LC[i].AfterCapacity / encode_info_7LC[i].BeforeCapacity * 100 << ";"
					  << encode_info_8LC[i].AfterCapacity / encode_info_8LC[i].BeforeCapacity * 100 << ";"
					  << actual_symbol_5LC[i] * log10(5) / log10(2) << ";"
					  << actual_symbol_6LC[i] * log10(6) / log10(2) << ";"
					  << actual_symbol_7LC[i] * log10(7) / log10(2) << ";"
					  << actual_symbol_8LC[i] * log10(8) / log10(2) << ";"
					  << accumulatepattern[i] << ";"
					  << encode_info_5LC[i].score << ";"
					  << encode_info_6LC[i].score << ";"
					  << encode_info_7LC[i].score << ";"
					  << encode_info_8LC[i].score << ";"
					  << encode_info_8LC[i].BeforeBER << ";"
					  << encode_info_5LC[i].AfterBER << ";"
					  << encode_info_6LC[i].AfterBER << ";"
					  << encode_info_7LC[i].AfterBER << ";"
					  << encode_info_8LC[i].AfterBER << ";"
					  << encode_info_8LC[i].BeforeCapacity << ";"
					  << encode_info_5LC[i].AfterCapacity << ";"
					  << encode_info_6LC[i].AfterCapacity << ";"
					  << encode_info_7LC[i].AfterCapacity << ";"
					  << encode_info_8LC[i].AfterCapacity << ";"
					  << encode_info_5LC[i].Before_ER << ";"
					  << encode_info_5LC[i].Before_A << ";"
					  << encode_info_5LC[i].Before_B << ";"
					  << encode_info_5LC[i].Before_C << ";"
					  << encode_info_5LC[i].Before_D << ";"
					  << encode_info_5LC[i].Before_E << ";"
					  << encode_info_5LC[i].Before_F << ";"
					  << encode_info_5LC[i].Before_G << ";"
					  << encode_info_5LC[i].After_ER << ";"
					  << encode_info_5LC[i].After_A << ";"
					  << encode_info_5LC[i].After_B << ";"
					  << encode_info_5LC[i].After_C << ";"
					  << encode_info_5LC[i].After_D << ";"
					  << encode_info_5LC[i].After_E << ";"
					  << encode_info_5LC[i].After_F << ";"
					  << encode_info_5LC[i].After_G << ";"
					  << encode_info_6LC[i].After_ER << ";"
					  << encode_info_6LC[i].After_A << ";"
					  << encode_info_6LC[i].After_B << ";"
					  << encode_info_6LC[i].After_C << ";"
					  << encode_info_6LC[i].After_D << ";"
					  << encode_info_6LC[i].After_E << ";"
					  << encode_info_6LC[i].After_F << ";"
					  << encode_info_6LC[i].After_G << ";"
					  << encode_info_7LC[i].After_ER << ";"
					  << encode_info_7LC[i].After_A << ";"
					  << encode_info_7LC[i].After_B << ";"
					  << encode_info_7LC[i].After_C << ";"
					  << encode_info_7LC[i].After_D << ";"
					  << encode_info_7LC[i].After_E << ";"
					  << encode_info_7LC[i].After_F << ";"
					  << encode_info_7LC[i].After_G << ";"
					  << encode_info_8LC[i].After_ER << ";"
					  << encode_info_8LC[i].After_A << ";"
					  << encode_info_8LC[i].After_B << ";"
					  << encode_info_8LC[i].After_C << ";"
					  << encode_info_8LC[i].After_D << ";"
					  << encode_info_8LC[i].After_E << ";"
					  << encode_info_8LC[i].After_F << ";"
					  << encode_info_8LC[i].After_G << ";"
					  << std::endl;
			if (accumulatepattern[i] != 0)
			{
				std::string max_lc = "8LC";
				float tmp;
				tmp = encode_info_8LC[i].score;
				for (int k = 0; k < 4; k++)
				{
					if (tmp < encode_info_7LC[i].score)
					{
						tmp = encode_info_7LC[i].score;
						max_lc = "7LC";
					}
					else if (tmp < encode_info_6LC[i].score)
					{
						tmp = encode_info_6LC[i].score;
						max_lc = "6LC";
					}
					else if (tmp < encode_info_5LC[i].score)
					{
						tmp = encode_info_5LC[i].score;
						max_lc = "5LC";
					}
				}
				fout << filename << "_" << i << ";"
					 << encode_info_8LC[i].filesize << ";"
					 << encode_info_8LC[i].encode_time << ";"
					 << encode_info_8LC[i].decode_time << ";"
					 << abs(average_symbol[i] / log10(2)) << ";"
					 << totalpattern[i] << ";"
					 << (encode_info_5LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << ";"
					 << (encode_info_6LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << ";"
					 << (encode_info_7LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << ";"
					 << (encode_info_8LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << ";"
					 << encode_info_5LC[i].AfterCapacity / encode_info_5LC[i].BeforeCapacity * 100 << ";"
					 << encode_info_6LC[i].AfterCapacity / encode_info_6LC[i].BeforeCapacity * 100 << ";"
					 << encode_info_7LC[i].AfterCapacity / encode_info_7LC[i].BeforeCapacity * 100 << ";"
					 << encode_info_8LC[i].AfterCapacity / encode_info_8LC[i].BeforeCapacity * 100 << ";"
					 << max_lc << ";"
					 << actual_symbol_5LC[i] * log10(5) / log10(2) << ";"
					 << actual_symbol_6LC[i] * log10(6) / log10(2) << ";"
					 << actual_symbol_7LC[i] * log10(7) / log10(2) << ";"
					 << actual_symbol_8LC[i] * log10(8) / log10(2) << ";"
					 << accumulatepattern[i] << ";"
					 << encode_info_5LC[i].score << ";"
					 << encode_info_6LC[i].score << ";"
					 << encode_info_7LC[i].score << ";"
					 << encode_info_8LC[i].score << ";"
					 << encode_info_8LC[i].BeforeBER << ";"
					 << encode_info_5LC[i].AfterBER << ";"
					 << encode_info_6LC[i].AfterBER << ";"
					 << encode_info_7LC[i].AfterBER << ";"
					 << encode_info_8LC[i].AfterBER << ";"
					 << encode_info_8LC[i].BeforeCapacity << ";"
					 << encode_info_5LC[i].AfterCapacity << ";"
					 << encode_info_6LC[i].AfterCapacity << ";"
					 << encode_info_7LC[i].AfterCapacity << ";"
					 << encode_info_8LC[i].AfterCapacity << ";"
					 << encode_info_5LC[i].Before_ER << ";"
					  << encode_info_5LC[i].Before_A << ";"
					  << encode_info_5LC[i].Before_B << ";"
					  << encode_info_5LC[i].Before_C << ";"
					  << encode_info_5LC[i].Before_D << ";"
					  << encode_info_5LC[i].Before_E << ";"
					  << encode_info_5LC[i].Before_F << ";"
					  << encode_info_5LC[i].Before_G << ";"
					  << encode_info_5LC[i].After_ER << ";"
					  << encode_info_5LC[i].After_A << ";"
					  << encode_info_5LC[i].After_B << ";"
					  << encode_info_5LC[i].After_C << ";"
					  << encode_info_5LC[i].After_D << ";"
					  << encode_info_5LC[i].After_E << ";"
					  << encode_info_5LC[i].After_F << ";"
					  << encode_info_5LC[i].After_G << ";"
					  << encode_info_6LC[i].After_ER << ";"
					  << encode_info_6LC[i].After_A << ";"
					  << encode_info_6LC[i].After_B << ";"
					  << encode_info_6LC[i].After_C << ";"
					  << encode_info_6LC[i].After_D << ";"
					  << encode_info_6LC[i].After_E << ";"
					  << encode_info_6LC[i].After_F << ";"
					  << encode_info_6LC[i].After_G << ";"
					  << encode_info_7LC[i].After_ER << ";"
					  << encode_info_7LC[i].After_A << ";"
					  << encode_info_7LC[i].After_B << ";"
					  << encode_info_7LC[i].After_C << ";"
					  << encode_info_7LC[i].After_D << ";"
					  << encode_info_7LC[i].After_E << ";"
					  << encode_info_7LC[i].After_F << ";"
					  << encode_info_7LC[i].After_G << ";"
					  << encode_info_8LC[i].After_ER << ";"
					  << encode_info_8LC[i].After_A << ";"
					  << encode_info_8LC[i].After_B << ";"
					  << encode_info_8LC[i].After_C << ";"
					  << encode_info_8LC[i].After_D << ";"
					  << encode_info_8LC[i].After_E << ";"
					  << encode_info_8LC[i].After_F << ";"
					  << encode_info_8LC[i].After_G << ";"
					 << std::endl;
			}
		}
	}
	free(actual_symbol_5LC);
	free(actual_symbol_6LC);
	free(actual_symbol_7LC);
	free(actual_symbol_8LC);
	free(average_symbol);
	free(totalpattern);
	free(accumulatepattern);
	free(encode_info_5LC);
	free(encode_info_6LC);
	free(encode_info_7LC);
	free(encode_info_8LC);
	fout.close();
	return 0;
}
void *pthread_entry(void *arg)
{
	Thread_Data *Thread = (struct Thread_Data *)arg;
	// std::cout << Thread->filepath << std::endl;
	// std::cout << Thread->Thread_ID << std::endl;
	switch ((Thread->Thread_ID) + 5)
	{
	case 5:
		encode_info_5LC = score_5LC(Thread->filepath, ACCUMULATE, alpha);
		break;
	case 6:
		encode_info_6LC = score_6LC(Thread->filepath, ACCUMULATE, alpha);
		break;
	case 7:
		encode_info_7LC = score_7LC(Thread->filepath, ACCUMULATE, alpha);
		break;
	case 8:
		encode_info_8LC = score_8LC(Thread->filepath, ACCUMULATE, alpha);
		break;
	}
}
unsigned int get_filenumber(const char *path)
{
	std::ifstream fin;
	fin.open(path, std::ios::in | std::ios::binary);
	if (!fin)
	{
		std::cout << "檔案無法開啟\n";
		exit(1);
	}
	fin.seekg(0, std::ios::end);
	std::streampos sp = fin.tellg();
	if (sp < SKIP_THRESHOLD)
	{
		std::cout << "file too small,close this proccess" << std::endl;
		fin.close();
		exit(0);
	}
	unsigned int num = 0;
	num = sp / (BUF_SIZE);
	if (sp % (BUF_SIZE) > 0)
		num++;
	fin.close();
	return num;
}