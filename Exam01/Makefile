.PHONY: all run clean

OUT = ./exam
CC = /usr/bin/gcc
CFLAGS = -std=c99 -Wall -Wextra -ggdb3

all: clean $(OUT)
	

$(OUT): exam.o
	$(CC) $(CFLAGS) $(INCLUDES) exam.o -l pthread -o $(OUT)

exam.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c exam.c -l pthread

run: clean $(OUT)
	find /usr -type f -perm -o=r | $(OUT) 16

clean:
	@-rm exam.o
	@-rm $(OUT) 