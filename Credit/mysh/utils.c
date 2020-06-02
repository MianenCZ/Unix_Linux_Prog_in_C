#define _XOPEN_SOURCE  500
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#include "utils.h"
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
static char * Current;
static char * CurrentTilde;


void exec_cd(char ** args, int argc)
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
            PERR("%s: cd: OLDPWD not set\n", mysh);
            myshval = 1;
            return;
        }
        char * target = strdup(Previous);
        change_dir(target);
        printf("%s\n", Current);
    }
    else if(argc == 1)
    {
        char * home = getenv("HOME");
        if(home == NULL)
        {            
            PERR("%s: cd: HOME not set\n", mysh);
            myshval = 1;
            return;
        }
        change_dir(strdup(home));
    }
    else
    {
        D_PRINTF("chdir(%s);\n", *(args + 1));
        change_dir(strdup(*(args + 1)));
    }
}

char * get_current_dir()
{    
    return CurrentTilde;
}

void change_dir(char * dir)
{
    char * target = NULL;
    if(*dir == '~')
    {
        target = fromTilde(dir);
        if(target == NULL)
            return;
    }
    else
    {
        target = strdup(dir);
    }

    FREE_S(Previous);
    Previous = strdup(Current);
    FREE_S(Current);
    FREE_S(CurrentTilde);
    if(chdir(target) != 0)
    {
        PERR("%s: cd: %s: %s\n", mysh, dir, strerror(errno));
        myshval = 1;
    }    
    MALLOC(Current, PATH_MAX);
    getcwd(Current, PATH_MAX);
    CurrentTilde = toTilde(Current);

    FREE_S(dir);
    FREE_S(target);
}

void cd_clear(void)
{
    FREE_S(Current);
    FREE_S(CurrentTilde);
    FREE_S(Previous);
}

void cd_init(void)
{    
    MALLOC(Current, PATH_MAX);
    getcwd(Current, PATH_MAX);
    if(Current == NULL)
    {
        PERR("%s: cd_init: Cant obtain current directory\n", mysh);
        myshval = 10;
        return;
    }
    Previous = NULL;
    char * oldpwd = getenv("OLDPWD");
    if(oldpwd != NULL)
        Previous = strdup(oldpwd);  
    CurrentTilde = toTilde(Current);
}

char * fromTilde(char * path)
{
    if(*path == '~')
    {
        char * new;
        char * home = getenv("HOME");
        if(home == NULL)
        {            
            PERR("%s: cd: HOME not set\n", mysh);
            myshval = 1;
            return NULL;
        }        
        CALLOC(new, strlen(home) + strlen(path));
        strcpy(new, home);
        strncat(new, (path + 1), strlen(path) - 1);
        return new;
        FREE_S(home);
    }
    return strdup(path);
}

char * toTilde(char * path)
{
    char * home = getenv("HOME");  
    char * res = NULL;
    if(home == NULL || strlen(home) > strlen(path))
    {
        //Cannot be change to ~
        res = strdup(path);   
    }   
    else
    {
        //Is possible to change to ~
        if(strncmp(home, path, strlen(home)) == 0)
        {
            //Match found
            CALLOC(res, strlen(path) - strlen(home) + 2);
            *res = '~';
            strcpy((res+1), (path + strlen(home)));
        }
        else
        {
            //No match
            res = strdup(path);
        }        
    }
    // FREE_S(home);
    return res;
}

void exec_exit(char ** args, int argc)
{
    if(argc == 1)
        exit(myshval);

    else if(argc == 2)
    {
        int exit_code = atoi(*(args + 1));
        if (exit_code == 0 && *(args + 1)[0] != '0')
        {
            PERR("%s: exit: %s: numeric argument required", mysh, *(args + 1));
            cd_clear();
            exit(2);
        }
        else
        {
            D_PRINTF("Exiting with code: %d\n", exit_code);
            cd_clear();
            exit(exit_code);
        }
    }
    else
    {
        PERR("%s: exit: too many arguments", mysh);
        return;
    }
}