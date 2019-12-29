#define _POSIX_C_SOURCE 500
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "callbin.h"
#include "debug.h"
#include "myshval.h"

// #define DEBUG
// #define INFO

static pid_t my_pid;

int CallBin(char* const argv[])
{  
  signal(SIGINT, handle_sigint);
  if ((my_pid = fork()) == 0) 
  {
      if(execvp(argv[0], argv) == -1)
      {
        E_PRINTF("execvp(%s, argv) returns -1\n", argv[0]); 
        exit(-1);
      }
      
  exit(0);
  }

  // D_PRINTF("Child %d await start\n", my_pid);
  AwaitChild(my_pid);
  // I_PRINTF("Child %d await stop\n", my_pid);
  return 0;
}


int AwaitChild(pid_t child_pid)
{
  int status;
  while (0 == waitpid(child_pid , &status , WNOHANG)) 
  {

  }
  return 0;
}

void handle_sigint(int sig) 
{ 
  D_PRINTF("handle SIGINT\n");
  kill(my_pid, SIGINT);
  myshval = 128 + sig;
}