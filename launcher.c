#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <assert.h>
#include <unistd.h>

#define SEP " "
#define BUFFER 102


void assert_valid_input(char *);
size_t split_into_arguments(char *, char ***);
void execute(char **);

void assert_valid_input(char * input) {
    size_t len = strlen(input);
    assert(len > 2 && "Please enter a command.\n");
    assert(input[len-1] == '\n' && "Your command cant exceed 100 characters. Please try again.\n");
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

void execute(char ** args) {
    pid_t pid;
    int status;

    pid = fork();
    assert(pid != -1 && "Error : could not create child process");
    if (pid == 0) {
        assert(execvp(args[0], args) && "Could not execute command in child process");
    }
    else {
        pid_t wait_status;
        while(wait_status = waitpid(pid, &status, 0), wait_status != pid)
            if (wait_status == -1) {
                perror("Wait failed");
                exit(1);
            }
    }
}

int main(void) {
    printf("$ ");
    char buffer[BUFFER];
    fgets(buffer, BUFFER, stdin);
    assert_valid_input(buffer);
    char ** args = NULL;
    size_t nb_of_args = split_into_arguments(buffer, &args);
    nb_of_args += 0; // this is to keep Wall quiet
    execute(args);
    free(args);
}
