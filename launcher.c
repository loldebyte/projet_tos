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


void assert_valid_input(char (*)[]);
size_t split_into_arguments(char *, char ***);
void execute(char **);

void assert_valid_input(char (*input)[]) {
    // parameter(s) : array of char * (strings)
    // asserts the inputed array of strings is valid,
    // and deletes the trailing \n (last char of last string)
    size_t len = strlen(*input);
    assert(len > 2 && "Please enter a command.\n");
    assert((*input)[len-1] == '\n' && "Your command cant exceed 100 characters. Please try again.\n");
    (*input)[len-1] = 0; // the \n makes most commands fail
}

size_t split_into_arguments(char * input, char *** output) {
    // parameters : 
    //    - input : the string to be split
    //    - output : the adress of the string array in which our tokens will be stored
    // returns the length of the `output` array as a size_t after storing all tokens
    // in the array pointed to by `output`
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
    // parameter(s) : creates a child process executing th contents of the `args` parameter
    // forks the current process, child process gets exec'd and parent process waits
    int status;
    if (strcmp(args[0], "exit") == 0)
        exit(0);
    pid_t pid = fork();
    assert(pid != -1 && "Error : could not create child process");
    if (pid == 0) {
        int check_execvp = 0;
        check_execvp = execvp(args[0], args);
        assert(check_execvp == 0 && "Execvp failed");
    }
    else {
        pid_t wait_status;
        while(wait_status = waitpid(pid, &status, 0), wait_status != pid)
            assert(wait_status != -1 && "Wait failed");
    }
}

int main(void) {
    while (true) {
        printf("$ ");
        char buffer[BUFFER];
        fgets(buffer, BUFFER, stdin);
        assert_valid_input(&buffer);
        char ** args = NULL;
        size_t nb_of_args = split_into_arguments(buffer, &args);
        nb_of_args += 0; // this is to keep Wall quiet
        execute(args);
        free(args);
    }
}
