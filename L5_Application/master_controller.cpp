#include "master_controller.hpp"
#include <stdio.h>

#define FORWARD_SPEED   (1.5)
#define REVERSE_SPEED   (-1.5)
#define TURN_ANGLE      (25)

#define front_threshold   65
#define left_threshold    20
#define right_threshold   20
#define back_threshold    25

bool keep_reversing_middle = 0;
bool keep_reversing_lr = 0;

void motor::update_motor(SENSOR_DATA_t sensor_data, MOTOR_UPDATE_t *motor_update, float actual_speed)
{
    if((sensor_data.SENSOR_middle_sensor < front_threshold) || (sensor_data.SENSOR_left_sensor < left_threshold) || (sensor_data.SENSOR_right_sensor < right_threshold))
    {
        if(sensor_data.SENSOR_left_sensor < left_threshold && sensor_data.SENSOR_right_sensor < right_threshold)
        {
            if(actual_speed && !keep_reversing_lr)
            {
                motor_update->MOTOR_speed = 0;
            }
            else
            {
                motor_update->MOTOR_speed = REVERSE_SPEED;
                keep_reversing_lr = 1;
            }
            motor_update->MOTOR_turn_angle = TURN_ANGLE;
        }
        if((sensor_data.SENSOR_left_sensor < left_threshold || sensor_data.SENSOR_right_sensor < right_threshold) && sensor_data.SENSOR_middle_sensor < 35)
        {
            if(sensor_data.SENSOR_left_sensor < left_threshold)
            {
                motor_update->MOTOR_turn_angle = TURN_ANGLE;
            }
            else if(sensor_data.SENSOR_right_sensor < right_threshold)
            {
                motor_update->MOTOR_turn_angle = -TURN_ANGLE;
            }
            motor_update->MOTOR_speed = FORWARD_SPEED;
            keep_reversing_lr = 0;
            keep_reversing_middle = 0;
        }
        if(sensor_data.SENSOR_left_sensor < left_threshold)
        {
            // Take right
            motor_update->MOTOR_speed = FORWARD_SPEED;
            motor_update->MOTOR_turn_angle = TURN_ANGLE;
            keep_reversing_lr = 0;
            keep_reversing_middle = 0;
        }
        else if(sensor_data.SENSOR_right_sensor < right_threshold)
        {
            // Take left
            motor_update->MOTOR_speed = FORWARD_SPEED;
            motor_update->MOTOR_turn_angle = -TURN_ANGLE;
            keep_reversing_lr = 0;
            keep_reversing_middle = 0;
        }
        else if(sensor_data.SENSOR_middle_sensor > 35 && sensor_data.SENSOR_middle_sensor < front_threshold)
        {
            // Slow down
            motor_update->MOTOR_speed = FORWARD_SPEED;
            if(sensor_data.SENSOR_left_sensor > 40 && sensor_data.SENSOR_right_sensor < 40)
            {
                motor_update->MOTOR_turn_angle = TURN_ANGLE;
            }
            else if(sensor_data.SENSOR_right_sensor > 40 && sensor_data.SENSOR_left_sensor < 40)
            {
                motor_update->MOTOR_turn_angle = -TURN_ANGLE;
            }
            else if(sensor_data.SENSOR_left_sensor < 40 && sensor_data.SENSOR_right_sensor < 40)
            {
                motor_update->MOTOR_turn_angle = (sensor_data.SENSOR_left_sensor > sensor_data.SENSOR_right_sensor)? TURN_ANGLE : -TURN_ANGLE;
            }
            else
                motor_update->MOTOR_turn_angle = TURN_ANGLE;

            keep_reversing_lr = 0;
            keep_reversing_middle = 0;
        }
        else if(sensor_data.SENSOR_middle_sensor <= 35 && sensor_data.SENSOR_back_sensor > back_threshold)
        {
            // Stop & Reverse
            if(actual_speed && !keep_reversing_middle)
            {
                motor_update->MOTOR_speed = 0;
            }
            else
            {
                motor_update->MOTOR_speed = REVERSE_SPEED;
                keep_reversing_middle = 1;
            }

            motor_update->MOTOR_turn_angle = TURN_ANGLE;
        }
    }
    else
    {
        motor_update->MOTOR_speed = FORWARD_SPEED;
        motor_update->MOTOR_turn_angle = 0;
        keep_reversing_lr = 0;
        keep_reversing_middle = 0;
    }
}
