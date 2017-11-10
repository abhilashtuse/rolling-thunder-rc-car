#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include "testfuncs.h"
Describe(Cgreen);
BeforeEach(Cgreen){}
AfterEach(Cgreen){}

// Ensure(Cgreen, passes_test){
//     assert_that(1 == 1);
// }
// Ensure(Cgreen, fails_test){
//     assert_that(0 == 1);
// }

TestSuite *gt_tests();

int main(int ac, char **ag)
{
    TestSuite *suite = create_test_suite();
    add_suite(suite, gt_tests());
    // add_test_with_context(suite, Cgreen, passes_test);
    // add_test_with_context(suite, Cgreen, fails_test);
    return run_test_suite(suite, create_text_reporter());
}