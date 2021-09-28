#include "After_ber.h"
#include "Raw_ber.h"
#include "testhuffman.h"
#include <algorithm>
#include <cmath>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <queue>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <vector>
#include <string.h>
#define LC_type "7LC"
#define DATAPATTERNNUM 256
// #define MB 1024 * 1024
using namespace std;

// A Huffman tree node
struct minHeap_7LCNode
{
    unsigned char data;                         // One of the input characters
    int Freq_7LC;                               // Freq_7LCuency of the character
    minHeap_7LCNode *G, *F, *E, *D, *C, *B, *A; // Left and right child

    minHeap_7LCNode(unsigned char data, int Freq_7LC)
    {
        G = NULL;
        F = NULL;
        E = NULL;
        D = NULL;
        C = NULL;
        B = NULL;
        A = NULL;
        this->data = data;
        this->Freq_7LC = Freq_7LC;
    }
};

struct compare
{
    bool operator()(minHeap_7LCNode *l, minHeap_7LCNode *r)
    {
        return (l->Freq_7LC > r->Freq_7LC);
    }
};
// STL priority queue to store heap tree, with respect
// to their heap root node value
priority_queue<minHeap_7LCNode *, vector<minHeap_7LCNode *>, compare> minHeap_7LC;
// utility function for the priority queue
static int calcCollision(int accumulate, float score, int n, int size, string filename, float BER, float CR);
static string write_encode(unsigned char *str, int length);
static void write_char_buf(string path, unsigned char *str, int size);
static void write_String(string path, string str);
static void write_Table_7LC(string path);
static void printCodes(struct minHeap_7LCNode *root, string str);
static void storeCodes(struct minHeap_7LCNode *root, string str);
static void HuffmanCodes(int size);
static void calcFreq_7LC(unsigned char *str, int n);
static string decode_file(struct minHeap_7LCNode *root, string s);
static int isLeaf(struct minHeap_7LCNode *root);
map<unsigned char, string> Table_7LC;
map<unsigned char, int> Freq_7LC;
map<unsigned char, double> DataBER_7LC;
const double lowerBER = 0.16;
const double eraseBER = 0.5; //000
const double aBER = 0.3;     //001
const double bBER = 0.4;     //010
const double cBER = 2.2;     //011
const double dBER = 2.0;     //100
const double eBER = 3.0;     //101
const double fBER = 5.2;     //110
const double gBER = 8.0;     //111
const double upperBER = 1.5;
float *actual_symbol_7LC = NULL;
encode_info *score_7LC(const char *path, float accumulate, float alpha)
{
    ifstream fin;
    ofstream fout;
    unsigned char **str = NULL;
    unsigned int num = 0;
    string encodedString, decodedString;
    string dir = path;
    int pos = dir.find_last_of('/');
    string filename = dir.substr(pos + 1);
    fin.open(path, ios::in | ios::binary);
    if (!fin)
    {
        cout << "檔案無法開啟\n";
        exit(1);
    }
    fin.seekg(0, ios::end);
    streampos sp = fin.tellg();
    if (sp < 0)
        exit(1);
    cout << endl;
    cout << "sp : " << sp << endl;
    num = sp / (BUF_SIZE);
    if (sp % (BUF_SIZE) > 0)
        num++;
    cout << " num : " << num << endl;
    mkdir("partition_temp", 0777);
    string dir_name = "partition_temp/" + string(LC_type);
    mkdir(dir_name.c_str(), 0777);
    dir_name += "/" + filename;
    mkdir(dir_name.c_str(), 0777);
    str = (unsigned char **)malloc(num * sizeof(unsigned char *));
    actual_symbol_7LC = (float *)malloc(num * sizeof(float));
    encode_info *encode_info_7LC = (encode_info *)malloc((num+1) * sizeof(encode_info));
    for (int i = 0; i < num; i++)
    {
        encode_info_7LC[i].filesize = 0;
        encode_info_7LC[i].CR = 0, encode_info_7LC[i].BeforeCapacity = 0, encode_info_7LC[i].AfterCapacity = 0;
        encode_info_7LC[i].CellCost = 0;
        encode_info_7LC[i].BER = 0, encode_info_7LC[i].BeforeBER = 0, encode_info_7LC[i].BufferBER = 0, encode_info_7LC[i].AfterBER = 0;
        encode_info_7LC[i].score = 0;
        encode_info_7LC[i].encode_time = 0;
        fin.seekg(i * BUF_SIZE, ios::beg);
        cout << "current pos : " << fin.tellg()<< endl;
        cout << " number of buffer : " << i<< endl;
        str[i] = (unsigned char *)malloc((BUF_SIZE+1) * sizeof(unsigned char));
        // memset(str[i], '0', (BUF_SIZE+1) * sizeof(unsigned char));
        fin.read((char *)str[i], BUF_SIZE);
        cout << " read : " << fin.gcount() << " Bytes" << endl;
        if (fin.gcount() < SKIP_THRESHOLD)
        {
            cout << "file too small, skip" << endl;
            free(str[i]);
            break;
        }
        calcFreq_7LC(str[i], fin.gcount());
        struct timeval start, end, diff;
        // 開始計算時間
        gettimeofday(&start, NULL);
        // 主要計算
        HuffmanCodes(fin.gcount());
        // 結束計算時間
        gettimeofday(&end, NULL);
        // 計算實際花費時間
        timersub(&end, &start, &diff);
        encode_info_7LC[i].encode_time = diff.tv_sec + (float)diff.tv_usec / 1000000.0;
        string name = "partition_temp/" + string(LC_type) + "/" + filename + "/" + filename + "_" + to_string(i);
        cout << name << endl;
        write_char_buf(name, str[i], fin.gcount());
        encodedString = write_encode(str[i], fin.gcount());
        string name_compressed = name + "_compressed";
        write_String(name_compressed, encodedString);
        // decodedString = decode_file(minHeap_7LC.top(), encodedString);
        string name_decode = name + "_uncompress";
        write_String(name_decode, decodedString);
        string name_Table_7LC = name + "_Table_7LC";
        write_Table_7LC(name_Table_7LC);
        BER_after *AfterBER_OB = new BER_after();
        BER_before *BeforeBER_OB = new BER_before();
        encode_info_7LC[i].filesize = fin.gcount();
        encode_info_7LC[i].BeforeBER = BeforeBER_OB->raw_ber(name.c_str());
        encode_info_7LC[i].BeforeCapacity = BeforeBER_OB->raw_cells();
        encode_info_7LC[i].AfterBER = AfterBER_OB->after_ber(name_compressed.c_str());
        encode_info_7LC[i].AfterCapacity = AfterBER_OB->after_cells();
        encode_info_7LC[i].Before_ER = BeforeBER_OB->return_state(0);
        encode_info_7LC[i].Before_A = BeforeBER_OB->return_state(1);
        encode_info_7LC[i].Before_B = BeforeBER_OB->return_state(2);
        encode_info_7LC[i].Before_C = BeforeBER_OB->return_state(3);
        encode_info_7LC[i].Before_D = BeforeBER_OB->return_state(4);
        encode_info_7LC[i].Before_E = BeforeBER_OB->return_state(5);
        encode_info_7LC[i].Before_F = BeforeBER_OB->return_state(6);
        encode_info_7LC[i].Before_G = BeforeBER_OB->return_state(7);
        encode_info_7LC[i].After_ER = AfterBER_OB->return_state(0);
        encode_info_7LC[i].After_A = AfterBER_OB->return_state(1);
        encode_info_7LC[i].After_B = AfterBER_OB->return_state(2);
        encode_info_7LC[i].After_C = AfterBER_OB->return_state(3);
        encode_info_7LC[i].After_D = AfterBER_OB->return_state(4);
        encode_info_7LC[i].After_E = AfterBER_OB->return_state(5);
        encode_info_7LC[i].After_F = AfterBER_OB->return_state(6);
        encode_info_7LC[i].After_G = AfterBER_OB->return_state(7);
        cout << "Before BER : " << encode_info_7LC[i].BeforeBER << endl;
        cout << "After BER : " << encode_info_7LC[i].AfterBER << endl;
        cout << "BER reduction: " << (encode_info_7LC[i].AfterBER - encode_info_7LC[i].BeforeBER) / encode_info_7LC[i].BeforeBER << endl;
        cout << "Compress ratio: " << encode_info_7LC[i].AfterCapacity / encode_info_7LC[i].BeforeCapacity * 100 << endl;
        cout << "Score: " << alpha * (1 - (encode_info_7LC[i].AfterCapacity / encode_info_7LC[i].BeforeCapacity)) - (1 - alpha) * ((encode_info_7LC[i].AfterBER - encode_info_7LC[i].BeforeBER) / encode_info_7LC[i].BeforeBER) << endl;
        calcCollision(accumulate, alpha * (1 - (encode_info_7LC[i].AfterCapacity / encode_info_7LC[i].BeforeCapacity)) - (1 - alpha) * ((encode_info_7LC[i].AfterBER - encode_info_7LC[i].BeforeBER) / encode_info_7LC[i].BeforeBER), i, fin.gcount(), filename, (encode_info_7LC[i].AfterBER - encode_info_7LC[i].BeforeBER) / encode_info_7LC[i].BeforeBER, encode_info_7LC[i].AfterCapacity / encode_info_7LC[i].BeforeCapacity * 100);
        encode_info_7LC[i].score = alpha * (1 - (encode_info_7LC[i].AfterCapacity / encode_info_7LC[i].BeforeCapacity)) - (1 - alpha) * ((encode_info_7LC[i].AfterBER - encode_info_7LC[i].BeforeBER) / encode_info_7LC[i].BeforeBER);
        free(str[i]);
        delete(AfterBER_OB);
        delete(BeforeBER_OB);
    }
    free(str);
    fin.close();
    return encode_info_7LC;
}
int calcCollision(int accumulate, float score, int n, int size, string filename, float BER, float CR)
{
    ofstream fout;
    // fcompare.open("compare.csv", ios::app);
    fout.open("7LC.csv", ios::app);
    if (!fout)
    {
        cout << "檔案無法開啟\n";
        exit(1);
    }
    unsigned int pattern = 0, total_pattern = 0, total_Freq_7LC = 0;
    vector<pair<unsigned char, int>> tmp(Freq_7LC.cbegin(), Freq_7LC.cend());
    sort(tmp.begin(), tmp.end(), [](pair<unsigned char, int> a, pair<unsigned char, int> b)
         { return a.second > b.second; });
    float probability;
    unsigned int buf = 0;
    actual_symbol_7LC[n] = 0;
    for (map<unsigned char, int>::iterator v = Freq_7LC.begin(); v != Freq_7LC.end(); v++)
    {
        if (v->second != 0)
        {
            probability = (float)v->second / (float)size;
            buf = Table_7LC[v->first].length();
            actual_symbol_7LC[n] += (float)buf * probability;
        }
    }
    for (auto x = tmp.begin(); x != tmp.end(); x++)
    {
        if (x->second != 0)
            total_pattern++;
    }
    for (auto x = tmp.begin(); x != tmp.end(); x++)
    {
        if (float(total_Freq_7LC) / float(size) * 100 > accumulate)
        {
            cout << total_Freq_7LC << " : " << float(total_Freq_7LC) / float(size) * 100 << " : " << accumulate << " : " << total_pattern << " : " << pattern << endl;
            fout << filename << "_" << n << "," << total_Freq_7LC << "," << float(total_Freq_7LC) / float(size) * 100 << "," << accumulate << "," << total_pattern << "," << pattern << "," << BER << "," << CR << "," << score << endl;
            return pattern;
        }
        total_Freq_7LC += x->second;
        pattern++;
    }
    fout.close();
    return 0;
}
float *_get_actual_symbol_7LC(void)
{
    return actual_symbol_7LC;
}
void write_char_buf(string path, unsigned char *str, int size)
{
    ofstream fout;
    fout.open(path, ios::out | ios::binary);
    if (!fout)
    {
        cout << "檔案無法開啟\n";
        exit(1);
    }
    fout.write((char *)str, size);
    fout.close();
}
void write_String(string path, string str)
{
    ofstream fout;
    fout.open(path, ios::out | ios::binary);
    if (!fout)
    {
        cout << "檔案無法開啟\n";
        exit(1);
    }
    fout << str;
    fout.close();
}
string write_encode(unsigned char *str, int length)
{
    string encodedString = "";
    char ch;
    for (int i = 0; i < length; i++)
    {
        ch = str[i];
        encodedString += Table_7LC[ch];
    }
    return encodedString;
}
void write_Table_7LC(string path)
{
    ofstream fout;
    int num_pattern = 0;
    fout.open(path, ios::out | ios::binary);
    if (!fout)
    {
        cout << "檔案無法開啟\n";
        exit(1);
    }
    fout << "Freq_7LCuency of each state : " << endl;
    vector<pair<unsigned char, int>> tmp(Freq_7LC.cbegin(), Freq_7LC.cend());
    sort(tmp.begin(), tmp.end(), [](pair<unsigned char, int> a, pair<unsigned char, int> b)
         { return a.second > b.second; });
    for (auto x = tmp.begin(); x != tmp.end(); x++)
    {
        if (x->second != 0)
        {
            fout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)x->first;
            fout << ':' << dec << x->second << endl;
            // cout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)x->first;
            // cout << ':' << dec << x->second << endl;
        }
    }
    fout << "Coding Table_7LC : " << endl;
    for (auto x = tmp.begin(); x != tmp.end(); x++)
    {
        if (x->second != 0)
        {
            fout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)x->first;
            fout << ':' << Table_7LC[x->first] << endl;
            // cout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)x->first;
            // cout << ':' << Table_7LC[x->first] << endl;
            // cout << dec;
            num_pattern++;
        }
    }
    fout << "Pattern Numbers  : " << dec << num_pattern << endl;
    fout.close();
}
// Utility function to check if this node is leaf
int isLeaf(struct minHeap_7LCNode *root)
{
    return !(root->G) && !(root->F) && !(root->E) && !(root->D) && !(root->C) && !(root->B) && !(root->A);
}
void printCodes(struct minHeap_7LCNode *root, string str)
{
    if (!root)
        return;
    if (isLeaf(root))
        cout << root->data << ": " << str << "\n";
    printCodes(root->G, str + "7");
    printCodes(root->F, str + "6");
    printCodes(root->E, str + "5");
    printCodes(root->D, str + "4");
    printCodes(root->C, str + "3");
    printCodes(root->B, str + "2");
    printCodes(root->A, str + "1");
}

