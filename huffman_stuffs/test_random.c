#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define PAGE_SIZE 16 * 1024
int main(int argc, char *argv[])
{
    unsigned char *buffer;
    long STATE[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file)
        exit(1);
    struct stat sb;
    if (stat(argv[1], &sb) == -1)
    {
        perror("stat");
        exit(1);
    }
    buffer = (unsigned char *)malloc(sb.st_size * sizeof(unsigned char));
    fread(buffer, sb.st_size, 1, input_file);
    unsigned int num = sb.st_size / (PAGE_SIZE) / 3;
    if (sb.st_size % (PAGE_SIZE)*3 > 0)
        num++;
    printf("size : %ld\n", sb.st_size);
    printf("num : %d\n", num);
    //p[num][3][PAGE_SIZE]
    unsigned char ***p = (unsigned char ***)malloc(num * sizeof(unsigned char **));
    for (int i = 0; i < num; i++)
    {
        p[i] = (unsigned char **)malloc(3 * sizeof(unsigned char *));
        for (int j = 0; j < 3; j++)
        {
            p[i][j] = (unsigned char *)malloc(PAGE_SIZE * sizeof(unsigned char));
            memset(p[i][j], 0xFF, PAGE_SIZE);
        }
    }
    unsigned int cnt = 0;
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < PAGE_SIZE && cnt < sb.st_size; k++)
            {
                p[i][j][k] = buffer[cnt++];
            }
        }
    }
    long total = 0;
    for (int i = 0; i < num; i++)
    {
        int pos = 0;
        while (pos < PAGE_SIZE)
        {
            for (int j = 0; j < 8; j++)
            {
                int tmp = ((p[i][0][pos] & (1 << j)) >> j) + ((p[i][1][pos] & (1 << j)) >> j) * 2 + ((p[i][2][pos] & (1 << j)) >> j) * 4;
                total++;
                switch (tmp)
                {
                case 0:
                    STATE[5]++;
                    break;
                case 1:
                    STATE[2]++;
                    break;
                case 2:
                    STATE[6]++;
                    break;
                case 3:
                    STATE[1]++;
                    break;
                case 4:
                    STATE[4]++;
                    break;
                case 5:
                    STATE[3]++;
                    break;
                case 6:
                    STATE[7]++;
                    break;
                case 7:
                    STATE[0]++;
                    break;
                }
            }
            pos++;
        }
    }
    printf("%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n", STATE[0], STATE[1], STATE[2], STATE[3], STATE[4], STATE[5], STATE[6], STATE[7]);
    printf("%ld\n", total);
    fclose(input_file);
    free(buffer);
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            free(p[i][j]);
        }
    }
    for (int i = 0; i < num; i++)
    {
        free(p[i]);
    }
    free(p);
    return 0;
}