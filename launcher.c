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

void assert_valid_input(char * input) {
    size_t len = strlen(input);
    assert(len > 2 && "Please enter a command.\n");
    assert(input[len-1] == '\n' && "Your command cant exceed 100 characters. Please try again.\n");
}
size_t split_into_arguments(char * input, char *** output) {
    if (*output != NULL) {
        printf("output lives !");
    } else {
        printf("output's dead\n");
    }
    size_t curr_len = 0, curr_cell = 0, len;
    char * token;
    while (token = strtok(curr_cell == 0 ? input : NULL, SEP), token != NULL) {
        len = strlen(token);
        printf("STARTING realloc\n");
        if (*output != NULL) {
            printf("output lives !\n");
        } else {
            printf("output's dead\n");
        }
        *output = (char **) realloc(*output, sizeof(*output) * (curr_cell+1));
        if (*output != NULL) {
            printf("output lives after realloc!\n");
        } else {
            printf("output's dead after realloc\n");
        }
        printf("previous len: %ld\n", curr_len);
        curr_len += len;
        printf("new len: %ld\n", curr_len);
        *output[curr_cell] = (char *) malloc(len);
        *output[curr_cell] = token;
        curr_cell++;
    }
    return curr_cell;
}

int main(void) {
    printf("$ ");
    char buffer[BUFFER];
    fgets(buffer, BUFFER, stdin);
    assert_valid_input(buffer);
    char ** args = NULL;
    size_t nb_of_args = split_into_arguments(buffer, &args);
    // execvp(args[0], &args[1]);
    for (int i=0; i<nb_of_args; i++) {
        printf("str #%d: %s", i, args[i]);
        free(args[i]);
    }
    if (args != NULL) {
        printf("it lives !\n");
        free(args);
    } else {
        printf("it's dead\n");
    }
    printf("nb args : %ld\n", nb_of_args);
}
