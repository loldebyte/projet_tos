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
    EXEC_FLAGS exec_type;
    size_t number_of_arguments
} EXECUTION_CONF;

void assert_valid_input(char (*)[]);
size_t split_into_arguments(char *, char ***);
void execute(char **, size_t);
EXEC_FLAGS get_execution_type(char **, size_t);
bool strings_are_the_same(char *, char *);

void assert_valid_input(char (*input)[]) {
    size_t len = strlen(*input);
    assert(len > 2 && "Please enter a command.\n");
    assert((*input)[len-1] == '\n' && "Your command cant exceed 100 characters. Please try again.\n");
    (*input)[len-1] = 0; // the \n makes most commands fail
}

size_t split_into_arguments(char * input, char *** output) {
    size_t curr_len = 0, curr_cell = 0, len;
    char * token;
    while (token = strtok(curr_cell == 0 ? input : NULL, SEP), token != NULL) {
        len = strlen(token);
        *output = realloc(*output, sizeof(*output) * (curr_cell+1));
        curr_len += len;
        *((*output)+curr_cell) = token;
        curr_cell++;
    }
    return curr_cell;
}

void execute(char ** args, size_t nb_args) {
    pid_t pid;
    int status;

    if (strcmp(args[0], "exit") == 0)
        exit(0);
    EXEC_FLAGS exec_type = get_execution_type(&args, nb_args);
    pid = fork();
    assert(pid != -1 && "Error : could not create child process");
    if (pid == 0) {
        int check_execvp = execvp(args[0], args);
        assert((check_execvp =! -1) && "Execvp failed");
    }
    else {
        if (exec_type == WAIT) {
            pid_t wait_status;
            while(wait_status = waitpid(pid, &status, 0), wait_status != pid)
                assert(wait_status != -1 && "Wait failed");
            }
        else if (exec_type == DONT_WAIT) {
            ;
        }   
        else {
            printf("NOT IMPLEMENTED !\n");
        }     
    }
}

EXEC_TYPE get_execution_type(char *** args, size_t nb_of_args) {
    if (strings_are_the_same(args[nb_of_args-1], "&")) {
        return DONT_WAIT;
    }
    return WAIT;
}

bool strings_are_the_same(char * arg, char * to_compare) {
    int val;
    val = strcmp(arg, to_compare);
    return val == 0 ? true : false;
}

int main(void) {
    while (true) {
        printf("$ ");
        char buffer[BUFFER];
        fgets(buffer, BUFFER, stdin);
        assert_valid_input(&buffer);
        char ** args = NULL;
        size_t nb_of_args = split_into_arguments(buffer, &args);
        execute(args, nb_of_args);
        free(args);
    }
}
