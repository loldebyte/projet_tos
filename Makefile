CC=gcc
CFLAGS=-Wall

make: launcher.o
	$(CC) -o launcher launcher.o
