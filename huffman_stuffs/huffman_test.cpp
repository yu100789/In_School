// HuffmanCode.cpp : Defines the entry point for the console application.
//

// C++ program to encode and decode a string using
// Huffman Coding.
#include <iostream>
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
#include <stdio.h>
#include <string.h>
#include <map>
#define MAX_TREE_HT 256
#define DATAPATTERNNUM 256
#define TLC 2.32

using namespace std;
void Fucking_WriteFile(string fuckingpath, string fuckingstring);

// to map each character its huffman value
map<unsigned char, string> Table;

// to store the frequency of character of the input data
map<unsigned char, int> Freq;

map<char, double> DataBER;

const double lowerBER = 0;
const double eraseBER = 0.5; //000
const double aBER = 0.3;	 //001
const double bBER = 0.4;	 //010
const double cBER = 2.2;	 //011
const double dBER = 2.0;	 //100
const double eBER = 3.0;	 //101
const double fBER = 5.2;	 //110
const double gBER = 8.0;	 //111
const double upperBER = 0;

// A Huffman tree node
struct MinHeapNode
{
	unsigned char data;					// One of the input characters
	int freq;							// Frequency of the character
	MinHeapNode *D, *C, *B, *A, *Erase; // Left and right child

	MinHeapNode(unsigned char data, int freq)
	{
		D = NULL;
		C = NULL;
		B = NULL;
		A = NULL;
		Erase = NULL;
		this->data = data;
		this->freq = freq;
	}
};

// utility function for the priority queue
struct compare
{
	bool operator()(MinHeapNode *l, MinHeapNode *r)
	{
		return (l->freq > r->freq);
	}
};

// utility function to print characters along with
// there huffman value
void printCodes(struct MinHeapNode *root, string str)
{
	if (!root)
		return;
	if (root->data != '$')
	{
		cout << root->data << ":" << str << "\n";
	}
	printCodes(root->D, str + "4");
	printCodes(root->C, str + "3");
	printCodes(root->B, str + "2");
	printCodes(root->A, str + "1");
	printCodes(root->Erase, str + "0");
}

// utility function to store characters along with
// there huffman value in a hash table, here we
// have C++ STL map
void storeCodes(struct MinHeapNode *root, string str)
{
	if (root == NULL)
		return;
	if (root->data != '$')
		Table[root->data] = str;
	storeCodes(root->D, str + "4");
	storeCodes(root->C, str + "3");
	storeCodes(root->B, str + "2");
	storeCodes(root->A, str + "1");
	storeCodes(root->Erase, str + "0");
}

// STL priority queue to store heap tree, with respect
// to their heap root node value
priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare> minHeap;

// function to build the Huffman tree and store it
// in minHeap
void HuffmanCodes(int size)
{
	struct MinHeapNode *D, *C, *B, *A, *Erase, *top;
	for (map<unsigned char, int>::iterator v = Freq.begin(); v != Freq.end(); v++)
		minHeap.push(new MinHeapNode(v->first, v->second));
	while (minHeap.size() != 1)
	{
		if (minHeap.size() >= 5)
		{
			D = minHeap.top();
			minHeap.pop();
			C = minHeap.top();
			minHeap.pop();
			B = minHeap.top();
			minHeap.pop();
			A = minHeap.top();
			minHeap.pop();
			Erase = minHeap.top();
			minHeap.pop();
			top = new MinHeapNode('$', D->freq + C->freq + B->freq + A->freq + Erase->freq);
			top->D = D;
			top->C = C;
			top->B = B;
			top->A = A;
			top->Erase = Erase;
			minHeap.push(top);
		}
		else if (minHeap.size() == 4)
		{
			C = minHeap.top();
			minHeap.pop();
			B = minHeap.top();
			minHeap.pop();
			A = minHeap.top();
			minHeap.pop();
			Erase = minHeap.top();
			minHeap.pop();
			top = new MinHeapNode('$', C->freq + B->freq + A->freq + Erase->freq);
			top->D = NULL;
			top->C = C;
			top->B = B;
			top->A = A;
			top->Erase = Erase;
			minHeap.push(top);
			break;
		}
		else if (minHeap.size() == 3)
		{
			B = minHeap.top();
			minHeap.pop();
			A = minHeap.top();
			minHeap.pop();
			Erase = minHeap.top();
			minHeap.pop();
			top = new MinHeapNode('$', B->freq + A->freq + Erase->freq);
			top->D = NULL;
			top->C = NULL;
			top->B = B;
			top->A = A;
			top->Erase = Erase;
			minHeap.push(top);
			break;
		}
		else if (minHeap.size() == 2)
		{
			A = minHeap.top();
			minHeap.pop();
			Erase = minHeap.top();
			minHeap.pop();
			top = new MinHeapNode('$', A->freq + Erase->freq);
			top->D = NULL;
			top->C = NULL;
			top->B = NULL;
			top->A = A;
			top->Erase = Erase;
			minHeap.push(top);
			break;
		}
	}
	storeCodes(minHeap.top(), "");
	// printCodes(minHeap.top(), "");
}

