#ifndef MYSH_DEF
#define MYSH_DEF

#include "types.h"
int ExecCommand(command * c);
void exec_line(char * line);
void handle_sig_in(int signo, siginfo_t *sinfo, void *context);
char * get_prompt();
void print_help(void);
void run_interactive(void);
void run_script(char * file_name);
#endif