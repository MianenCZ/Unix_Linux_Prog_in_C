#ifndef FD_GETLINE_DEF_H
#define FD_GETLINE_DEF_H

#include <sys/types.h>

# define GET_C_BUFF_SIZE 1024

# define BUFF_SIZE 128
# define EOF (-1)


int fd_getchar(const int fd);
/*
fd_get_line() reads an entire line from file, storing the address of the buffer containing the text into *lineptr.

The buffer is null-terminated and DOES NOT include the newline character, if one was found.
If *lineptr is set to NULL and *n is set 0 before the call, then fd_get_line() will allocate a buffer for storing the line.

Alternatively,  before  calling  fd_get_line(), *lineptr can contain a pointer to a malloc(3)-allocated buffer *n bytes in size.  If the buffer is not large enough to hold the line, fd_get_line() resizes it with realloc(3), updating *lineptr and *n as necessary.
*/
int fd_get_line(const int fd, char **lineptr, ssize_t *n);

#endif
