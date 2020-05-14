#ifndef TYPES_DEF_H
#define TYPES_DEF_H

#define DELIM_NEWLINE       1
#define DELIM_COMMA         2
#define DELIM_PIPE          3

typedef struct command_t
{
    char** args;
    int arg_count;
    int delim;
    char* input_file;
    char* append_file;
    char* rewrite_file;
} command;
#endif