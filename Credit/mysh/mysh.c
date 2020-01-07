#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>  
// #include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

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
#include "myshval.h"
#include "perr.h"

int main(int argc, char* const *argv)
{
    E_PRINTF("stoop");

    mysh = *argv;
    int opt; 
    while((opt = getopt(argc, argv, ":hc:")) != -1)  
    {  
        switch(opt)  
        {  
            case 'c':
                D_PRINTF("c cmd: %s\n", optarg);
                exec_line(optarg);
                return myshval; 
            case '?':  
                PERR("%s: -%c: invalid option\n\n", mysh, optopt); 
                print_help();
                return 0;
			case 'h':
                print_help();
				return 0;
            case ':':  
                PERR("%s: -%c: option requires an argument\n", mysh, optopt);  
                break; 
        }  
    }  


    if(optind < argc)
    {
        run_script(argv[optind]);
    }
    
    run_interactive();
    return 0;
}

void run_script(char * file_name)
{
    D_PRINTF("Non-option argument %s\n", file_name);
    int fd = open(file_name, O_RDONLY);
    if(fd == -1)
    {
        PERR("%s: %s: %s\n",mysh, file_name, strerror(errno));
        exit(127);
    }
    while(1)
    {
        ssize_t n = 0;
        char * line = NULL;
        if(fd_get_line(fd, &line, &n) != EOF)
        {
            exec_line(line);                
            FREE(line);
            if(myshval != 0)
            {
                cd_clear();
                exit(myshval);
            }

        }
        else
        {
            FREE(line);
            cd_clear();
            exit(myshval);
        }
    }

}

void run_interactive()
{
    while(1)
    {     
        signal(SIGINT, handle_sig_in);    
        char * line = NULL;
        S_PRINTF("%s/n", line);
        char * prompt = get_prompt();
        line = readline(prompt);
        S_PRINTF("%s/n", line);
        if(line == NULL)
        {
            printf("exit\n");
            exit((myshval == 0)?0:-1);
        }                        
        add_history(line);
        exec_line(line);   
        signal(SIGINT, handle_sig_in);
        free(line);
        free(prompt);
    }
    cd_clear();
}

//NO LEAK 16/12/29
void exec_line(char * line)
{
    int command_count = 0;
    command ** c = GetCommands(line, &command_count);        
    run_command(c, command_count);
    //free c
    for (int i = 0; i < command_count; i++)
    {
        command * act = *(c+i);
        //free args
        for (int j = 0; j < act->arg_count + 1; j++)
        {
            free(*(act->args + j));
        }
        free(act->args);        
        free(act);
    }
    free(c);    
}

char * get_prompt()
{
    D_PRINTF("ENTER get_prompt()\n");
    char * prompt = NULL;
    char * dir = GetCurrentDir();
    
    
    size_t size = strlen(dir) + 100;
    CALLOC(prompt, size);

    if(myshval == 0)
    {
        snprintf(prompt, size, "%s%s $ %s",ANSI_COLOR_BRIGHT_WHITE, dir, ANSI_COLOR_RESET);
    }
    else
    {
        snprintf(prompt, size, "%s%d%s %s%s $ %s", ANSI_COLOR_BRIGHT_RED, myshval, ANSI_COLOR_RESET, ANSI_COLOR_BRIGHT_WHITE, dir, ANSI_COLOR_RESET);
    }
    
    // FREE(dir);
    
    D_PRINTF("LEAVE get_prompt() with result \"%s\"\n", prompt);
    return prompt; 

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
                    PERR("%s: syntax error nearby %c\n", mysh, (*(c + i))->delim);
                    myshval = 2;
                    return;
                } 
            }
            else
            {
                PERR("%s: syntax error nearby %c\n", mysh, (*(c + i))->delim);
                myshval = 2;
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
            PERR("%s: %s: command not found \n", mysh, *(*(c + i))->args);
        }
        //TODO: FreeCommand
    }    

}

void handle_sig_in(int sig) 
{ 
    signal(SIGINT, handle_sig_in);   
    D_PRINTF("IN PROMPT CTRL+C");
    printf("\n"); // Move to a new line
    rl_on_new_line(); // Regenerate the prompt on a newline
    rl_replace_line("", 0); // Clear the previous text
    rl_redisplay();
    myshval = 128 + sig; 
}
void print_help(void)
{    
    printf("MYSHEL, version 0.1.2 - release-(x86_64-pc-linux-gnu)\n");
    printf("Jiri Mianen Pelc - Faculty of Mathematics and Physics, Charles Universit, Prague, Czech republic\n\n");
    printf("Usage:\t%s -c command\n", mysh);
    printf("\t%s script-file\n", mysh);
}


