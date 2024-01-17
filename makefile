# Austin Van Braeckel
# 2020-10-08
# Makefile for compiling the source code

CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: list.o

clean:
	rm -fv *.o

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c