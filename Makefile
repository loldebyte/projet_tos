CC=gcc -march=native -o
CFLAGS=-Wall
DEPS=lol_shell.c hashmap.c
TARGETS=launcher.c 
TEST= tests/test_lol_shell.c

make: $(TARGETS) $(DEPS)
	$(CC) launcher $(TARGETS) $(DEPS)

test: $(TEST) $(DEPS)
	$(CC) test $(TEST) $(DEPS)
	./test
