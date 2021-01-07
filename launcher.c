#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <assert.h>

#define BUFFER 102


void assert_valid_input(char *);
char ** split_into_arguments(char *);

void assert_valid_input(char * input) {
    len = strlen(input);
    assert(len > 2 && "Please enter a command.\n");
    assert(input[len-1] == '\n' && "Your command cant exceed 100 characters. Please try again.\n");
}
char ** split_into_arguments(char * input) {
    char ** array;
    char * token;
    while (token = strtok(input, ' '); token != NULL) {
        size_t len = strlen(token);

    }
}

int main(void) {
    char * command[BUFFER]
    printf("$ ");
    char * buffer[BUFFER];
    fgets(buffer, BUFFER, stdin);
    assert_valid_input(input);
    execvp(command, xargs);
}