// utility function to store characters along with
// there huffman value in a hash Table_7LC, here we
// have C++ STL map
void storeCodes(struct minHeap_7LCNode *root, string str)
{
    if (root == NULL)
        return;
    if (isLeaf(root))
    {
        if (str == "")
            str += "0";
        Table_7LC[root->data] = str;
    }
    storeCodes(root->G, str + "7");
    storeCodes(root->F, str + "6");
    storeCodes(root->E, str + "5");
    storeCodes(root->D, str + "4");
    storeCodes(root->C, str + "3");
    storeCodes(root->B, str + "2");
    storeCodes(root->A, str + "1");
}
// function to build the Huffman tree and store it
// in minHeap_7LC
void HuffmanCodes(int size)
{
    // priority_queue<minHeap_7LCNode *, vector<minHeap_7LCNode *>, compare> minHeap_7LC;
    struct minHeap_7LCNode *G = NULL, *F = NULL, *E = NULL, *D = NULL, *C = NULL, *B = NULL, *A = NULL, *top = NULL;
    while (!minHeap_7LC.empty())
    {
        // top = minHeap_7LC.top();
        // delete(top);
        minHeap_7LC.pop();
    }
    unsigned int total_pattern = 0;
    for (map<unsigned char, int>::iterator v = Freq_7LC.begin(); v != Freq_7LC.end(); v++)
    {
        if (v->second != 0)
        {
            total_pattern++;
            minHeap_7LC.push(new minHeap_7LCNode(v->first, v->second));
        }
    }
    if ((7 % 6) == (total_pattern % 6))
    {
        cout << (7 % 6) << " = " << total_pattern << " mod 6 " << endl;
        G = minHeap_7LC.top();
        minHeap_7LC.pop();
        F = minHeap_7LC.top();
        minHeap_7LC.pop();
        E = minHeap_7LC.top();
        minHeap_7LC.pop();
        A = minHeap_7LC.top();
        minHeap_7LC.pop();
        D = minHeap_7LC.top();
        minHeap_7LC.pop();
        C = minHeap_7LC.top();
        minHeap_7LC.pop();
        B = minHeap_7LC.top();
        minHeap_7LC.pop();
        top = new minHeap_7LCNode(0, G->Freq_7LC + F->Freq_7LC + E->Freq_7LC + A->Freq_7LC + D->Freq_7LC + C->Freq_7LC + B->Freq_7LC );
        top->G = G;
        top->F = F;
        top->E = E;
        top->A = A;
        top->D = D;
        top->C = C;
        top->B = B;
        minHeap_7LC.push(top);
    }
    else if ((6 % 6) == (total_pattern % 6))
    {
        cout << (6 % 6) << " = " << total_pattern << " mod 6 " << endl;
        F = minHeap_7LC.top();
        minHeap_7LC.pop();
        E = minHeap_7LC.top();
        minHeap_7LC.pop();
        A = minHeap_7LC.top();
        minHeap_7LC.pop();
        D = minHeap_7LC.top();
        minHeap_7LC.pop();
        C = minHeap_7LC.top();
        minHeap_7LC.pop();
        B = minHeap_7LC.top();
        minHeap_7LC.pop();
        top = new minHeap_7LCNode(0, F->Freq_7LC + E->Freq_7LC + A->Freq_7LC + D->Freq_7LC + C->Freq_7LC + B->Freq_7LC );
        top->G = NULL;
        top->F = F;
        top->E = E;
        top->A = A;
        top->D = D;
        top->C = C;
        top->B = B;
        minHeap_7LC.push(top);
    }
    else if ((5 % 6) == (total_pattern % 6))
    {
        cout << (5 % 6) << " = " << total_pattern << " mod 6 " << endl;
        E = minHeap_7LC.top();
        minHeap_7LC.pop();
        A = minHeap_7LC.top();
        minHeap_7LC.pop();
        D = minHeap_7LC.top();
        minHeap_7LC.pop();
        C = minHeap_7LC.top();
        minHeap_7LC.pop();
        B = minHeap_7LC.top();
        minHeap_7LC.pop();
        top = new minHeap_7LCNode(0, E->Freq_7LC + A->Freq_7LC +D->Freq_7LC + C->Freq_7LC + B->Freq_7LC );
        top->G = NULL;
        top->F = NULL;
        top->E = E;
        top->A = A;
        top->D = D;
        top->C = C;
        top->B = B;
        minHeap_7LC.push(top);
    }
    else if ((4 % 6) == (total_pattern % 6))
    {
        cout << (4 % 6) << " = " << total_pattern << " mod 6 " << endl;
        A = minHeap_7LC.top();
        minHeap_7LC.pop();
        D = minHeap_7LC.top();
        minHeap_7LC.pop();
        C = minHeap_7LC.top();
        minHeap_7LC.pop();
        B = minHeap_7LC.top();
        minHeap_7LC.pop();
        top = new minHeap_7LCNode(0,A->Freq_7LC+D->Freq_7LC + C->Freq_7LC + B->Freq_7LC );
        top->G = NULL;
        top->F = NULL;
        top->E = NULL;
        top->A = A;
        top->D = D;
        top->C = C;
        top->B = B;
        minHeap_7LC.push(top);
    }
    else if ((3 % 6) == (total_pattern % 6))
    {
        cout << (3 % 6) << " = " << total_pattern << " mod 6 " << endl;
        D = minHeap_7LC.top();
        minHeap_7LC.pop();
        C = minHeap_7LC.top();
        minHeap_7LC.pop();
        B = minHeap_7LC.top();
        minHeap_7LC.pop();
        top = new minHeap_7LCNode(0, D->Freq_7LC + C->Freq_7LC + B->Freq_7LC);
        top->G = NULL;
        top->F = NULL;
        top->E = NULL;
        top->A = NULL;
        top->D = D;
        top->C = C;
        top->B = B;
        minHeap_7LC.push(top);
    }
    else if ((2 % 6) == (total_pattern % 6))
    {
        cout << (2 % 6) << " = " << total_pattern << " mod 6 " << endl;
        C = minHeap_7LC.top();
        minHeap_7LC.pop();
        B = minHeap_7LC.top();
        minHeap_7LC.pop();
        top = new minHeap_7LCNode(0, C->Freq_7LC + B->Freq_7LC);
        top->G = NULL;
        top->F = NULL;
        top->E = NULL;
        top->D = NULL;
        top->A = NULL;
        top->C = C;
        top->B = B;
        minHeap_7LC.push(top);
    }
    while (minHeap_7LC.size() != 1)
    {
        if (minHeap_7LC.size() >= 7)
        {
            G = minHeap_7LC.top();
            minHeap_7LC.pop();
            F = minHeap_7LC.top();
            minHeap_7LC.pop();
            E = minHeap_7LC.top();
            minHeap_7LC.pop();
            A = minHeap_7LC.top();
            minHeap_7LC.pop();
            D = minHeap_7LC.top();
            minHeap_7LC.pop();
            C = minHeap_7LC.top();
            minHeap_7LC.pop();
            B = minHeap_7LC.top();
            minHeap_7LC.pop();
            top = new minHeap_7LCNode(0, G->Freq_7LC + F->Freq_7LC + E->Freq_7LC + A->Freq_7LC+ D->Freq_7LC + C->Freq_7LC + B->Freq_7LC );
            top->G = G;
            top->F = F;
            top->E = E;
            top->A = A;
            top->D = D;
            top->C = C;
            top->B = B;
            minHeap_7LC.push(top);
        }
        else if (minHeap_7LC.size() == 6)
        {
            F = minHeap_7LC.top();
            minHeap_7LC.pop();
            E = minHeap_7LC.top();
            minHeap_7LC.pop();
            A = minHeap_7LC.top();
            minHeap_7LC.pop();
            D = minHeap_7LC.top();
            minHeap_7LC.pop();
            C = minHeap_7LC.top();
            minHeap_7LC.pop();
            B = minHeap_7LC.top();
            minHeap_7LC.pop();
            top = new minHeap_7LCNode(0, F->Freq_7LC + E->Freq_7LC + A->Freq_7LC+ D->Freq_7LC + C->Freq_7LC + B->Freq_7LC );
            top->G = NULL;
            top->F = F;
            top->E = E;
            top->A = A;
            top->D = D;
            top->C = C;
            top->B = B;
            minHeap_7LC.push(top);
            break;
        }
        else if (minHeap_7LC.size() == 5)
        {
            E = minHeap_7LC.top();
            minHeap_7LC.pop();
            A = minHeap_7LC.top();
            minHeap_7LC.pop();
            D = minHeap_7LC.top();
            minHeap_7LC.pop();
            C = minHeap_7LC.top();
            minHeap_7LC.pop();
            B = minHeap_7LC.top();
            minHeap_7LC.pop();
            top = new minHeap_7LCNode(0, E->Freq_7LC + A->Freq_7LC +D->Freq_7LC + C->Freq_7LC + B->Freq_7LC );
            top->G = NULL;
            top->F = NULL;
            top->E = E;
            top->A = A;
            top->D = D;
            top->C = C;
            top->B = B;
            minHeap_7LC.push(top);
            break;
        }
        else if (minHeap_7LC.size() == 4)
        {
            A = minHeap_7LC.top();
            minHeap_7LC.pop();
            D = minHeap_7LC.top();
            minHeap_7LC.pop();
            C = minHeap_7LC.top();
            minHeap_7LC.pop();
            B = minHeap_7LC.top();
            minHeap_7LC.pop();
            top = new minHeap_7LCNode(0, A->Freq_7LC+D->Freq_7LC + C->Freq_7LC + B->Freq_7LC );
            top->G = NULL;
            top->F = NULL;
            top->E = NULL;
            top->A = A;
            top->D = D;
            top->C = C;
            top->B = B;
            minHeap_7LC.push(top);
            break;
        }
        else if (minHeap_7LC.size() == 3)
        {
            D = minHeap_7LC.top();
            minHeap_7LC.pop();
            C = minHeap_7LC.top();
            minHeap_7LC.pop();
            B = minHeap_7LC.top();
            minHeap_7LC.pop();
            top = new minHeap_7LCNode(0, D->Freq_7LC + C->Freq_7LC + B->Freq_7LC);
            top->G = NULL;
            top->F = NULL;
            top->E = NULL;
            top->A = NULL;
            top->D = D;
            top->C = C;
            top->B = B;
            minHeap_7LC.push(top);
            break;
        }
        else if (minHeap_7LC.size() == 2)
        {
            C = minHeap_7LC.top();
            minHeap_7LC.pop();
            B = minHeap_7LC.top();
            minHeap_7LC.pop();
            top = new minHeap_7LCNode(0, C->Freq_7LC+B->Freq_7LC);
            top->G = NULL;
            top->F = NULL;
            top->E = NULL;
            top->D = NULL;
            top->A = NULL;
            top->C = C;
            top->B = B;
            minHeap_7LC.push(top);
            break;
        }
    }
    storeCodes(minHeap_7LC.top(), "");
}

