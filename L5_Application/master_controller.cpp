#include "master_controller.hpp"
#include <stdio.h>

#define FORWARD_SPEED   (1.5)
#define REVERSE_SPEED   (-1.5)
#define TURN_ANGLE      (25)

#define front_threshold   50
#define left_threshold    20
#define right_threshold   20
#define back_threshold    25

void motor::update_motor(SENSOR_DATA_t sensor_data, MOTOR_UPDATE_t *motor_update)
{
    if((sensor_data.SENSOR_middle_sensor < front_threshold) || (sensor_data.SENSOR_left_sensor < left_threshold) || (sensor_data.SENSOR_right_sensor < right_threshold))
    {
        if(sensor_data.SENSOR_left_sensor < left_threshold)
        {
            // Take right
            motor_update->MOTOR_speed = FORWARD_SPEED;
            motor_update->MOTOR_turn_angle = TURN_ANGLE;
        }
        else if(sensor_data.SENSOR_right_sensor < right_threshold)
        {
            // Take left
            motor_update->MOTOR_speed = FORWARD_SPEED;
            motor_update->MOTOR_turn_angle = -TURN_ANGLE;
        }
        else if(sensor_data.SENSOR_middle_sensor > 30 && sensor_data.SENSOR_middle_sensor < front_threshold)
        {
            // Slow down
            motor_update->MOTOR_speed = FORWARD_SPEED;
            motor_update->MOTOR_turn_angle = (sensor_data.SENSOR_right_sensor > sensor_data.SENSOR_left_sensor)? TURN_ANGLE: -TURN_ANGLE;
        }
        else if(sensor_data.SENSOR_middle_sensor <= 30 && sensor_data.SENSOR_back_sensor > back_threshold)
        {
            // Stop & Reverse
            motor_update->MOTOR_speed = REVERSE_SPEED;
            motor_update->MOTOR_turn_angle = (sensor_data.SENSOR_right_sensor > sensor_data.SENSOR_left_sensor)? TURN_ANGLE: -TURN_ANGLE;
        }
    }
    else
    {
        motor_update->MOTOR_speed = FORWARD_SPEED;
        motor_update->MOTOR_turn_angle = 0;
    }
}
