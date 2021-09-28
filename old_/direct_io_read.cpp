#include <fcntl.h>
#include <memory.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 32*1024*1024
#define read_times 50

void Set_CPU(int CPU_NUM);

int main(int argc, char *argv[])
{
    unsigned char *buffer;
    if (posix_memalign((void **)&buffer, 512, BUF_SIZE))
    {
        perror("posix_memalign failed");
        exit(1);
    }
    // memset(buffer, 'F', BUF_SIZE);
    for (int i = 0; i < read_times; i++)
    {
        int pFile = open(argv[1], O_RDONLY | O_DIRECT, 0777);
        if (pFile < 0)
        {
            perror("open tmp failed");
            free(buffer);
            exit(1);
        }
        printf("read %ld Bytes\n", read(pFile, buffer, BUF_SIZE));
        printf("%s\n", buffer);
        close(pFile);
    }
    printf("pid=%d\n", getpid());
    free(buffer);
    return 0;
}
void Set_CPU(int CPU_NUM)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(CPU_NUM, &set);
    sched_setaffinity(0, sizeof(set), &set);
}