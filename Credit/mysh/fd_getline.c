#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "fd_getline.h"

int fd_getchar(const int fd)
{
    static char buff[GET_C_BUFF_SIZE];
    static char* chr;
    static int  pos = 0;
    static ssize_t  ret = 0;

    if (pos >= ret)
    {
        if ((ret = read(fd, buff, GET_C_BUFF_SIZE)) > 0)
        {
            chr = buff;
            pos = 0;
            return *(chr + pos++);
        }
        else
            return EOF;
    }
    else
        return *(chr + pos++);
}

int fd_get_line(const int fd, char **lineptr, ssize_t *n)
{   
    if(*lineptr == NULL)
    {
        MALLOC((*lineptr), BUFF_SIZE);
        *n = BUFF_SIZE;
    }


    char c;
    int pos = 0;
    while(1)
    {
        c = fd_getchar(fd);
        if (c == EOF || c == '\n')
        {
            (*lineptr)[(pos)] = '\0';
            return ((c == EOF)? EOF:pos);
        }
        else
        {
            (*lineptr)[(pos)++] = c;
        }

        if(pos >= *n)
        {
            REALLOC((*lineptr), (*n) * 17 / 8);
            *n = (*n) * 17 / 8;
        }        
    }
}

#ifdef TEST
 #include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        printf("Test USAGE: %s filename", *argv);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    while(1)
    {
        ssize_t n = 0;
        char * line = NULL;
        if(fd_get_line(fd, &line, &n) != EOF)
        {
            printf("%s\n", line);
            free(line);
        }
        else
        {
            free(line);
            break;
        }
        
    
    }

    return 0;
}


#endif