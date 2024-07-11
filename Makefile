.PHONY: all clean

CC = gcc
CFLAGS = -o1 -g -Wall

all:
	$(CC) $(CFLAGS) src/main.c -o Timer

clean:
	rm Timer.exe

