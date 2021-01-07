CC=gcc
CFLAGS=-Wall
DEPS=
OBJ= launcher.o
to_clean=launcher.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: launcher.o
	$(CC) -o launcher launcher.o

clean:
	rm $(to_clean)
