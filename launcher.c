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
    if(len <= 1) {
        printf("Please enter a command.\n");
        perror("String length <= 1 : cannot be a command.\n");
        abort();
    }
    if(input[len-1] != '\n') {
        printf("Your command cant exceed 100 characters. Please try again.\n");
        perror("Last character in command is not \\n : input exceeds buffer length of %d.\n", BUFFER);
        abort();
    }
}
char ** split_into_arguments(char * input) {
    char * copy = malloc(strlen(input)); // might be unecessary : just reorganise the str in array and dont touch anything
    strcpy(copy, input);
    char ** array;
    char * token;
    while (token = strtok(copy, ' ') != NULL) {
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
