CC = gcc
CFLAGS = -Wall

advcalc: main.c
	$(CC) $(CFLAGS) -o advcalc main.c 
