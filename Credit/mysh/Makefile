
.PHONY: clean cltest

SHELL = /bin/sh
CC = /usr/bin/gcc
CFLAGS = -std=c99 -Wall -Wextra -ggdb3
INCLUDES = -I Includes
OUT = ./mysh
TARGETS = callbin.o mysh.o getcmd.o cd.o fd_getline.o myshval.o exit.o exec_pipeline.o
DTARGETS = D_callbin.o D_mysh.o D_getcmd.o D_cd.o D_exec_pipeline.o fd_getline.o myshval.o exit.o
DFLAGS = -D DEBUG
TESTFILE = mytests/Phase01.mysh

all: release

r: release

d: debug

val: release
	-valgrind --leak-check=full --show-leak-kinds=all $(OUT) $(TESTFILE)
	#valgrind --leak-check=full $(OUT) $(TESTFILE)
	-valgrind --leak-check=full --show-leak-kinds=all $(OUT) -c "echo A;echo B;echo C\n"

debug: clean $(DTARGETS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TARGETS) -o $(OUT) -lfl -l readline

release: clean $(TARGETS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TARGETS) -o $(OUT) -lfl -l readline

mysh.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c mysh.c

callbin.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c callbin.c

getcmd.o:
	flex getcmd.l
	$(CC) $(CFLAGS) $(INCLUDES) -c lex.yy.c -o getcmd.o


cd.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c cd.c
fd_getline.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c fd_getline.c

myshval.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c myshval.c

exit.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c exit.c
exec_pipeline.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c exec_pipeline.c

D_mysh.o: 
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c mysh.c

D_callbin.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c callbin.c

D_getcmd.o:
	flex getcmd.l
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES) -c lex.yy.c -o getcmd.o


D_cd.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c cd.c
D_exec_pipeline.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c exec_pipeline.c

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
