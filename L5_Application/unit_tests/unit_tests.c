#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include "testfuncs.h"
Describe(Cgreen);
BeforeEach(Cgreen){}
AfterEach(Cgreen){}

// TestSuite *gt_tests();
TestSuite *p10_test();

int main(int ac, char **ag)
{
    TestSuite *suite = create_test_suite();
    // add_suite(suite, gt_tests());
    add_suite(suite, p10_test());
    // BRIDGE_START_STOP_t ans = period_ten_test("c10091134.321453-121.987654321");
    // period_ten_test("c10091134.321453-121.987654321");
    return run_test_suite(suite, create_text_reporter());
}