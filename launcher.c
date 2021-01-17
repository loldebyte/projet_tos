#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lol_shell.h"


int main(void) {
    while (true) {
        printf("$ ");
        char buffer[BUFFER];
        fgets(buffer, BUFFER, stdin);
        assert(validate_input(&buffer)
               && "An error was detected in your string ! Check stderr for details\n");
        EXECUTION_CONF * conf = exec_conf_factory();
        split_into_arguments(buffer, conf);
        set_execution_type(conf);
        execute(conf);
        exec_conf_destructor(conf);
    }
}
