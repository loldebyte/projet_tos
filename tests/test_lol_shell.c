#include "../lol_shell.h"


bool test_validate_input(void);
bool test_split_into_arguments(char *, EXECUTION_CONF *);
bool test_execute(EXECUTION_CONF *);
bool test_set_execution_type(EXECUTION_CONF *);
bool test_strings_are_the_same(EXECUTION_CONF *);
bool test_exec_conf_factory(void);
bool test_exec_conf_destructor(EXECUTION_CONF *);
bool test_dealloc_last_argument(EXECUTION_CONF *);

bool test_validate_input(void) {
    {
        char valid_string[BUFFER] = "my test string\n";
        assert(validate_input(&valid_string) && "String validation of valid string failed\n");
        int len = strlen(valid_string);
        assert(valid_string[len-1] == 0 && "trailing \\n not removed !\n");
    }
    {
        char too_short_string[BUFFER] = "a\n";
        assert(validate_input(&too_short_string) == false
               && "Invalid string didnt fail !");
    }
    {
        char too_long_string[BUFFER] = "Vivamus pellentesque luctus mauris, sed convallis ligula. Nulla fermentum massa vitae scelerisque ac.";
        assert(validate_input(&too_long_string) == false
               && "Invalid string didnt fail !");
    }
}
