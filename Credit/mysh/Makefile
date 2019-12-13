
.PHONY: clean 

SHELL = /bin/sh
CC = /usr/bin/gcc
CFLAGS = -std=c99 -Wall -Wextra 
INCLUDES = -I Includes -l readline
OUT = "mysh"
TARGETS = callbin.o mysh.o getcmd.o execcmd.o cd.o fd_getline.o
DTARGETS = D_callbin.o D_mysh.o D_getcmd.o D_execcmd.o D_cd.o fd_getline.o
DFLAGS = -D DEBUG

all: debug

r: release

d: debug

debug: clean $(DTARGETS)
	$(CC) $(CFLAGS) $(INCLUDES) -lfl $(TARGETS) -o $(OUT) 

release: clean $(TARGETS)
	$(CC) $(CFLAGS) $(INCLUDES) -lfl $(TARGETS) -o $(OUT) 

mysh.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c mysh.c

callbin.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c callbin.c

getcmd.o:
	flex getcmd.c
	$(CC) $(INCLUDES) -c lex.yy.c -o getcmd.o

execcmd.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c execcmd.c

cd.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c cd.c
fd_getline.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c fd_getline.c
D_mysh.o: 
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c mysh.c

D_callbin.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c callbin.c

D_getcmd.o:
	flex getcmd.c
	$(CC) $(DFLAGS) $(INCLUDES) -c lex.yy.c -o getcmd.o

D_execcmd.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c execcmd.c

D_cd.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c cd.c

clean:
	-@rm $(DTARGETS)
	-@rm $(TARGETS)
	-@rm lex.yy.c	
	-@rm $(OUT)

install:
	cp mysh /user/bin
