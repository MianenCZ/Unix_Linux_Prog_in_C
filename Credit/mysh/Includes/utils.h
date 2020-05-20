#ifndef UTILS_DEF_H
#define UTILS_DEF_H



/*
Run cd command
*/
void exec_cd(char ** args, int argc);


/*
Returns formated current directory.
~ if current directory is in $HOME
full if not
*/
char * get_current_dir();

/*
Change current directory
Accept relative, full or from $HOME path
*/
void change_dir(char * Dir);

void cd_clear(void);

void cd_init(void);

char * fromTilde(char * path);

char * toTilde(char * path);

void exec_exit(char ** args, int argc);

#endif