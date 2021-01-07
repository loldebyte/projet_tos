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
    char ** array = (char **) NULL;
    size_t curr_len = 0, curr_cell = 0;
    while (token = strtok(input, ' '); token != NULL) {
        size_t len = strlen(token);
        array = (char **) realloc(array, curr_len + len);
        curr_len += len;
        array[curr_cell] = token;
        curr_cell++;
    }
    return array;
}

int main(void) {
    printf("$ ");
    char * buffer[BUFFER];
    fgets(buffer, BUFFER, stdin);
    assert_valid_input(buffer);
    char ** args = split_into_arguments(buffer);
    excecvp(args[0], &args[1]);
}
