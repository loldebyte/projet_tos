#ifndef LOL_SHELL_H
#define LOL_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>

#define SEP " "
#define BUFFER 102


typedef enum EXEC_FLAGS {
    WAIT=10, DONT_WAIT
} EXEC_FLAGS;

typedef struct EXECUTION_CONF {
    char ** arguments;
    EXEC_FLAGS exec_type;
    size_t number_of_arguments;
} EXECUTION_CONF;

bool validate_input(char (*)[]);
void split_into_arguments(char *, EXECUTION_CONF *);
void execute(EXECUTION_CONF *);
void set_execution_type(EXECUTION_CONF *);
bool strings_are_the_same(char *, char *);
EXECUTION_CONF * exec_conf_factory(void);
void exec_conf_destructor(EXECUTION_CONF *);
void dealloc_last_argument(EXECUTION_CONF *);
void print_all_args(EXECUTION_CONF *);
void print_all_chars_in_str(char *);

#endif
