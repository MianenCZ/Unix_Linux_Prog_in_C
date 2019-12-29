#ifndef DEBUG_DEF_H
    #define DEBUG_DEF_H

    #include <stdio.h>
    #include "colorout.h"

    #ifdef DEBUG
    #define D_PRINTF(args...)\
        do {\
            printf (ANSI_COLOR_YELLOW);\
            printf ("\t");\
            printf (args);\
            printf (ANSI_COLOR_RESET);\
            fflush (stdout);\
        } while (0)
        #define E_PRINTF(args...)\
        do {\
            printf (ANSI_COLOR_RED);\
            printf ("\t");\
            printf (args);\
            printf (ANSI_COLOR_RESET);\
            fflush (stdout);\
        } while (0)
        #define S_PRINTF(args...)\
        do {\
            printf (ANSI_COLOR_GREEN);\
            printf ("\t");\
            printf (args);\
            printf (ANSI_COLOR_RESET);\
            fflush (stdout);\
        } while (0)
        #define I_PRINTF(args...)\
        do {\
            printf (ANSI_COLOR_BLUE);\
            printf ("\t");\
            printf (args);\
            printf (ANSI_COLOR_RESET);\
            fflush (stdout);\
        } while (0)        
    #else
        #define D_PRINTF(args...)
        #define E_PRINTF(args...)
        #define S_PRINTF(args...)
        #define I_PRINTF(args...)
    #endif 
#endif