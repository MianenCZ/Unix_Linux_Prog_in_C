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
#include <sys/wait.h>

#include "types.h"
#include "mysh.h"
#include "getcmd.h"
#include "debug.h"
#include "memory.h"
#include "utils.h"
#include "colorout.h"
#include "fd_getline.h"
#include "myshval.h"
#include "perr.h"
#include "exec_pipeline.h"

struct sigaction act = {0};

int main(int argc, char* const *argv)
{
    mysh = *argv;
    int opt; 
    while((opt = getopt(argc, argv, ":hc:")) != -1)  
    {  
        switch(opt)  
        {  
            case 'c':
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
    cd_init();
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
            FREE_S(line);
            // if(myshval != 0)
            // {
            //     cd_clear();
            //     exit(myshval);
            // }

        }
        else
        {
            FREE_S(line);
            cd_clear();
            exit(myshval);
        }
    }

}

void run_interactive()
{
    cd_init();
    while(1)
    {     
        struct sigaction act = {0};
        // memset(&act, 0, sizeof(struct sigaction));
        sigemptyset(&act.sa_mask);
        act.sa_handler = handle_sig_in;
        // act.sa_flags = SA_RESETHAND;
        if (-1 == sigaction(SIGINT, &act, NULL))
        {
            perror("sigaction()");
            exit(EXIT_FAILURE);
        }

        char * line = NULL;
        S_PRINTF("%s/n", line);
        char * prompt = get_prompt();
        line = readline(prompt);
        I_PRINTF("readline red\n");
        S_PRINTF("%s/n", line);
        if(line == NULL)
        {
            printf("exit\n");
            exit((myshval == 0)?0:-1);
        }                        
        add_history(line);
        exec_line(line);   
        free(line);
        free(prompt);
    }
    cd_clear();
}

void exec_line(char * line)
{
    int command_count = 0;
    myshval = 0;
    command ** c = parse_line(line, &command_count);   
    if(myshval == 0) 
        exec_pipeline(c, command_count);
    
    //free c
    for (int i = 0; i < command_count; i++)
    {
        command * act = *(c+i);
        // free args
        for (int j = 0; j < act->arg_count + 1; j++)
        {
            FREE_S(*(act->args + j));
        }
        FREE_S(act->args);   
        FREE_S(act->input_file);   
        FREE_S(act->append_file);   
        FREE_S(act->rewrite_file);   
        FREE_S(act);
    }
    FREE_S(c);    
}

char* get_prompt()
{
    D_PRINTF("ENTER get_prompt()\n");
    char * prompt = NULL;
    char * dir = get_current_dir();   
    
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

#pragma GCC diagnostic ignored "-Wunused-parameter"
void handle_sig_in(int signo) 
{ 
    D_PRINTF("handle_sig_in\n");
    myshval = 128 + signo; 
    printf("\n"); // Move to a new line
    char* p = get_prompt();
    rl_set_prompt(p);
    free(p);
    rl_on_new_line(); // Regenerate the prompt on a newline
    // rl_replace_line("", 0); // Clear the previous text
    rl_redisplay();    
}

void print_help(void)
{    
    printf("MYSHEL, version 0.1.2 - release-(x86_64-pc-linux-gnu)\n");
    printf("Jiri Mianen Pelc - Faculty of Mathematics and Physics, Charles Universit, Prague, Czech republic\n\n");
    printf("Usage:\t%s -c command\n", mysh);
    printf("\t%s script-file\n", mysh);
}


