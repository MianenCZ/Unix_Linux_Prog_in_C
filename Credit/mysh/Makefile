
.PHONY: clean cltest

SHELL = /bin/sh
CC = /usr/bin/gcc -ggdb3
CFLAGS = -std=c99 -Wall -Wextra -ggdb3
INCLUDES = -I Includes
OUT = ./mysh
TARGETS = callbin.o mysh.o getcmd.o execcmd.o cd.o fd_getline.o myshval.o exit.o
DTARGETS = D_callbin.o D_mysh.o D_getcmd.o D_execcmd.o D_cd.o fd_getline.o myshval.o exit.o
DFLAGS = -D DEBUG
TESTFILE = mytests/Phase01.mysh

all: release

r: release

d: debug

val: release
	#valgrind --leak-check=full --show-leak-kinds=all $(OUT) $(TESTFILE)
	valgrind --leak-check=full $(OUT) $(TESTFILE)
	#valgrind --leak-check=full --show-leak-kinds=all $(OUT) -c "echo A;echo B;echo C\n"

debug: clean $(DTARGETS)
	$(CC) $(CFLAGS) $(INCLUDES) -lfl $(TARGETS) -l readline -o $(OUT) 

release: clean $(TARGETS)
	$(CC) $(CFLAGS) $(INCLUDES) -lfl $(TARGETS) -l readline -o $(OUT) 

mysh.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c mysh.c

callbin.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c callbin.c

getcmd.o:
	flex getcmd.c
	$(CC) $(CFLAGS) $(INCLUDES) -c lex.yy.c -o getcmd.o

execcmd.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c execcmd.c

cd.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c cd.c
fd_getline.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c fd_getline.c

myshval.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c myshval.c

exit.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c exit.c

D_mysh.o: 
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c mysh.c

D_callbin.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c callbin.c

D_getcmd.o:
	flex getcmd.c
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES) -c lex.yy.c -o getcmd.o

D_execcmd.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c execcmd.c

D_cd.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c cd.c

clean:
	-@rm -f $(DTARGETS)
	-@rm -f $(TARGETS)
	-@rm -f lex.yy.c	
	-@rm -f $(OUT)

install:
	cp mysh /user/bin

test: cltest
	gcc -I Includes -D TEST fd_getline.c -o TEST_fd_getline.out

cltest:
	-@rm TEST_fd_getline.out