/*Code改到這裡*/

// utility function to store map each character with its
// frequency in input string
void calcFreq(unsigned char *str, int n)
{
	unsigned char ch;
	double BER = 0;
	// ch = str;
	for (int i = 0; i < n; i++)
	{
		if (0 > str[i] || 0xff < str[i])
		{
			printf("overflow  %x,offset %d\n", str[i], i);
			exit(1);
		}
		ch = str[i];
		// switch (ch)
		// {
		// case '0':
		// 	BER = BER + eraseBER + lowerBER;
		// 	break;
		// case '1':
		// 	BER = BER + aBER + lowerBER;
		// 	break;
		// case '2':
		// 	BER = BER + bBER + lowerBER;
		// 	break;
		// case '3':
		// 	BER = BER + cBER + lowerBER;
		// 	break;
		// case '4':
		// 	BER = BER + dBER + lowerBER;
		// 	break;
		// case '5':
		// 	BER = BER + eBER + lowerBER;
		// 	break;
		// case '6':
		// 	BER = BER + fBER + lowerBER;
		// 	break;
		// case '7':
		// 	BER = BER + gBER + lowerBER;
		// 	break;
		// case '8':
		// 	BER = BER + eraseBER + upperBER;
		// 	break;
		// case '9':
		// 	BER = BER + aBER + upperBER;
		// 	break;
		// case 'A':
		// 	BER = BER + bBER + upperBER;
		// 	break;
		// case 'B':
		// 	BER = BER + cBER + upperBER;
		// 	break;
		// case 'C':
		// 	BER = BER + dBER + upperBER;
		// 	break;
		// case 'D':
		// 	BER = BER + eBER + upperBER;
		// 	break;
		// case 'E':
		// 	BER = BER + fBER + upperBER;
		// 	break;
		// case 'F':
		// 	BER = BER + gBER + upperBER;
		// 	break;
		// }

		Freq[ch]++;
		DataBER[ch] = BER;
		BER = 0;
	}
}

// function iterates through the encoded string s
// if s[i]=='1' then move to node->right
// if s[i]=='0' then move to node->left
// if leaf node append the node->data to our output string
string decode_file(struct MinHeapNode *root, string s)
{
	string ans = "";
	struct MinHeapNode *curr = root;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '4')
			curr = curr->D;
		else if (s[i] == '3')
			curr = curr->C;
		else if (s[i] == '2')
			curr = curr->B;
		else if (s[i] == '1')
			curr = curr->A;
		else if (s[i] == '0')
			curr = curr->Erase;

		// reached leaf node
		if (curr->D == NULL && curr->C == NULL && curr->B == NULL && curr->A == NULL && curr->Erase == NULL)
		{
			ans += curr->data;
			curr = root;
		}
	}
	// cout<<ans<<endl;
	return ans + '\0';
}

