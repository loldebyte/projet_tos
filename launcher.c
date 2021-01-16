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

void assert_valid_input(char (*)[]);
void split_into_arguments(char *, EXECUTION_CONF *);
void execute(EXECUTION_CONF *);
void set_execution_type(EXECUTION_CONF *);
bool strings_are_the_same(char *, char *);
EXECUTION_CONF * exec_conf_factory(void);
void exec_conf_destructor(EXECUTION_CONF *);
void dealloc_last_argument(EXECUTION_CONF *);

void assert_valid_input(char (*input)[]) {
    size_t len = strlen(*input);
    assert(len > 2 && "Please enter a command.\n");
    assert((*input)[len-1] == '\n' && "Your command cant exceed 100 characters. Please try again.\n");
    (*input)[len-1] = 0; // the \n makes most commands fail
}

void split_into_arguments(char * input, EXECUTION_CONF * config) {
    size_t curr_cell = 0;
    char * token;
    while (token = strtok(curr_cell == 0 ? input : NULL, SEP), token != NULL) {
        config->arguments = realloc(config->arguments,
                                    sizeof(config->arguments)
                                    * (curr_cell+1));
        config->arguments[curr_cell] = token;
        curr_cell++;
    }
    config->number_of_arguments = curr_cell;
}

void execute(EXECUTION_CONF * config) {
    pid_t pid;
    int status;

    if (strcmp(config->arguments[0], "exit") == 0)
        exit(0);
    if (config->exec_type != WAIT)
        dealloc_last_argument(config);
    pid = fork();
    assert(pid != -1 && "Error : could not create child process");
    if (pid == 0) {
        int check_execvp = execvp(config->arguments[0], config->arguments);
        assert((check_execvp =! -1) && "Execvp failed");
    }
    else {
        if (config->exec_type == WAIT) {
            pid_t wait_status;
            while(wait_status = waitpid(pid, &status, 0), wait_status != pid)
                assert(wait_status != -1 && "Wait failed");
            }
        else if (config->exec_type == DONT_WAIT) {
            ;
        }   
        else {
            printf("NOT IMPLEMENTED !\n");
        }     
    }
}

void set_execution_type(EXECUTION_CONF * config) {
    if (strings_are_the_same(config->arguments[config->number_of_arguments-1], "&")) {
        config->exec_type = DONT_WAIT;
    }
    config->exec_type = WAIT;
}

bool strings_are_the_same(char * arg, char * to_compare) {
    int val;
    val = strcmp(arg, to_compare);
    return val == 0 ? true : false;
}

void dealloc_last_argument(EXECUTION_CONF * config) {
    config->arguments = realloc(config->arguments,
                                sizeof(config->arguments)
                                * (config->number_of_arguments-1));
    config->number_of_arguments--; // not included in the line above for explicitness' sake
}

EXECUTION_CONF * exec_conf_factory(void) {
    EXECUTION_CONF * conf = malloc(sizeof(EXECUTION_CONF));
    conf->arguments = malloc(sizeof(1));
    conf->number_of_arguments = 0;
    conf->exec_type = WAIT;
    return conf;
}

void exec_conf_destructor(EXECUTION_CONF * conf) {
    free(conf->arguments);
    free(conf);
}

int main(void) {
    while (true) {
        printf("$ ");
        char buffer[BUFFER];
        fgets(buffer, BUFFER, stdin);
        assert_valid_input(&buffer);
        EXECUTION_CONF * conf = exec_conf_factory();
        split_into_arguments(buffer, conf);
        set_execution_type(conf);
        execute(conf);
        exec_conf_destructor(conf);
    }
}
