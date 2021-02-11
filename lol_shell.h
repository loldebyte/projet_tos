#ifndef LOL_SHELL_H
#define LOL_SHELL_H

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include "hashmap.h"

#define SEP " "
#define BUFFER 102


typedef enum EXEC_FLAGS {
    WAIT=10, DONT_WAIT, DEFINE_VARIABLE
} EXEC_FLAGS;

typedef struct EXECUTION_CONF {
    char ** arguments;
    EXEC_FLAGS exec_type;
    size_t number_of_arguments;
} EXECUTION_CONF;

bool validate_input(char (*)[]);
void split_into_arguments(char *, EXECUTION_CONF *);
void execute(EXECUTION_CONF *, word_hashmap **);
void set_execution_type(EXECUTION_CONF *);
bool strings_are_the_same(char *, char *);
EXECUTION_CONF * exec_conf_factory(void);
void exec_conf_destructor(EXECUTION_CONF *);
void dealloc_last_argument(EXECUTION_CONF *);

void print_all_args(EXECUTION_CONF *);
void print_all_chars_in_str(char *);

bool save_variable(char *, word_hashmap **);
bool extract_key_value_pair(char **, int32_t *, char *);
bool replace_variables_by_their_value(char *, word_hashmap *);
int number_of_chars_behind_cash(char *);

#endif
