#ifndef GETCMD_DEF_H
#define GETCMD_DEF_H
#include "types.h"

command** GetCommands(char* Line, int* Count);
void AddCmd(char delim);
void AddArg(char * text);
command** GetCMD(int*Count);

//manully declaration of missing declaration
//based on https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
int fileno(FILE *stream);

#endif