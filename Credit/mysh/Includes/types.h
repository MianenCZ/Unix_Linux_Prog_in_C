#ifndef TYPES_DEF_H
#define TYPES_DEF_H

typedef struct command_t
{
    char** args;
    int arg_count;
    char delim;
} command;
#endif