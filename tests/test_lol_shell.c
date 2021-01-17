#include "../lol_shell.h"


bool test_validate_input(void);
bool test_split_into_arguments(void);
bool test_set_execution_type(void);
bool test_strings_are_the_same(void);
bool test_exec_conf_factory(void);
bool test_dealloc_last_argument(EXECUTION_CONF *);
bool test_execute(EXECUTION_CONF *);

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
        char too_long_string[BUFFER] = "Vivamus pellentesque luctus mauris, sed convallis ligula. Nulla fermentum massa vitae scelerisque ac."; // thanks to lipsum.com !
        assert(validate_input(&too_long_string) == false
               && "Invalid string didnt fail !");
    }
}

bool test_exec_conf_factory(void) {
    EXECUTION_CONF * conf = exec_conf_factory();
    assert(conf->number_of_arguments == 0 && "nb args initialization failing !");
    assert(conf->exec_type == WAIT && "exec_type initialization failing !");
    exec_conf_destructor(conf);
    return true;
}

bool test_strings_are_the_same(void) {
    {
        char * identic_strings1 = "str1", identic_strings2 = "str1";
        assert(strings_are_the_same(identic_strings1,
                                    identic_strings2)
               && "identical strings return false !");
    }
    {
        char * str1 = "str1", str2 = "str2";
        assert(strings_are_the_same(str1, str2) == false
               && "dissimilar strings return true !");
    }
}

bool test_split_into_arguments(void) {
    {
        EXECUTION_CONF * conf = exec_conf_factory();
        char * single_arg_input = "echo";
        split_into_arguments(single_arg_input, conf);
        assert(conf->number_of_arguments == 1 && "number_of_arguments has incorrect value !");
        assert(strings_are_the_same(conf->arguments[0], single_arg_input)
               && "args members badly set !");
        assert(conf->exec_type == WAIT && "exec_type modified !");
    }
    {
        EXECUTION_CONF * conf = exec_conf_factory();
        char * two_arg_input = "echo arg2";
        split_into_arguments(two_arg_input, conf);
        assert(conf->number_of_arguments == 2 && "number_of_arguments has incorrect value !");
        assert(test_strings_are_the_same(conf->arguments[0], "echo")
               && "args' 1rst member badly set !");
        assert(test_strings_are_the_same(conf->arguments[1], "arg2")
               && "args' 2nd member badly set !");
        assert(conf->exec_type == WAIT && "exec_type modified !");
    }
    return true;
}

bool test_set_execution_type(void) {
    {
        EXECUTION_CONF * conf_no_wait = exec_conf_factory();
        char * no_wait = "echo &";
        split_into_arguments(no_wait, conf_no_wait);
        set_execution_type(conf_no_wait);
        assert(conf_no_wait->exec_type == DONT_WAIT
               && "set_execution_type doesnt set to DONT_WAIT");
    }
    {
        EXECUTION_CONF * conf_wait = exec_conf_factory();
        char * wait = "echo";
        split_into_arguments(wait, conf_wait);
        set_execution_type(conf_wait);
        assert(conf_wait->exec_type == WAIT
               && "set_execution_type incorrectly modifies WAIT");
    }
}

int main(void) {
    assert(test_validate_input() && "validate_input test failing !");
    assert(test_strings_are_the_same() && "strings_are_the_same test failing !");
    assert(test_exec_conf_factory() && "exec_conf_factory test failing !");
    assert(test_split_into_arguments() && "split_into_args test failing !");
    assert(test_set_execution_type() && "set_execution_type test failing !");
    printf("----- OK : ALL TEST PASS ! -----\n");
}
