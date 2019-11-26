#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>  
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "types.h"
#include "mysh.h"
#include "callbin.h"
#include "getcmd.h"
#include "debug.h"
#include "execcmd.h"


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
            case 'h':  
            case 'c':  
                D_PRINTF("c cmd: %s\n", optarg);
                command ** c = GetCommands(optarg);        
                run_command(c); 
                return 0;
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  

    char *b = NULL;
    size_t bufsize = 0;


    while(1)
    {
        printf("Mysh$ ");
        getline(&b,&bufsize,stdin);
        command ** c = GetCommands(b);        
        run_command(c);
    }
    return(0);
}

void run_command(command ** c)
{
    for (size_t i = 0; *(c + i) != NULL; i++)
    {
        // E_PRINTF("True command **> i = %zu\n", i);
        // E_PRINTF("*(c + i + 1) != NULL ---> %s\n", (*(c + i + 1) != NULL)?"TRUE":"FALSE");
        if(ExecCommand(*(c+i)) == -1)
        {
            printf(ANSI_COLOR_RED);
            printf("%s: %s: %s \n", Me, *(*(c + i))->args, strerror(errno));
            printf(ANSI_COLOR_RESET);
            //Error ocurred
        }
        //TODO: FreeCommand
    }    

}

