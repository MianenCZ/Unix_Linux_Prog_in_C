#include <string.h>
#include <stdlib.h>
#include "execcmd.h"
#include "types.h"
#include "debug.h"
#include "callbin.h"
#include "cd.h"
#include "myshval.h"
#include "exit.h"
#include "perr.h"


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
    const char* cmd = *c->args;
    D_PRINTF("ExecIfSpecial %s\n", cmd);
    if(0 == strcmp("exit", cmd))
    {
        RunEXIT(c->args, c->arg_count); 
        return 1;
    }
    else if(0 == strcmp("cd", cmd))
    {
        RunCD(c->args, c->arg_count);    
        return 1;
    }
    else if(0 == strcmp("alias", cmd))
    {
        PERR("%s: Not implemented command\n", mysh);
        myshval = 1;
        return 1;
    }
    else
    {
        return 0;
    }    
}