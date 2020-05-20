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