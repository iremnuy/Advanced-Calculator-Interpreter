CC = gcc
CFLAGS = -Wall -g

all: program

program: main.o operator.o
    $(CC) $(CFLAGS) -o program main.o operator.o

main.o: main.c operator.h
    $(CC) $(CFLAGS) -c main.c

operator.o: operator.c operator.h
    $(CC) $(CFLAGS) -c operator.c

clean:
    rm -f program *.o