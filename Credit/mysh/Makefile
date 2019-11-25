
.PHONY: clean 

SHELL = /bin/sh
CC = /usr/bin/gcc
CFLAGS = -std=c99 -Wall -Wextra 
INCLUDES = -I Includes
OUT = "mysh"
TARGETS = callbin.o mysh.o getcmd.o execcmd.o
DTARGETS = D_callbin.o D_mysh.o D_getcmd.o D_execcmd.o
DFLAGS = -D DEBUG

all: clean $(DTARGETS)
	$(CC) $(CFLAGS) $(INCLUDES) -lfl $(TARGETS) -o $(OUT) 

release: clean $(TARGETS)
	$(CC) -D DEBUG $(CFLAGS) $(INCLUDES) -lfl $(TARGETS) -o $(OUT) 

mysh.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c mysh.c

callbin.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c callbin.c

getcmd.o:
	flex getcmd.c
	$(CC) $(INCLUDES) -c lex.yy.c -o getcmd.o

execcmd.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c execcmd.c

D_mysh.o: 
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c mysh.c

D_callbin.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c callbin.c

D_getcmd.o:
	flex getcmd.c
	$(CC) $(DFLAGS) $(INCLUDES) -c lex.yy.c -o getcmd.o

D_execcmd.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCLUDES) -c execcmd.c

clean:
	-@rm callbin.o
	-@rm mysh.o
	-@rm getcmd.o
	-@rm lex.yy.c
	-@rm mysh

install:
	cp mysh /user/bin
