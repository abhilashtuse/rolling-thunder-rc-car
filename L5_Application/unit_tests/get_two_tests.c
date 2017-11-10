#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include "testfuncs.h"

Describe(GetTwoDigits);
BeforeEach(GetTwoDigits){}
AfterEach(GetTwoDigits){}

Ensure(GetTwoDigits, null_test){
    char *temp = strdup("");
    uint8_t ans = get_two(temp);
    assert_that(ans, is_equal_to(0));
    free(temp);
}

Ensure(GetTwoDigits, negative_test){
    char *temp = strdup("1101");
    uint8_t ans = get_two(temp);
    assert_that(ans, is_equal_to(11));
    free(temp);
}

Ensure(GetTwoDigits, zero_test){
    char *temp = strdup("00");
    uint8_t ans = get_two(temp);
    assert_that(ans, is_equal_to(0));
    free(temp);
}

Ensure(GetTwoDigits, less_than_ten_test){
    char *temp = strdup("09");
    uint8_t ans = get_two(temp);
    assert_that(ans, is_equal_to(9));
    free(temp);
}

Ensure(GetTwoDigits, expected_input_test){
    char *temp = strdup("11");
    uint8_t ans = get_two(temp);
    assert_that(ans, is_equal_to(11));
    free(temp);
}

TestSuite *gt_tests(){
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, GetTwoDigits, null_test);
    add_test_with_context(suite, GetTwoDigits, negative_test);
    add_test_with_context(suite, GetTwoDigits, zero_test);
    add_test_with_context(suite, GetTwoDigits, less_than_ten_test);
    add_test_with_context(suite, GetTwoDigits, expected_input_test);
    return suite;
}
/*
* Functions to be tested
*/

uint8_t get_two(char *ptr)
{
    uint8_t n_c = 0;
    int    i = 0;

    while (ptr && *ptr && i < 2)
    {
        n_c *= 10;
        n_c += ptr[i] - '0';
        i++;
    }
    return (n_c);
}