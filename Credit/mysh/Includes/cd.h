#ifndef CD_DEF_H
#define CD_DEF_H



/*
Run cd command
*/
void RunCD(char ** args, int argc);


/*
Returns formated current directory.
~ if current directory is in $HOME
full if not
*/
char * GetCurrentDir();

/*
Change current directory
Accept relative, full or from $HOME path
*/
void ChangeDir(char * Dir);

void DoChange(char * dir);

void cd_clear(void);

void cd_init(void);

#endif