/*Code改到這裡*/

// utility function to store map each character with its
// Freq_7LCuency in input string
void calcFreq_7LC(unsigned char *str, int n)
{
    // printf("test1\n");
    for (int i = 0; i < 256; i++)
    {
        // printf("test%d\n",i);
        DataBER_7LC[i] = 0;
        Freq_7LC[i] = 0;
        Table_7LC[i] = "";
    }
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
        Freq_7LC[ch]++;
        DataBER_7LC[ch] = BER;
        BER = 0;
    }
}

// function iterates through the encoded string s
// if s[i]=='1' then move to node->right
// if s[i]=='0' then move to node->left
// if leaf node append the node->data to our output string
string decode_file(struct minHeap_7LCNode *root, string s)
{
    string ans = "";
    struct minHeap_7LCNode *curr = root;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '7')
            curr = curr->G;
        else if (s[i] == '6')
            curr = curr->F;
        else if (s[i] == '5')
            curr = curr->E;
        else if (s[i] == '4')
            curr = curr->D;
        else if (s[i] == '3')
            curr = curr->C;
        else if (s[i] == '2')
            curr = curr->B;
        else if (s[i] == '1')
            curr = curr->A;

        // reached leaf node
        if (curr->G == NULL && curr->F == NULL && curr->E == NULL && curr->D == NULL && curr->C == NULL && curr->B == NULL && curr->A == NULL)
        {
            ans += curr->data;
            curr = root;
        }
    }
    // cout<<ans<<endl;
    // return ans + '\0';
    return ans;
}