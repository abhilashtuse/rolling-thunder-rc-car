#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

using namespace cgreen;

// The file under test
#include "master_controller.hpp"

Ensure(test_update_motor)
{
	SENSOR_DATA_t sensor_data;
	MOTOR_UPDATE_t motor_data;
	motor motor;
	
	// Turn left
	sensor_data.SENSOR_middle_sensor = 100;
	sensor_data.SENSOR_left_sensor = 100;
	sensor_data.SENSOR_right_sensor = 6;
	sensor_data.SENSOR_left_sensor = 100;
    motor.update_motor(sensor_data, &motor_data);
    assert_equal(motor_data.MOTOR_speed, 1.5);
	assert_equal(motor_data.MOTOR_turn_angle, -20);
	
	// Go straight
	sensor_data.SENSOR_middle_sensor = 100;
	sensor_data.SENSOR_left_sensor = 100;
	sensor_data.SENSOR_right_sensor = 100;
	sensor_data.SENSOR_left_sensor = 100;
    motor.update_motor(sensor_data, &motor_data);
    assert_equal(motor_data.MOTOR_speed, 1.5);
	assert_equal(motor_data.MOTOR_turn_angle, 0);
	
	// stop and reverse
	sensor_data.SENSOR_middle_sensor = 25;
	sensor_data.SENSOR_left_sensor = 100;
	sensor_data.SENSOR_right_sensor = 100;
	sensor_data.SENSOR_left_sensor = 100;
    motor.update_motor(sensor_data, &motor_data);
    assert_equal(motor_data.MOTOR_speed, -2);
	assert_equal(motor_data.MOTOR_turn_angle, 0);
	
}

TestSuite *master_controller_test_suite()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, test_update_motor);
    return suite;
}
