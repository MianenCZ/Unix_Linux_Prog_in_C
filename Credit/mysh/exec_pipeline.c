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

#include "exec_pipeline.h"
#include "types.h"
#include "debug.h"
#include "myshval.h"
#include "perr.h"
#include "cd.h"
#include "exit.h"
#include "memory.h"

#define WRITE_END 1
#define READ_END 0

pid_t* pids = NULL;

void exec_pipeline(command ** coms, int coms_count)
{
    CALLOC(pids, coms_count);

    for (int i = 0; i < coms_count; i++)
    {
        command * cur = *(coms+i);
        D_PRINTF("%s (%d) %d\n\t", *(cur->args), cur->arg_count, cur->delim);
        for (int j = 0; j < cur->arg_count; j++)
        {
            D_PRINTF("%s", *(cur->args + j));
        }
        D_PRINTF("\n");
        D_PRINTF("\tinput_file = %s\n", cur->input_file);
        D_PRINTF("\tappend_file = %s\n", cur->append_file);
        D_PRINTF("\trewrite_file = %s\n", cur->rewrite_file);
    }

    //pipelines fields
    int mypipe[2];
    mypipe[0] = 0;
    mypipe[1] = 0;
    int prev_read_fd = 0;   
    int pipe_size = 0;

    for (int i = 0; i < coms_count; i++)
    {
        command * cur = *(coms+i);

        if(cur->arg_count == 0 && i != coms_count-1 && cur->delim == DELIM_COMMA)
        {
            PERR("%s: syntax error nearby ;\n", mysh); 
            myshval = 2;
        }
        if(cur->arg_count == 0 && i != coms_count-1 && cur->delim == DELIM_PIPE)
        {
            //TODO: stop pipe
            PERR("%s: syntax error nearby |\n", mysh); 
            myshval = 2;
        }
        if(i == coms_count-1 && cur->delim == DELIM_PIPE)
        {
            PERR("%s: syntax error nearby |\n", mysh); 
            myshval = 2;
        }

        if(strcmp(*(cur->args), "exit") == 0)
        {
            if(pipe_size == 0 && (cur->delim == DELIM_NEWLINE || cur->delim == DELIM_COMMA))
            {
                RunEXIT(cur->args, cur->arg_count); 
            }
            else
            {
                PERR("%s: exit: exit is not available in pipeline\n", mysh); 
            }            
        }
        if(strcmp(*(cur->args), "cd") == 0)
        {
            if(pipe_size == 0 && (cur->delim == DELIM_NEWLINE || cur->delim == DELIM_COMMA))
            {
                RunCD(cur->args, cur->arg_count);    
                continue;
            }
            else
            {
                PERR("%s: cd: cd is not available in pipeline\n", mysh); 
            }
        }
        
        if(pipe(mypipe) == -1)
        {
            //TODO: resolve fail reason
            PERR("%s: %s: pipe(mypipe) failed\n", mysh, *(cur->args)); 
            myshval = errno;
            return;
        }
   
        pipe_size++; 
        pids[i] = fork();    
        if(pids[i]==0)
        {
            //Child
            D_PRINTF("child %zu started\n",i);            
            if(cur->delim == DELIM_PIPE) //Not last
            {
                dup2(mypipe[WRITE_END], STDOUT_FILENO);
            }            
            if(pipe_size != 1) //Not first
            {
                dup2(prev_read_fd, STDIN_FILENO);
            }            

            if(cur->input_file != NULL)
            {
                int fd = open(cur->input_file, O_RDONLY);
                E_PRINTF("In open (%s) = %d", cur->input_file, fd);
                //TODO: check file
                dup2(fd, STDIN_FILENO);
            }

            if(cur->rewrite_file != NULL)
            {
                int fd = open(cur->rewrite_file, O_CREAT | O_TRUNC | O_RDWR, 0666);
                E_PRINTF("Rewrite open (%s) = %d", cur->rewrite_file, fd);
                
                //TODO: check file
                dup2(fd, STDOUT_FILENO);
            }
            else if(cur->append_file != NULL)
            {
                int fd = open(cur->append_file, O_CREAT | O_RDWR | O_APPEND, 0666);
                E_PRINTF("Append open (%s) = %d", cur->append_file, fd);
                //TODO: check file
                dup2(fd, STDOUT_FILENO);
            }            

            D_PRINTF("executing %s\n", *cur->args);
            execvp(*cur->args, cur->args);
            D_PRINTF(stderr, "Failed to execute %s in child: %zu\n", *cur->args, i);
            exit(1); 
        }
        // PARENT

        if(prev_read_fd != 0 && prev_read_fd != 1)
            close(prev_read_fd);
        prev_read_fd = mypipe[READ_END];
        if(mypipe[WRITE_END] != 0 && mypipe[WRITE_END] != 1)
            close(mypipe[WRITE_END]);


        //Wait for childs if cur is end of pipeline
        if(cur->delim == DELIM_NEWLINE || cur->delim == DELIM_COMMA)
        {
            int child_status = 0;
            for (int i = 0; i < coms_count; i++)
            {
                if(pids[i] == 0)
                    continue;
                D_PRINTF("P: waiting for child (%d) %zu\n",pids[i], i);
                int status = 0;
                while(0 == waitpid(pids[i] , &status, 0))
                {             
                    sleep(1);
                    printf("P: waitpid %d\n",i);
                }    
                D_PRINTF("Child %zu exited with status %d\n", i, status);
                child_status |= status;
            }
            if(child_status != 0)
                myshval = 127;
            else
                myshval = 0;
            pipe_size = 0;
        }
    }
    FREE_S(pids);   
    
}