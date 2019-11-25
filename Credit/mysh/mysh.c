#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>  
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "mysh.h"
#include "callbin.h"
#include "types.h"
#include "getcmd.h"
#include "debug.h"
#include "execcmd.h"


// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <getopt.h>

int main(int argc, char* const *argv)
{
    int opt; 
    while((opt = getopt(argc, argv, ":if:lrx")) != -1)  
    {  
        switch(opt)  
        {  
            case 'i':  
            case 'l':  
            case 'r':  
                printf("option: %c\n", opt);  
                break;  
            case 'f':  
                printf("filename: %s\n", optarg);  
                break;  
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
    // S_PRINTF("S_PRINTF\n");
    // printf("Test\n");
    // I_PRINTF("I_PRINTF\n");
    // printf("Test\n");
    // E_PRINTF("E_PRINTF\n");
    // printf("Test\n");
    // D_PRINTF("D_PRINTF\n");
    // printf("Test\n");

    // int ret = CallBin((char*)(argv + 1));    
    // printf("Return value: %d\n", ret);
    // return 0;

    char *b = NULL;
    size_t bufsize = 0;


    while(1)
    {
        printf("Mysh$ ");
        getline(&b,&bufsize,stdin);
        command ** c = GetCommands(b);
        
        for (size_t i = 0; *(c + i) != NULL; i++)
        {
            // E_PRINTF("True command **> i = %zu\n", i);
            // E_PRINTF("*(c + i + 1) != NULL ---> %s\n", (*(c + i + 1) != NULL)?"TRUE":"FALSE");
            if(ExecCommand(*(c+i)) == -1)
            {
                printf(ANSI_COLOR_RED);
                printf("%s: %s: %s \n", *argv, *(*(c + i))->args, strerror(errno));
                printf(ANSI_COLOR_RESET);
                //Error ocurred
            }
            //TODO: FreeCommand
        }    
    }
    return(0);
}

