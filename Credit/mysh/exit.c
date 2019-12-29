#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "exit.h"
#include "myshval.h"
#include "perr.h"


void RunEXIT(char ** args, int argc)
{
    if(argc == 1)
        {
            //TODO: Exit with last cmd exitvalue
            exit(myshval);
        }
        else if(argc == 2)
        {
            int exit_code = atoi(*(args + 1));
            if (exit_code == 0 && *(args + 1)[0] != '0')
            {
                PERR("%s: exit: %s: numeric argument required", mysh, *(args + 1));
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
            PERR("%s: exit: too many arguments", mysh);
            return;
        }
}