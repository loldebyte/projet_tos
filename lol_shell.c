#include "lol_shell.h"


bool validate_input(char (*input)[]) {
    // parameter(s) : array of char * (strings)
    // asserts the inputed array of strings is valid,
    // and deletes the trailing \n (last char of last string)
    size_t len = strlen(*input);
    if (len <= 2) {
        fprintf(stderr, "Please enter a command\n");
        return false;
    }
    if ((*input)[len-1] != '\n') {
        fprintf(stderr, "Your command cant exceed 100 characters. Please try again.\n");
        return false;
    }
    (*input)[len-1] = 0; // the \n makes most commands fail
    return true;
}

void split_into_arguments(char * input, EXECUTION_CONF * config) {
    size_t curr_cell = 0;
    // parameters : 
    //    - input : the string to be split
    //    - config : the struct stocking all parameters related to execution, including :
    //    - - arguments : array of strings (char *)
    //    - - number_of_arguments : length of the array `arguments`
    //    - - exec_type : a flag dictating how to manage execution
    // returns the length of the `output` array as a size_t after storing all tokens
    // in the array pointed to by `output`
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
    int status;
    if (strcmp(config->arguments[0], "exit") == 0)
        exit(0);
    if (config->exec_type != WAIT)
        dealloc_last_argument(config);
    // creates a child process executing the `arguments` member of the parameter by forking
    // the current process, child process gets exec'd and parent process waits
    // parameter(s) :
    // - config : the struct containing all relevant data about how/what to execute
    pid_t pid = fork();
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
    else config->exec_type = WAIT;
}

bool strings_are_the_same(char * arg, char * to_compare) {
    int val = strcmp(arg, to_compare);
    return val == 0 ? true : false;
}

void dealloc_last_argument(EXECUTION_CONF * config) {
    config->arguments[config->number_of_arguments-1][0] = '\0';
    config->arguments[config->number_of_arguments-1] = (char *) NULL;
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
    conf->arguments = NULL;
    free(conf);
    conf = NULL;
}

void print_all_args(EXECUTION_CONF * config) {
    for (int i=0; i<config->number_of_arguments; i++)
        printf("%s\n", config->arguments[i]);
}

void print_all_chars_in_str(char * str) {
    for (int i = strlen(str)-1; i>-1; i--)
        printf("char #%d: %c\n", i, str[i]);
}