CC=gcc -o
CFLAGS=-Wall
DEPS=
TARGETS= launcher.c lol_shell.c

make: $(TARGETS)
	$(CC) launcher $(TARGETS)
