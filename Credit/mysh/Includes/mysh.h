#ifndef MYSH_DEF
#define MYSH_DEF

#include "types.h"
int ExecCommand(command * c);
void exec_line(char * line);
void run_command(command ** c, int Count);
void handle_sig_in(int sig);
#endif