#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define BUF_SIZE 1024
#define UPDATE_TIMES 2048
int main(int argc, char *argv[])
{
    unsigned char *buffer;
    if (posix_memalign((void **)&buffer, 512, BUF_SIZE))
    {
        perror("posix_memalign failed");
        exit(1);
    }
    memset(buffer, 'f', BUF_SIZE);
    for (int k = 0; k < UPDATE_TIMES; k++)
    {
        int fd = open("tmp", O_WRONLY | O_CREAT | O_DIRECT | O_APPEND, 0777);
        if (fd < 0)
        {
            perror("open tmp failed");
            free(buffer);
            exit(1);
        }
        int i = 20;
        for (i; i < 127; i++)
        {
            buffer[i] = i;
        }
        if (!write(fd, buffer, BUF_SIZE))
            perror("write tmp failed");
        close(fd);
    }
    free(buffer);
    return 0;
}