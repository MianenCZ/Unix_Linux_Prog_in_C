#include "execcmd.h"
#include "types.h"
#include "debug.h"
#include "callbin.h"


int ExecCommand(command * c)
{
    return CallBin(c->args);
}

int ExecIfSpecial(command * c)
{
    I_PRINTF("ExecIfSpecial: %s", *(c->args));
    return 0;
}