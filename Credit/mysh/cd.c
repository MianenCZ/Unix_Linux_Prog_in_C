#define _GNU_SOURCE
#include <unistd.h>
#undef _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "cd.h"
#include "memory.h"
#include "debug.h"

void RunCD(char ** args, int argc)
{
    if(argc > 2)
    {
        printf("cd: To many arguments\n");
        return;
    }
    else if(argc == 2 && strncmp(*(args + 1), "-", 1) == 0)
    {
        if(Previous == NULL)
        {
            E_PRINTF("Previous == NULL");
            return;
        }
        ChangeDir(Previous);
        printf("%s\n", GetCurrentDir());
    }
    else if(argc == 1)
    {
        char * home = getenv("HOME");
        ChangeDir(home);
    }
    else
    {
        D_PRINTF("chdir(%s);\n", *(args + 1));
        ChangeDir(*(args + 1));
    }
}

char * GetCurrentDir()
{    
    D_PRINTF("Enter: GetCurrentDir()\n");
    char * home = getenv("HOME");
    char * current = get_current_dir_name();

    char * res = NULL;
    if(strlen(home) > strlen(current))
    {
        //Cannot be change to ~
        res = strdup(current);   
    }   
    else
    {
        //Is possible to change to ~

        D_PRINTF("\tHomeDir: %s\n", home);
        D_PRINTF("\tCurrDir: %s\n", current);        
        // D_PRINTF("HomeSize: %zu\n", strlen(home));

        if(strncmp(home, current, strlen(home)) == 0)
        {
            CALLOC(res, strlen(current) - strlen(home) + 2);
            *res = '~';
            strcpy((res+1), (current + strlen(home)));
        }
        else
        {
            I_PRINTF("Cannot be converted:\n");
            res = current;
        }
        
    }

    // chdir(res);
    
    D_PRINTF("Enter: GetCurrentDir\n");  
    return res;     
}

void ChangeDir(char * dir)
{
    D_PRINTF("Enter: ChangeDir([char *] %s)\n", dir);
    if(*dir == '~')
    {
    char * new;
        char * home = getenv("HOME");        
        CALLOC(new, strlen(home) + strlen(dir));
        strcpy(new, home);
        strncat(new, (dir + 1), strlen(dir) - 1);
        S_PRINTF("Cd to: %s\n", new);
        DoChange(new);
        FREE(new);
        // FREE(home);
    }
    else
    {
        DoChange(dir);
    }
    
    D_PRINTF("Leave: ChangeDir\n");
    return;

}

void DoChange(char * dir)
{
    D_PRINTF("Enter: DoChange([char *] %s)\n", dir);

    char * prev = get_current_dir_name();
    if(chdir(dir) == 0)
    {
        if(Previous != NULL)
        {
            FREE(Previous);
        }
        Previous = prev;
    }
    else
    {
        printf("cd: %s: %s\n", dir, strerror(errno));
    }

    D_PRINTF("Leave: DoChange\n");
}
