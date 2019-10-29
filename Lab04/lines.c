#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s PATH LINE_SIZE\n", argv[0]);
        return 1;
    }

    int OnLineSize = atoi(argv[2]);

    size_t BuffSize = OnLineSize;
    char* Buffer = malloc(sizeof(char) * BuffSize);
    int rsize = 0;

    int fdesc = open(argv[1], O_RDONLY);

    int onLine = 0;
    while( 0 != (rsize = read(fdesc, Buffer, BuffSize)))
    {
        for (int i = 0; i < rsize; i++)
        {
            putchar(Buffer[i]);
            onLine++;

            if(Buffer[i] == '\n')
                onLine = 0;
            if(onLine >= OnLineSize)
                onLine = 0;
                putchar('\n');
        }        
    }    
    putchar('\n');
    fflush(stdout);


    return 0;
}
