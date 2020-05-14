#define _POSIX_C_SOURCE 500

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "cd.h"
#include "exit.h"
#include "callpipeline.h"
#include "debug.h"
#include "myshval.h"
#include "memory.h"

#define WRITE_END 1
#define READ_END 0

#define ABBORT_EXIT     0x01
#define ABBORT_EMPTY    0x10


pid_t* pids = NULL;
size_t pipe_size;

int call_pipeline(char* const pipe_args[], size_t argc)
{
    int abort_val = 0;

    I_PRINTF("Pipe size = %zu\n", pipe_size);
    for (size_t i = 0; i < pipe_size; i++)
    {           
        if(pipe(mypipe) == -1)
        {
            fprintf(stderr,"pipe(mypipe) failed\n");
            exit(-1);
        }

        pids[i] = fork();        
        if(pids[i]==0)
        {            
            D_PRINTF("child %zu started\n",i);
            
            if(i != pipe_size - 1) //Not last
            {
                dup2(mypipe[WRITE_END], STDOUT_FILENO);
            }
            if(i != 0) //Not first
            {
                dup2(prev_read_fd, STDIN_FILENO);
            }            

            if(in_file != NULL)
            {
                int fd = open(in_file, O_RDONLY);
                //TODO: check file
                dup2(fd, STDIN_FILENO);
            }

            if(rewrite_file != NULL)
            {
                int fd = open(rewrite_file, O_CREAT | O_TRUNC | O_RDWR, 0666);
                
                //TODO: check file
                dup2(fd, STDOUT_FILENO);
            }
            else if(append_file != NULL)
            {
                int fd = open(append_file, O_CREAT | O_RDWR | O_APPEND, 0666);
                //TODO: check file
                dup2(fd, STDOUT_FILENO);
            }
            

            D_PRINTF("executing %s\n", *cmds );
            execvp(*cmds, cmds);
            fprintf(stderr, "Failed to execute echo number: %zu\n", i);
            //should not be reached
            //Print 
            exit(1);   
        }

        //Parent        
        close(prev_read_fd);
        prev_read_fd = mypipe[READ_END];
        close(mypipe[WRITE_END]);

        // for (size_t i = 0; i < pipe_size + 1; i++)
        // {
        //     free(*(cmds + i));
        //     *(cmds + i) = NULL;
        // }        
        // free(cmds);
    }

    close(prev_read_fd);

    if(abort_val == 0)
    {
        //Await childs
        int child_status = 0;
        for (size_t i = 0; i < pipe_size; i++)
        {
            if(pids[i] == 0)
                continue;
            D_PRINTF("P: waiting for child (%d) %zu\n",pids[i], i);
            int status = 0;
            while(0 == waitpid(pids[i] , &status, 0))
            {             
                sleep(1);
                printf("P: waitpid %zu\n",i);
            }    
            D_PRINTF("Child %zu exited with status %d\n", i, status);
            child_status |= status;
        }
        if(child_status != 0)
        {
            myshval = 127;
            //TODO: Right value
        }
    }
    else
    {
        //Kill childs
        for (size_t i = 0; i < pipe_size; i++)
        {
            if(pids[i] == 0)
                continue;
            kill(pids[i], SIGINT);
            D_PRINTF("Child %zu (pid: %d) killed\n", i, pids[i]);
            //TODO: check for 
        }
    }
    FREE_S(pids);
    D_PRINTF("Here\n");
    return 0;
}

size_t count_pipe(char* const args[], size_t argc)
{
    // E_PRINTF("count_pipe: argc = %zu\n", argc);
    size_t count = 1;
    for (size_t i = 0; i < argc-1; i++)
    {
        // E_PRINTF("count_pipe: strcmp(\"%s\", \"|\") == %d\n", args[i], strcmp(args[i], "|") == 0);
        if(strcmp(args[i], "|") == 0)
        {
            count++;
        }
    } 
    return count;
}