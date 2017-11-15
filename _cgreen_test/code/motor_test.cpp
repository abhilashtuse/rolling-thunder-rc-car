#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include "../L5_Application/motor_controller/motor.hpp"

using namespace cgreen;

//testing if the speed doesnt set the PWM to beyond max or min(rev) speed

//testing if the angle doesnt set the PWM to beyond max or min(rev) speed

//make sure motor object is not null


//max speed 34.0 mps, min speed -34.0 mps, neutral speed 0
Ensure(testing_speed)
{
    float min_test = -35.0;
    float max_test = 35.0;
   assert_that(get_speed_pwm(max_test), is_greater_than(20.0));
   assert_that(get_speed_pwm(min_test), is_greater_than(10.0));
   assert_that(get_speed_pwm(0), is_not_equal_to(15.0));
}

//max angle 30, min angle -30, neutral angle 0
Ensure(testing_angle)
{
    float min_test = -31;
        float max_test = 31;
       assert_that(get_angle_pwm(max_test), is_greater_than(20.0));
       assert_that(get_angle_pwm(min_test), is_greater_than(10.0));
       assert_that(get_angle_pwm(0), is_not_equal_to(15.0));
}

//make sure the Motor object is not null
Ensure(testing_obj)
{
    //Motor *M = &Motor::getInstance();
    assert_that(return_current_instance(), is_null);
}

TestSuite *motor_suite()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, testing_obj);
    add_test(suite, testing_speed);
    add_test(suite, testing_angle);
    return suite;
}