// Driver program to test above functions
int main(int argc, char *argv[])
{
	//5LC
	ifstream fin;
	ofstream fout;
	// string fileName = argv[1];
	// int pos = fileName.find_last_of('/');
	// string dir = fileName.substr(0, pos);
	// string name = fileName.substr(pos + 1, fileName.length() - pos);
	// string keyword = "Preprocess";
	// fileName.replace(fileName.find(keyword), keyword.size(), "HuffmanOutput/5LC_Output/Frequency");
	// cout << fileName << endl;
	// dir.replace(dir.find(keyword), keyword.size(), "HuffmanOutput/5LC_Output/Frequency");
	// cout << dir << endl;
	// dir = "mkdir -p " + dir;
	// const int dir_err = system(dir.c_str());
	// if (dir_err == -1)
	// 	cout << "create error!";
	fin.open(argv[1], ios::in);
	// fileName = fileName.substr(0, fileName.find_last_of('.')) + ".txt";
	fout.open("Table");
	//compare
	// ofstream fcompare;
	// fcompare.open("compare.csv", ios::app);
	// pos = name.find_last_of('_');
	// string output_name = name.substr(0, pos);
	// pos = name.find("_output") + 7;
	// string output_number = name.substr(pos, name.find(".") - pos);
	// fcompare << output_name << ',';
	// fcompare << output_number << ',';
	unsigned char *str = NULL;
	if (!fin)
	{
		cout << "檔案無法開啟\n";
		exit(1);
	}
	fin.seekg(0, ios::end);
	long long length = fin.tellg();
	fin.seekg(0, ios::beg);
	// if (posix_memalign((void **)&str, 512, length))
	// {
	// 	perror("posix_memalign failed");
	// 	exit(1);
	// }
	str = (unsigned char *)calloc(length, sizeof(char));
	fin.read((char *)str, length);
	if (fin)
		cout << "all characters read successfully." << endl;
	else
		cout << "error: only " << fin.gcount() << " could be read" << endl;

	string encodedString, decodedString;
	// char buf;
	// unsigned char ch;
	// for (long i = 0;; i++)
	// {
	// 	fin.get(buf);
	// 	if (fin.eof())
	// 	{

	// 		cout << i-1 << endl;

	// 		break;
	// 	}
	// 	ch = buf;
	// 	calcFreq(ch, 0);
	// 	// if (i % 166668 == 0)
	// 	// 	printf("%02x\n", ch);
	// }
	cout << fin.gcount() << endl;
	calcFreq(str, fin.gcount());
	HuffmanCodes(fin.gcount());
	/*計算壓縮率*/
	double CR = 0, BeforeCapacity = 0, AfterCapacity = 0; //Before：壓縮前大小，After：壓縮後大小
	double CellCost = 0;
	double BER = 0, BeforeBER = 0, BufferBER = 0, AfterBER = 0;
	/*各Data Pattern的BER*/

	//	double BERstatus[4] = { eraseBER, aBER, cBER, bBER};

	int yee = 0, k = 0;

	int freq[DATAPATTERNNUM], freqCount = 0;

	cout << "Frequency of each state : " << endl;
	// vector<pair<char, int>> tmp(Freq.cbegin(), Freq.cend());
	// sort(tmp.begin(), tmp.end(), [](pair<string, int> a, pair<string, int> b) {
	// 	return a.second > b.second;
	// });
	for (auto x = Freq.begin(); x != Freq.end(); x++)
	{
		fout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)x->first;
		fout << ':' << dec <<x->second << endl;
		cout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)x->first;
		cout << ':' << dec << x->second << endl;
	}
	// for (const auto &p : tmp)
	// {
	// 	for (char ch : p.first)
	// 	{
	// 		cout << ch;
	// 	}
	// 	cout << " : " << p.second << endl;
	// }
	for (auto x = Freq.begin(); x != Freq.end(); x++)
	{
		freq[freqCount] = x->second;
		BeforeCapacity += x->second;
		freqCount++;
	}
	freqCount = 0;
	// fout << std::endl;
	BeforeCapacity *= 8; //壓縮前大小，一個Data Pattern為8 bits
	unsigned long int total_freq = 0;
	for (auto x = DataBER.begin(); x != DataBER.end(); x++)
	{
		BeforeBER += x->second * freq[k];
		total_freq += freq[k];
		k++;
	}
	double BeforeBER2 = BeforeBER / (double)total_freq;
	// cout << total_freq << endl;
	// fout << "Status mapping : 4 = D, 3 = C, 2 = B, 1 = A, 0 = Erase" << std::endl;
	char a = '$';
	int count = 0;
	int pattern_number = 0;
	//	cout << "Character With there Frequencies:\n";

	total_freq = 0;
	for (map<unsigned char, string>::iterator v = Table.begin(); v != Table.end(); v++)
	{ //計算壓縮後大小
		//		cout << v->first << ' ' << v->second << endl;
		for (int i = 0; i < 10; ++i)
		{
			if (v->second[i] == '\0')
				break;
			else
			{
				a = v->second[i];
				count++;
				if (a == '4')
				{ //D
					BufferBER += dBER;
				}
				else if (a == '3')
				{ //C
					BufferBER += cBER;
				}
				else if (a == '2')
				{ //B
					BufferBER += bBER;
				}
				else if (a == '1')
				{ //A
					BufferBER += aBER;
				}
				else if (a == '0')
				{ //Erase
					BufferBER += eraseBER;
				}
			}
		}
		AfterBER += BufferBER * freq[freqCount];
		total_freq += freq[freqCount];
		BufferBER = 0;
		count *= freq[freqCount++];
		CellCost += count;
		AfterCapacity += TLC * count; //一個狀態為3 bits
		count = 0;
		pattern_number++;
		// for (char ch : v->first)
		// {
		// 	fout << ch;
		// }
		fout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)v->first;
		fout << ':' << v->second << endl;
	}
	cout << total_freq << endl;
	double AfterBER2 = AfterBER / (double)total_freq;
	cout << "pattern number : " << pattern_number << endl;
	cout << "Uncompression Cell number : " << setprecision(0) << fixed << ceil(BeforeCapacity / 3) << endl;
	cout << "Compression Cell number : " << setprecision(0) << fixed << CellCost << endl;
	CR = (CellCost)*300 / BeforeCapacity;
	cout << "Compression Rate(CR) (%) : " << setprecision(3) << fixed << CR << "%" << endl;

	BER = (AfterBER - BeforeBER) * 100 / BeforeBER;
	cout << "Uncompression bit error rate(BER) : " << setprecision(2) << fixed << BeforeBER2 << endl;
	cout << "Compression bit error rate(BER) : " << setprecision(2) << fixed << AfterBER2 << endl;
	// cout << "bit error rate difference(%) : " << setprecision(3) << fixed << BER << "%" << endl;
	// printf("%f %f %ld\n", AfterBER2, BeforeBER2, total_freq);
	// cout << "size of Table : " << std::setprecision(3) << fixed << sizeof(Table)  << endl;
	// fout << "Uncompression bit error rate(BER) : " << std::setprecision(2) << fixed << BeforeBER << endl;
	// fout << "Compression bit error rate(BER) : " << std::setprecision(2) << fixed << AfterBER << endl;
	// fout << "bit error rate difference(%) : " << std::setprecision(3) << fixed << BER << "%" << endl;
	// fcompare << std::setprecision(3) << fixed << BER << "%,";
	// fcompare << std::setprecision(3) << fixed << CR << "%,";
	// fcompare << "5" << endl;
	// fcompare.close();
	fout.close();

	string BestFitString;
	int CClemon = 0;
	for (int i = 0; i < length; i++)
	{
		char ch = '$';
		double Yee = 0;
		ch = str[i];
		BestFitString += Table[ch];
		encodedString += Table[ch];
		CClemon += 2;
		if ((CClemon == 8192) || (i == length - 1))
		{
			Yee = BestFitString.size() / 8192.0;
			// cout << Yee << endl;
			BestFitString = "";
			CClemon = 0;
		}
	}

	//	cout << "\nEncoded Huffman data:\n" << encodedString << endl;

	decodedString = decode_file(minHeap.top(), encodedString);
	//	cout << "\nDecoded Huffman Data:\n" << decodedString << endl;
	// cout << sizeof(map<int, int>) << "\t" << sizeof(Table) << "\t" << sizeof(Freq) << "\t" << sizeof(char) << endl;
	// printCodes(minHeap.top(), "");
	// 2020/10/22
	// fileName = argv[1];
	// keyword = "Preprocess";
	// fileName.replace(fileName.find(keyword), keyword.size(), "HuffmanOutput/5LC_Output/Frequency");

	// // 2020/10/22
	// string fileName2 = fileName;
	// //

	// keyword = ".txt";
	// fileName2.replace(fileName2.find(keyword), keyword.size(), "_compressd_5LC.txt");
	// Fucking_WriteFile(fileName2.c_str(), encodedString);

	// string fileName3 = fileName;
	// fileName3.replace(fileName3.find(keyword), keyword.size(), "_uncompressd_5LC.txt");
	// Fucking_WriteFile(fileName3.c_str(), decodedString);
	free(str);
	return 0;
}

/*
write binary file
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
void Fucking_WriteFile(string fuckingpath, string fuckingstring)
{

	const char *buffer = fuckingstring.c_str();
	// unsigned char *buffer;
	// if (posix_memalign((void **)&buffer, 512, sizeof(fuckingstring)))
	// {
	//     perror("posix_memalign failed");
	//     exit(1);
	// }
	// memset(buffer, 'f', BUF_SIZE);
	cout << fuckingpath << endl;
	int fd = open(fuckingpath.c_str(), O_WRONLY | O_CREAT);
	if (fd < 0)
	{
		perror("open file failed");
		exit(1);
	}
	int ret = write(fd, buffer, fuckingstring.size());
	if (ret < 0)
		perror("write file failed");
	else
		printf("write %d bytes\n", ret);
	close(fd);
}
