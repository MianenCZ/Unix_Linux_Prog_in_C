#ifndef MYSH_EXEC_PIPELINE
#define MYSH_EXEC_PIPELINE

#include "types.h"
void exec_pipeline(command ** c, int Count);

void handle_sig_kill_childrens(int signo);

#endif