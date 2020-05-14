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
#include "myshval.h"
#include "perr.h"

#define     ENV_OVERWRITE        1
#define     ENV_NONOWERWRITE     0

/*
Previous directory
 */
static char * Previous;
//TODO: SET PWD, SET OLDPWD


void RunCD(char ** args, int argc)
{
    if(argc > 2)
    {
        
        PERR("%s: cd: too many arguments\n", mysh);
        myshval = 1;
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
        char* cdir = GetCurrentDir();
        printf("%s\n", cdir);
        free(cdir);
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
    char * current = getcwd(NULL, 0);

    char * res = NULL;
    if(home == NULL || strlen(home) > strlen(current))
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

    // free(home);
    free(current);
    
    D_PRINTF("Leave: GetCurrentDir with result: \"%s\"\n", res);  
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
        FREE_S(new);
        FREE_S(home);
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
            FREE_S(Previous);
        }
        Previous = prev;
        // setenv("OLDPWD", Previous, ENV_OVERWRITE);
        // setenv("PWD", dir, ENV_OVERWRITE);
        //free(prev);
    }
    else
    {
        PERR("%s: cd: %s: %s\n", mysh, dir, strerror(errno));
        myshval = 1;
        FREE_S(prev);
    }
    // 

    D_PRINTF("Leave: DoChange\n");
}

void cd_clear(void)
{
    if(Previous != NULL)
    {
        FREE_S(Previous);
    }
}

void cd_init(void)
{
    Previous = getenv("OLDPWD");
    if(Previous == NULL)
    {
        Previous = get_current_dir_name();
    }
}


