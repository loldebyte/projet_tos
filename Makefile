CC=gcc
CFLAGS=-Wall
DEPS=
OBJ= launcher.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: launcher.o
	$(CC) -o launcher launcher.o

clean:
	rm launcher.o
