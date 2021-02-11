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
    // parameters : 
    //    - input : the string to be split
    //    - config : the struct stocking all parameters related to execution, including :
    //    - - arguments : array of strings (char *)
    //    - - number_of_arguments : length of the array `arguments`
    //    - - exec_type : a flag dictating how to manage execution
    // stores all tokens in the array pointed to by `output`
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

void execute(EXECUTION_CONF * config, word_hashmap ** hm) {
    // creates a child process executing the `arguments` member of the parameter by forking
    // the current process, child process gets exec'd and parent process waits
    // parameter(s) :
    // - config : the struct containing all relevant data about how/what to execute
    int status;
    if (strcmp(config->arguments[0], "exit") == 0)
        exit(0);
    if (config->exec_type == DEFINE_VARIABLE) {
        if (!save_variable(config->arguments[0], hm)) {
            perror("Could not save variable !\n");
        }
        return;
    }
    if (config->exec_type == DONT_WAIT)
        dealloc_last_argument(config);
    for (size_t k=0; k<config->number_of_arguments-1; k++) {
        assert(replace_variables_by_their_value(config->arguments[k], *hm));
    }
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
    else if (config->number_of_arguments == 1 && strchr(config->arguments[0], '=') != NULL) {
        config->exec_type = DEFINE_VARIABLE;
    }
    else config->exec_type = WAIT;
}

bool strings_are_the_same(char * arg, char * to_compare) {
    return strcmp(arg, to_compare) == 0 ? true : false;
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

bool save_variable(char * argument, word_hashmap ** hashmap) {
    /*  argument : the string containing the key/value pair
        hashmap : if NULL, create a new word_hashmap and make hashmap point towards it
        if not-null, will use the hashmap it points to */
    if (*hashmap == NULL)
        *hashmap = new_word_hashmap();
    char * key;
    int32_t value;
    bool extraction_succeeded = extract_key_value_pair(&key, &value, argument);
    if (extraction_succeeded)
        if (word_hashmap_insert(key, value, *hashmap)) {
            free(key);
            return true;
        }
    free(key);
    return false;
}

bool extract_key_value_pair(char ** key, int32_t * value, char * argument) {
    *key = strdup(strtok(argument, "="));
    char * string_value = strdup(strtok(NULL, "="));
    char * endptr;
    errno = 0;
    *value = strtol(string_value, &endptr, 10);
    if (errno == ERANGE || (errno != 0 && *value == 0)) {
        perror("extract_key_value: strtol failed\n");
        free(string_value);
        return false;
    }
    if (endptr == string_value) {
        fprintf(stderr, "No digits found in the value side !\n");
        free(string_value);
        return false;
    }
    free(string_value);
    return true;
}

bool replace_variables_by_their_value(char * argument, word_hashmap * hm) {
    char * substrings[100];
    char * keys[99];
    char * str_values[99];
    substrings[0] = strtok(argument, "$");
    assert(substrings[0] != NULL);
    char * tmp;
    int indice = 1;
    while (tmp = strtok(NULL, "$"), tmp != NULL) {
        substrings[indice] = tmp+number_of_chars_behind_cash(tmp);
        strncpy(keys[indice-1], tmp, number_of_chars_behind_cash(tmp));
        indice++;
        assert(indice<99 && "Too many variables in one argument !");
    }
    for (int k=0; k<indice; k++) {
        int32_t value = word_hashmap_search(keys[k], hm);
        int n = snprintf(NULL, 0, "%li", (long) value);
        char buf[n+1];
        int c = snprintf(buf, n+1, "%li", (long) value);
        assert(n == c);
        str_values[k] = buf;
    }
    char final_string[strlen(argument)*2+20];
    final_string[0] = '\0';
    strcat(final_string, substrings[0]);
    for (int k=0; k<indice; k++) {
        strcat(strcat(final_string, str_values[k]), substrings[k+1]);
    }
    free(argument);
    argument = strdup(final_string);
    return true;
}

int number_of_chars_behind_cash(char * str) {
    int i = 0;
    while (str[i] != ' ' && str[i] != '\0') {
        i++;
    }
    return i;
}

