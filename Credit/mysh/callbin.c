#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "callbin.h"
#include "debug.h"

// #define DEBUG
// #define INFO


int CallBin(char* const argv[])
{  
  pid_t   my_pid;
  if ((my_pid = fork()) == 0) {
      if(execvp(argv[0], argv) == -1)
      {
        E_PRINTF("execvp(%s, argv) returns -1\n", argv[0]); 
        return -1;
      }
      
  exit(0);
  }

  // D_PRINTF("Child %d await start\n", my_pid);
  int sleepret = AwaitChild(my_pid);
  // I_PRINTF("Child %d await stop\n", my_pid);
  return sleepret;
}


int AwaitChild(pid_t child_pid)
{
  int status;
  while (0 == waitpid(child_pid , &status , WNOHANG)) 
  {

  }
  I_PRINTF("WEXITSTATUS %d WIFEXITED %d [status %d]\n", WEXITSTATUS(status), WIFEXITED(status), status);
  if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) 
  {
    return -1;
  }
  return 0;
}