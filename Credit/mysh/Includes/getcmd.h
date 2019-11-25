#ifndef GETCMD_DEF_H
#define GETCMD_DEF_H
#include "types.h"

command** GetCommands(char* Line);
void AddCmd(char delim);
void AddArg(char * text);
command** GetCMD();

#endif