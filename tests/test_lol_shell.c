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
        int len_before = strlen(valid_string);
        assert(validate_input(&valid_string) && "String validation of valid string failed\n");
        assert(valid_string[len_before-1] == 0 && "trailing \\n not removed !\n");
        {
            int len_after = strlen(valid_string);
            assert(len_before == (len_after+1)
                   && "validate_input alters too much of the string !\n");
        }
    }
    freopen("/dev/null", "w", stderr);
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

int main(void) {
    assert(test_validate_input() && "validate_input test failing"); 
}
