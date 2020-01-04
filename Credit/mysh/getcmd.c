/* Predefines */
%option noyywrap
%option nounput
%option noinput
%{

#include <stdlib.h>
#include <stdio.h>
#include <sys/queue.h>
#include <string.h>
#include <stdbool.h>
#include "getcmd.h"
#include "types.h"
#include "memory.h"
#include "debug.h"



struct arg
{
    char* s;
    // This macro does the magic to point to other nextarg
    TAILQ_ENTRY(arg) nextarg;
};

struct cmd
{
    command* value;
    // This macro does the magic to point to other nextarg
    TAILQ_ENTRY(cmd) nextcmd;
};


TAILQ_HEAD(args, arg) args;
TAILQ_HEAD(cmds, cmd) cmds;
static int argcount = 0;
static int cmdcount = 0;
static bool dirty = false;


%}

%%

#.* { //Comment
    S_PRINTF("Comment: %s\n", yytext);

}
(\ |\t)+ { 
    I_PRINTF("WhiteChars: /%s/\n", yytext);
    // free(yytext);
}
;|(>>)|<|>|\| { //Deliminer
    I_PRINTF("Delim %s\n", yytext);
    S_PRINTF("Dirty = %s\n", (dirty)?"TRUE":"FALSE");
    if(yytext == ">>")
    {
        AddCmd('2');
    }
    AddCmd(yytext[0]);
    // free(yytext);
}
\"((\\\")|[^"\n\r])+\" { //Basic keyword
    // AddArg(yytext);
    S_PRINTF("\"MATCH\": %s\n", yytext);
    char * text = NULL;
    MALLOC(text, (sizeof(yytext) - 2));
    strncpy(text, (yytext + 1), sizeof(yytext) - 2);
    S_PRINTF("MATCH: %s\n", text);
    AddArg(text);
    free(text);
    // free(yytext);
}
([^ "\t\n\r><;\|])+ { //Basic keyword
    AddArg(yytext);
    //"
}
\n { //End of entry
    S_PRINTF("NewLine\n");
    S_PRINTF("Dirty = %s\n", (dirty)?"TRUE":"FALSE");
    if(dirty)
    {
        AddCmd('\n');
    }
    // free(yytext);
}
. { 
    E_PRINTF("Unknown character:'%s'\n", yytext);
    // free(yytext);
}
%%

command** GetCommands(char* Line, int*Count)
{
    S_PRINTF("GetCommands(%s)\n", Line);
    // Initialize the args before use
    TAILQ_INIT(&args);
    TAILQ_INIT(&cmds);

    yy_scan_string(Line);
    yylex();
    yylex_destroy();
    // yy_delete_buffer(YY_CURRENT_BUFFER);

    D_PRINTF("after yylex(); DIRTY: %s\n", (dirty)?"TRUE":"FALSE");
    if(dirty)
    {
        AddCmd('\n');
    }
    return GetCMD(Count);
}

void AddCmd(char delim)
{   
    char** arg_vals = NULL;
    MALLOC(arg_vals, argcount + 1);
    
    struct arg * e = NULL;
    // print the queue
    int i = 0;
    TAILQ_FOREACH(e, &args, nextarg)
    {
        S_PRINTF("res: %s\n", e->s);
        *(arg_vals + i) =  e->s;
        i++;
    }
    *(arg_vals + argcount) =  NULL;

    // free the elements from the queue
    while (!TAILQ_EMPTY(&args))
    {
        e = TAILQ_FIRST(&args);
        TAILQ_REMOVE(&args, e, nextarg);
        FREE(e);
    }

    command* c = NULL;
    MALLOC(c,1);
    c->args = arg_vals;
    c->delim = delim;
    c->arg_count = argcount;

    struct cmd* cm = NULL;
    MALLOC(cm,1);
    cm->value = c;
    TAILQ_INSERT_TAIL(&cmds, cm, nextcmd);


    argcount = 0;
    dirty = false;
    cmdcount++;
}

void AddArg(char * text)
{
    dirty = true;
    struct arg * e = NULL;
    MALLOC(e,1);

    char* copy = NULL;
    size_t len = strlen(text);
    MALLOC(copy, len + 1);
    strncpy(copy, text, len);
    copy[len] = '\0';
    

    e->s = copy;
    // Actually insert the arg e into the queue at the end
    TAILQ_INSERT_TAIL(&args, e, nextarg);    
    argcount++;

}

command** GetCMD(int*Count)
{
    *Count = cmdcount;
    command ** result = NULL;
    MALLOC(result,cmdcount + 1);

    struct cmd* c = NULL;  
    int i = 0; 
    TAILQ_FOREACH(c, &cmds, nextcmd)
    {
        S_PRINTF("getcmd: %s\n", *((c->value)->args));
        *(result + i) = (c->value);
        i++;
    }
    *(result + cmdcount ) = NULL;

    // free the elements from the queue
    while (!TAILQ_EMPTY(&cmds))
    {
        c = TAILQ_FIRST(&cmds);
        TAILQ_REMOVE(&cmds, c, nextcmd);
        free(c);
        c = NULL;
    }
    cmdcount = 0;
    return result;

}