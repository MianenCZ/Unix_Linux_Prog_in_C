#include "myshval.h"

static int __myshval = 0;
static char* __mysh;
int *__getmyshval(void)
{
    return &__myshval;
}

char **__getmysh(void)
{
    return &__mysh;
}