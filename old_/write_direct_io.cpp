#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BUF_SIZE 4096
#define UPDATE_TIMES 300
void write_char(unsigned char buffer, int num)
{
    unsigned char buf[1];
    buf[0] = buffer;
    // sprintf((char *)buffer, "%c", num);
    int fd = open("tmp2", O_WRONLY | O_CREAT | O_APPEND | O_DIRECT, 0777);
    if (fd < 0)
    {
        perror("open tmp failed");
        // free(buffer);
        exit(1);
    }
    for (int j = 0; j <= num; j++)
    {
        if (!write(fd, buf, 1))
            perror("write tmp failed");
    }
    close(fd);
}
int main(int argc, char *argv[])
{
    unsigned char *buffer;
    if (posix_memalign((void **)&buffer, 512, BUF_SIZE))
    {
        perror("posix_memalign failed");
        exit(1);
    }
    memset(buffer, 'f', BUF_SIZE);
    int n, s, L = 255;
    int i = 0;
    int a[L];
    int x = 1, y = 2;
    a[0] = x;
    a[1] = x;
    a[2] = y;
    for (n = 0; n < L; n++)
        a[n] = (64 * n) + 1;
    for (n = 0; n < L; n++)
    {
        write_char((unsigned char)i, a[n]);
        printf("%d ", a[n]);
        i++;
    }
    free(buffer);
    return 0;
}