#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>

#include "memory.h"



bool IsPalidrome(int fd);
int ReadOpts(int argc, char const *argv[]);
__off_t GetSize(int fd);



int CaseInsinsitive = 0;

int main(int argc, char const *argv[])
{
    ReadOpts(argc, argv);

    int fdesc = open(argv[1], O_RDONLY);


    printf("Result: %s\n", (IsPalidrome(fdesc))?"true":"false");

    //*/
    close(fdesc);

    return EXIT_SUCCESS;
}

bool IsPalidrome(int fd)
{
    
    long size = GetSize(fd);    
    printf("FileSize: %ld\n", size);

    char* l;
    char *r;
    MALLOC(l, sizeof(char));
    MALLOC(r, sizeof(char));

    for(int i = 0; i < size / 2 + 1; i++)
    {
        lseek(fd, i, SEEK_SET);
        read(fd, l, sizeof(char));
        lseek(fd, size - i - 1, SEEK_SET);
        read(fd, r, sizeof(char));

        if((*l) != (*r))
            return false;
    }
    return true;
}

int ReadOpts(int argc, char const *argv[]) 
{
    int opt;
    while ((opt = getopt(argc, argv, "i")) != -1) 
    {
        switch (opt) {
        case 'i':
            CaseInsinsitive = 1;
            break;
        case 't':
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s FILE_NAME [-i CASE_INSENSITIVE]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}

__off_t GetSize(int fd)
{
    __off_t size = lseek(fd, 0, SEEK_END);
    return size;
}