#ifndef MYSHVAL_DEF_H
#define MYSHVAL_DEF_H

    #define myshval (*__getmyshval())
    #define mysh (*__getmysh())

    int *__getmyshval(void);
    char**__getmysh(void);
#endif