#ifndef CALLBIN_DEF_H
#define CALLBIN_DEF_H
#include <sys/types.h>
/*
CallBin - Call binary using argv arguments. Search $PATH if argv[0] is not / or ./
const char** argv - Arguments for calling binary. argv[0] is binary name, argv[>0] are arguments

returns - 0 if call was successful, -1 otherwise
*/
int CallBin(char* const argv[]);

/*
AwaitChild - Wait for child process to end
child_pid - Child process identification

returns - 0 if waiting finished successfully, -1 on timeout or halt
*/
int AwaitChild(pid_t child_pid);

void handle_sigint(int sig);
#endif