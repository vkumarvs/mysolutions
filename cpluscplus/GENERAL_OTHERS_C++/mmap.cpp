#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
 #include <string.h>
using namespace std;

#define SHARED_MEMORY_SIZE 1024
int main()
{
    void *m_sharedBase;
    int fd;
    fd = open("/home/vipin/mmap.txt", O_RDONLY);
               if (fd == -1)
               {
                 printf("unable to open file\n");
               }

    int sharedFd = 0;
    if((m_sharedBase = mmap(NULL, SHARED_MEMORY_SIZE, PROT_WRITE | PROT_READ,
                            MAP_SHARED|MAP_NORESERVE|MAP_LOCKED|MAP_POPULATE ,
                            fd, 0)) == (char*)-1)
    {
      printf("unable to map the required shared memory \n");
      perror("mmap");
      return -1;
    }

    memset(m_sharedBase, 0, SHARED_MEMORY_SIZE);
    sleep(100);
    return 0;
}

