#ifndef PERR_DEF_H
#define PERR_DEF_H

#include <stdio.h>
#include "colorout.h"   
#define PERR(args...)\
    do {\
        fprintf(stderr,ANSI_COLOR_RED);\
        fprintf(stderr,args);\
        fprintf(stderr,ANSI_COLOR_RESET);\
        fflush (stderr);\
    } while (0)

#endif



