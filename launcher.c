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
void free_args(char ***, size_t);

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
        printf("previous len: %ld\n", curr_len);
        curr_len += len;
        printf("new len: %ld\n", curr_len);
        *((*output)+curr_cell) = malloc(len);
        *((*output)+curr_cell) = token;
        curr_cell++;
    }
    return curr_cell;
}

void free_args(char *** args, size_t nb_args) {
    for (int i = nb_args-1; i>-1; i--) {
        printf("str #%d: %s\n", i, (*args)[i]);
        free((*args)[i]);
    }
    free(*args);
}

int main(void) {
    printf("$ ");
    char buffer[BUFFER];
    fgets(buffer, BUFFER, stdin);
    assert_valid_input(buffer);
    char ** args = NULL;
    size_t nb_of_args = split_into_arguments(buffer, &args);
    // execvp(args[0], &args[1]);
    printf("nb args : %ld\n", nb_of_args);
    free_args(&args, nb_of_args);
}
