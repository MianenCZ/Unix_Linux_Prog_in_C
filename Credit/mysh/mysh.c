#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>  
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "types.h"
#include "mysh.h"
#include "callbin.h"
#include "getcmd.h"
#include "debug.h"
#include "execcmd.h"
#include "memory.h"
#include "cd.h"
#include "colorout.h"
#include "fd_getline.h"


// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <getopt.h>

static char* Me;

int main(int argc, char* const *argv)
{
    Me = *argv;
    int opt; 
    while((opt = getopt(argc, argv, ":hc:")) != -1)  
    {  
        switch(opt)  
        {  
            case 'c':  
            {
                int command_count = 0;
                D_PRINTF("c cmd: %s\n", optarg);
                command ** c = GetCommands(optarg, &command_count);        
                run_command(c, command_count); 
                return 0;
            }
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  


    if(optind < argc)
    {
        D_PRINTF("Non-option argument %s\n", argv[optind]);
        int fd = open(argv[optind], O_RDONLY);

        while(1)
        {
            ssize_t n = 0;
            char * line = NULL;
            if(fd_get_line(fd, &line, &n) != EOF)
            {
                exec_line(line);
            }
            else
            {
                return(0);
            }
        }
    }
    
    while(1)
    {         
        char * dir = GetCurrentDir();    
        char * line = readline("Mysh$ ");
        add_history(line);
        exec_line(line);
    }
    return(0);
}

void exec_line(char * line)
{
    int command_count = 0;
    command ** c = GetCommands(line, &command_count);        
    run_command(c, command_count);
}

void run_command(command ** c, int Count)
{
    D_PRINTF("Run comand Line: Count = %d\n", Count);
    for (int i = 0; i < Count; i++)
    {
        if(*(*(c + i))->args == NULL)
        {
            E_PRINTF("NULL DELIM: %c\n", (*(c + i))->delim);
            if((*(c + i))->delim == ';')
            {
                if(i + 1 == Count)
                {
                    //This is Last one
                    break;
                }
                else
                {
                    printf("error: 1: Syntax error nearby %c\n", (*(c + i))->delim);
                    return;
                } 
            }
            else
            {
                printf("error: 1: Syntax error nearby %c\n", (*(c + i))->delim);
                return;
            }



            E_PRINTF("*(*(c + i))->args == NULL\n");
            continue;
            // exit(142);
        }

        // E_PRINTF("True command **> i = %zu\n", i);
        // E_PRINTF("*(c + i + 1) != NULL ---> %s\n", (*(c + i + 1) != NULL)?"TRUE":"FALSE");
        if(ExecCommand(*(c+i)) == -1)
        {
            printf(ANSI_COLOR_RED);
            printf("%s: %s: %s \n", Me, *(*(c + i))->args, "command not found");
            printf(ANSI_COLOR_RESET);
            //Error ocurred
        }
        //TODO: FreeCommand
    }    

}

