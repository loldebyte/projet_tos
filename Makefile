CC=gcc 
CFLAGS=-Wall -march=native -o
DEPS=lol_shell.c hashmap.c
TARGETS=launcher.c 

TEST_SHELL= tests/test_lol_shell.c
TEST_HASHMAP= tests/test_lol_hashmap.c

BENCH_CC=gcc -march=native -O2 -pg -o
BENCH_TARGET=benchmark/benchmark.c
BENCH_OUT=benchmark/bench
BENCH_DEP=hashmap.c
BENCH_ARGS= i

DEBUG_CC=gcc -ggdb

make: $(TARGETS) $(DEPS)
	$(CC) $(CFLAGS) launcher $(TARGETS) $(DEPS)

test: $(TEST) $(DEPS)
	$(CC) $(CFLAGS) test_shell $(TEST_SHELL) $(DEPS)
	./test_shell
	$(CC) $(CFLAGS) test_hashmap $(TEST_HASHMAP) $(DEPS)
	./test_hashmap

benchmark: $(BENCH_TARGET) $(BENCH_DEP)
	$(BENCH_CC) $(BENCH_OUT) $(BENCH_TARGET) $(BENCH_DEP)
	$(foreach var,$(BENCH_ARGS),time ./$(BENCH_OUT) $(var);)

debug: $(TEST) $(DEPS)
	$(DEBUG_CC) $(CFLAGS) test_shell $(TEST_SHELL) $(DEPS)
	./test_shell
	$(DEBUG_CC) $(CFLAGS) test_hashmap $(TEST_HASHMAP) $(DEPS)
	./test_hashmap

clean:
	rm test_*

.PHONY: benchmark test debug clean
