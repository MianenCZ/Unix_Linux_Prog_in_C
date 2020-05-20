#ifndef GETCMD_DEF_H
#define GETCMD_DEF_H
#include "types.h"

command** parse_line(char* Line, int* Count);
void build_cmd(int delim);
void add_arg(char * text);
command** end_parse(int*Count);

//manually declaration of missing declaration
//based on https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
int fileno(FILE *stream);

#endif