#include <string.h>
#include <stdlib.h>
#include "execcmd.h"
#include "types.h"
#include "debug.h"
#include "callbin.h"
#include "cd.h"


int ExecCommand(command * c)
{
    int res = ExecIfSpecial(c);
    if(res == 1)
    {
        return res;
    }
    return CallBin(c->args);
}

int ExecIfSpecial(command * c)
{
    //TODO: Solve return value<=
    const char* cmd = *c->args;
    D_PRINTF("ExecIfSpecial %s\n", cmd);
    if(0 == strcmp("exit", cmd))
    {
        if(c->arg_count == 1)
        {
            //TODO: Exit with last cmd exitvalue
            exit(0);
        }
        else if(c->arg_count == 2)
        {
            int exit_code = atoi(*(c->args + 1));
            if (exit_code == 0 && *(c->args + 1)[0] != '0')
            {
                printf("Not a Number!");
                exit(2);
            }
            else
            {
                D_PRINTF("Exiting with code: %d\n", exit_code);
                exit(exit_code);
            }
        }
        else
        {
            printf("exit: too many arguments\n");
            return 1;
        }
        
        
    }
    else if(0 == strcmp("cd", cmd))
    {
        RunCD(c->args, c->arg_count);    
        return 1;
    }
    else if(0 == strcmp("alias", cmd))
    {
        printf("Not implemented command\n");
        return 1;
    }
    else
    {
        /* code */
    }

    return 0;
    
}