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
#include "testhuffman.h"
#include "Raw_ber.h"
#include "After_ber.h"
#define LC_type "5LC"
#define DATAPATTERNNUM 256
#define TLC 2.32
#define MB 1024 * 1024
using namespace std;

// A Huffman tree node
struct minHeap_5LCNode
{
    unsigned char data;                     // One of the input characters
    int Freq_5LC;                           // Freq_5LCuency of the character
    minHeap_5LCNode *D, *C, *B, *A, *Erase; // Left and right child

    minHeap_5LCNode(unsigned char data, int Freq_5LC)
    {
        D = NULL;
        C = NULL;
        B = NULL;
        A = NULL;
        Erase = NULL;
        this->data = data;
        this->Freq_5LC = Freq_5LC;
    }
};

struct compare
{
    bool operator()(minHeap_5LCNode *l, minHeap_5LCNode *r)
    {
        return (l->Freq_5LC > r->Freq_5LC);
    }
};
// STL priority queue to store heap tree, with respect
// to their heap root node value
priority_queue<minHeap_5LCNode *, vector<minHeap_5LCNode *>, compare> minHeap_5LC;
// utility function for the priority queue
static int calcCollision(int accumulate, float score, int n, int size, string filename, float BER, float CR);
static string write_encode(unsigned char *str, int length);
static void write_char_buf(string path, unsigned char *str, int size);
static void write_String(string path, string str);
static void write_Table_5LC(string path);
static void printCodes(struct minHeap_5LCNode *root, string str);
static void storeCodes(struct minHeap_5LCNode *root, string str);
static void HuffmanCodes(int size);
static void calcFreq_5LC(unsigned char *str, int n);
static string decode_file(struct minHeap_5LCNode *root, string s);
static int isLeaf(struct minHeap_5LCNode *root);
map<unsigned char, string> Table_5LC;
map<unsigned char, int> Freq_5LC;
map<unsigned char, double> DataBER_5LC;
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
float *actual_symbol_5LC = NULL;
encode_info *score_5LC(const char *path, float accumulate, float alpha)
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
    actual_symbol_5LC = (float *)malloc(num * sizeof(float));
    encode_info *encode_info_5LC = (encode_info *)malloc(num * sizeof(encode_info));
    for (int i = 0; i < num; i++)
    {
        encode_info_5LC[i].filesize = 0;
        encode_info_5LC[i].CR = 0, encode_info_5LC[i].BeforeCapacity = 0, encode_info_5LC[i].AfterCapacity = 0;
        encode_info_5LC[i].CellCost = 0;
        encode_info_5LC[i].BER = 0, encode_info_5LC[i].BeforeBER = 0, encode_info_5LC[i].BufferBER = 0, encode_info_5LC[i].AfterBER = 0;
        encode_info_5LC[i].score = 0;
        encode_info_5LC[i].encode_time = 0;
        fin.seekg(i * BUF_SIZE, ios::beg);
        cout << "current pos : " << fin.tellg();
        cout << " number of buffer : " << i;
        str[i] = (unsigned char *)calloc('0', BUF_SIZE * sizeof(unsigned char));
        fin.read((char *)str[i], BUF_SIZE);
        cout << " read : " << fin.gcount() << " Bytes" << endl;
        if (fin.gcount() < SKIP_THRESHOLD)
        {
            cout << "file too small, skip" << endl;
            free(str[i]);
            break;
        }
        calcFreq_5LC(str[i], fin.gcount());
        struct timeval start, end, diff;
        // 開始計算時間
        gettimeofday(&start, NULL);
        // 主要計算
        HuffmanCodes(fin.gcount());
        // 結束計算時間
        gettimeofday(&end, NULL);
        // 計算實際花費時間
        timersub(&end, &start, &diff);
        encode_info_5LC[i].encode_time = diff.tv_sec + (float)diff.tv_usec / 1000000.0;
        string name = "partition_temp/" + string(LC_type) + "/" + filename + "/" + filename + "_" + to_string(i);
        cout << name << endl;
        write_char_buf(name, str[i], fin.gcount());
        encodedString = write_encode(str[i], fin.gcount());
        string name_compressed = name + "_compressed";
        write_String(name_compressed, encodedString);
        decodedString = decode_file(minHeap_5LC.top(), encodedString);
        string name_decode = name + "_uncompress";
        write_String(name_decode, decodedString);
        string name_Table_5LC = name + "_Table_5LC";
        write_Table_5LC(name_Table_5LC);
        BER_after *AfterBER_OB = new BER_after();
        BER_before *BeforeBER_OB = new BER_before();
        encode_info_5LC[i].BeforeBER = BeforeBER_OB->raw_ber(name.c_str());
        encode_info_5LC[i].BeforeCapacity = BeforeBER_OB->raw_cells();
        encode_info_5LC[i].AfterBER = AfterBER_OB->after_ber(name_compressed.c_str());
        encode_info_5LC[i].AfterCapacity = AfterBER_OB->after_cells();
        cout << "Before BER : " << encode_info_5LC[i].BeforeBER << endl;
        cout << "After BER : " << encode_info_5LC[i].AfterBER << endl;
        cout << "BER reduction: " << (encode_info_5LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER << endl;
        cout << "Compress ratio: " << encode_info_5LC[i].AfterCapacity / encode_info_5LC[i].BeforeCapacity * 100 << endl;
        cout << "Score: " << alpha * (1 - (encode_info_5LC[i].AfterCapacity / encode_info_5LC[i].BeforeCapacity)) - (1 - alpha) * ((encode_info_5LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER) << endl;
        calcCollision(accumulate, alpha * (1 - (encode_info_5LC[i].AfterCapacity / encode_info_5LC[i].BeforeCapacity)) - (1 - alpha) * ((encode_info_5LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER), i, fin.gcount(), filename, (encode_info_5LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER, encode_info_5LC[i].AfterCapacity / encode_info_5LC[i].BeforeCapacity * 100);
        encode_info_5LC[i].score = alpha * (1 - (encode_info_5LC[i].AfterCapacity / encode_info_5LC[i].BeforeCapacity)) - (1 - alpha) * ((encode_info_5LC[i].AfterBER - encode_info_5LC[i].BeforeBER) / encode_info_5LC[i].BeforeBER);
        free(str[i]);
        free(AfterBER_OB);
        free(BeforeBER_OB);
    }
    free(str);
    fin.close();
    return encode_info_5LC;
}
int calcCollision(int accumulate, float score, int n, int size, string filename, float BER, float CR)
{
    ofstream fout;
    // fcompare.open("compare.csv", ios::app);
    fout.open("5LC.csv", ios::app);
    if (!fout)
    {
        cout << "檔案無法開啟\n";
        exit(1);
    }
    unsigned int pattern = 0, total_pattern = 0, total_Freq_5LC = 0;
    vector<pair<unsigned char, int>> tmp(Freq_5LC.cbegin(), Freq_5LC.cend());
    sort(tmp.begin(), tmp.end(), [](pair<unsigned char, int> a, pair<unsigned char, int> b) {
        return a.second > b.second;
    });
    float probability;
    unsigned int buf = 0;
    actual_symbol_5LC[n] = 0;
    for (map<unsigned char, int>::iterator v = Freq_5LC.begin(); v != Freq_5LC.end(); v++)
    {
        if (v->second != 0)
        {
            probability = (float)v->second / (float)size;
            buf = Table_5LC[v->first].length();
            actual_symbol_5LC[n] += (float)buf * probability;
        }
    }
    for (auto x = tmp.begin(); x != tmp.end(); x++)
    {
        if (x->second != 0)
            total_pattern++;
    }
    for (auto x = tmp.begin(); x != tmp.end(); x++)
    {
        if (float(total_Freq_5LC) / float(size) * 100 > accumulate)
        {
            cout << total_Freq_5LC << " : " << float(total_Freq_5LC) / float(size) * 100 << " : " << accumulate << " : " << total_pattern << " : " << pattern << endl;
            fout << filename << "_" << n << "," << total_Freq_5LC << "," << float(total_Freq_5LC) / float(size) * 100 << "," << accumulate << "," << total_pattern << "," << pattern << "," << BER << "," << CR << "," << score << endl;
            return pattern;
        }
        total_Freq_5LC += x->second;
        pattern++;
    }
    fout.close();
    return 0;
}
float *_get_actual_symbol_5LC(void)
{
    return actual_symbol_5LC;
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
        encodedString += Table_5LC[ch];
    }
    return encodedString;
}
void write_Table_5LC(string path)
{
    ofstream fout;
    int num_pattern = 0;
    fout.open(path, ios::out | ios::binary);
    if (!fout)
    {
        cout << "檔案無法開啟\n";
        exit(1);
    }
    fout << "Freq_5LCuency of each state : " << endl;
    vector<pair<unsigned char, int>> tmp(Freq_5LC.cbegin(), Freq_5LC.cend());
    sort(tmp.begin(), tmp.end(), [](pair<unsigned char, int> a, pair<unsigned char, int> b) {
        return a.second > b.second;
    });
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
    fout << "Coding Table_5LC : " << endl;
    for (auto x = tmp.begin(); x != tmp.end(); x++)
    {
        if (x->second != 0)
        {
            fout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)x->first;
            fout << ':' << Table_5LC[x->first] << endl;
            // cout << setiosflags(ios::fixed) << setw(2) << setfill('0') << hex << uppercase << (unsigned int)x->first;
            // cout << ':' << Table_5LC[x->first] << endl;
            // cout << dec;
            num_pattern++;
        }
    }
    fout << "Pattern Numbers  : " << dec << num_pattern << endl;
    fout.close();
}
// Utility function to check if this node is leaf
int isLeaf(struct minHeap_5LCNode *root)
{
    return !(root->D) && !(root->C) && !(root->B) && !(root->A) && !(root->Erase);
}
void printCodes(struct minHeap_5LCNode *root, string str)
{
    if (!root)
        return;
    if (isLeaf(root))
        cout << root->data << ": " << str << "\n";
    printCodes(root->D, str + "4");
    printCodes(root->C, str + "3");
    printCodes(root->B, str + "2");
    printCodes(root->A, str + "1");
    printCodes(root->Erase, str + "0");
}

// utility function to store characters along with
// there huffman value in a hash Table_5LC, here we
// have C++ STL map
void storeCodes(struct minHeap_5LCNode *root, string str)
{
    if (root == NULL)
        return;
    if (isLeaf(root))
    {
        if (str == "")
            str += "0";
        Table_5LC[root->data] = str;
    }
    storeCodes(root->D, str + "4");
    storeCodes(root->C, str + "3");
    storeCodes(root->B, str + "2");
    storeCodes(root->A, str + "1");
    storeCodes(root->Erase, str + "0");
}
// function to build the Huffman tree and store it
// in minHeap_5LC
void HuffmanCodes(int size)
{
    struct minHeap_5LCNode *D = NULL, *C = NULL, *B = NULL, *A = NULL, *Erase = NULL, *top = NULL;
    while (!minHeap_5LC.empty())
        minHeap_5LC.pop();
    unsigned int total_pattern = 0;
    for (map<unsigned char, int>::iterator v = Freq_5LC.begin(); v != Freq_5LC.end(); v++)
    {
        if (v->second != 0)
        {
            total_pattern++;
            minHeap_5LC.push(new minHeap_5LCNode(v->first, v->second));
        }
    }
    if ((5 % 4) == (total_pattern % 4))
    {
        cout << (5 % 4) << " = " << total_pattern << " mod 4 " << endl;
        D = minHeap_5LC.top();
        minHeap_5LC.pop();
        C = minHeap_5LC.top();
        minHeap_5LC.pop();
        B = minHeap_5LC.top();
        minHeap_5LC.pop();
        A = minHeap_5LC.top();
        minHeap_5LC.pop();
        Erase = minHeap_5LC.top();
        minHeap_5LC.pop();
        top = new minHeap_5LCNode(0, D->Freq_5LC + C->Freq_5LC + B->Freq_5LC + A->Freq_5LC + Erase->Freq_5LC);
        top->D = D;
        top->C = C;
        top->B = B;
        top->A = A;
        top->Erase = Erase;
        minHeap_5LC.push(top);
    }
    else if ((4 % 4) == (total_pattern % 4))
    {
        cout << (4 % 4) << " = " << total_pattern << " mod 4 " << endl;
        C = minHeap_5LC.top();
        minHeap_5LC.pop();
        B = minHeap_5LC.top();
        minHeap_5LC.pop();
        A = minHeap_5LC.top();
        minHeap_5LC.pop();
        Erase = minHeap_5LC.top();
        minHeap_5LC.pop();
        top = new minHeap_5LCNode(0, C->Freq_5LC + B->Freq_5LC + A->Freq_5LC + Erase->Freq_5LC);
        top->D = NULL;
        top->C = C;
        top->B = B;
        top->A = A;
        top->Erase = Erase;
        minHeap_5LC.push(top);
    }
    else if ((3 % 4) == (total_pattern % 4))
    {
        cout << (3 % 4) << " = " << total_pattern << " mod 4 " << endl;
        B = minHeap_5LC.top();
        minHeap_5LC.pop();
        A = minHeap_5LC.top();
        minHeap_5LC.pop();
        Erase = minHeap_5LC.top();
        minHeap_5LC.pop();
        top = new minHeap_5LCNode(0, B->Freq_5LC + A->Freq_5LC + Erase->Freq_5LC);
        top->D = NULL;
        top->C = NULL;
        top->B = B;
        top->A = A;
        top->Erase = Erase;
        minHeap_5LC.push(top);
    }
    else if ((2 % 4) == (total_pattern % 4))
    {
        cout << (2 % 4) << " = " << total_pattern << " mod 4 " << endl;
        A = minHeap_5LC.top();
        minHeap_5LC.pop();
        Erase = minHeap_5LC.top();
        minHeap_5LC.pop();
        top = new minHeap_5LCNode(0, A->Freq_5LC + Erase->Freq_5LC);
        top->D = NULL;
        top->C = NULL;
        top->B = NULL;
        top->A = A;
        top->Erase = Erase;
        minHeap_5LC.push(top);
    }
    while (minHeap_5LC.size() != 1)
    {
        if (minHeap_5LC.size() >= 5)
        {
            D = minHeap_5LC.top();
            minHeap_5LC.pop();
            C = minHeap_5LC.top();
            minHeap_5LC.pop();
            B = minHeap_5LC.top();
            minHeap_5LC.pop();
            A = minHeap_5LC.top();
            minHeap_5LC.pop();
            Erase = minHeap_5LC.top();
            minHeap_5LC.pop();
            top = new minHeap_5LCNode(0, D->Freq_5LC + C->Freq_5LC + B->Freq_5LC + A->Freq_5LC + Erase->Freq_5LC);
            top->D = D;
            top->C = C;
            top->B = B;
            top->A = A;
            top->Erase = Erase;
            minHeap_5LC.push(top);
        }
        else if (minHeap_5LC.size() == 4)
        {
            C = minHeap_5LC.top();
            minHeap_5LC.pop();
            B = minHeap_5LC.top();
            minHeap_5LC.pop();
            A = minHeap_5LC.top();
            minHeap_5LC.pop();
            Erase = minHeap_5LC.top();
            minHeap_5LC.pop();
            top = new minHeap_5LCNode(0, C->Freq_5LC + B->Freq_5LC + A->Freq_5LC + Erase->Freq_5LC);
            top->D = NULL;
            top->C = C;
            top->B = B;
            top->A = A;
            top->Erase = Erase;
            minHeap_5LC.push(top);
            break;
        }
        else if (minHeap_5LC.size() == 3)
        {
            B = minHeap_5LC.top();
            minHeap_5LC.pop();
            A = minHeap_5LC.top();
            minHeap_5LC.pop();
            Erase = minHeap_5LC.top();
            minHeap_5LC.pop();
            top = new minHeap_5LCNode(0, B->Freq_5LC + A->Freq_5LC + Erase->Freq_5LC);
            top->D = NULL;
            top->C = NULL;
            top->B = B;
            top->A = A;
            top->Erase = Erase;
            minHeap_5LC.push(top);
            break;
        }
        else if (minHeap_5LC.size() == 2)
        {
            A = minHeap_5LC.top();
            minHeap_5LC.pop();
            Erase = minHeap_5LC.top();
            minHeap_5LC.pop();
            top = new minHeap_5LCNode(0, A->Freq_5LC + Erase->Freq_5LC);
            top->D = NULL;
            top->C = NULL;
            top->B = NULL;
            top->A = A;
            top->Erase = Erase;
            minHeap_5LC.push(top);
            break;
        }
    }
    storeCodes(minHeap_5LC.top(), "");
}

/*Code改到這裡*/

// utility function to store map each character with its
// Freq_5LCuency in input string
void calcFreq_5LC(unsigned char *str, int n)
{
    // printf("test1\n");
    for (int i = 0; i < 256; i++)
    {
        // printf("test%d\n",i);
        DataBER_5LC[i] = 0;
        Freq_5LC[i] = 0;
        Table_5LC[i] = "";
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
        Freq_5LC[ch]++;
        DataBER_5LC[ch] = BER;
        BER = 0;
    }
}

// function iterates through the encoded string s
// if s[i]=='1' then move to node->right
// if s[i]=='0' then move to node->left
// if leaf node append the node->data to our output string
string decode_file(struct minHeap_5LCNode *root, string s)
{
    string ans = "";
    struct minHeap_5LCNode *curr = root;
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
    // return ans + '\0';
    return ans;
}