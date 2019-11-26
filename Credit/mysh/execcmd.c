#include "execcmd.h"
#include "types.h"
#include "debug.h"
#include "callbin.h"


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
    I_PRINTF("ExecIfSpecial: %s", *(c->args));
    return 0;
}