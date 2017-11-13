/*
 * sensor.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: Thrishna
 */

#include "lpc_timers.h"
#include "eint.h"
#include "utilities.h"
#include "printf_lib.h"
#include "sensor.hpp"

#define DISTANCE_FACTOR 147

uint64_t time_start_left = 0;
uint64_t time_start_middle = 0;
uint64_t time_start_right = 0;
uint64_t time_start_back = 0;

uint8_t distance_left = 0;
uint16_t distance_middle = 0;
uint16_t distance_right = 0;
uint8_t distance_back = 0;

//Set sensor_gpio as output pin
void set_sensor_pin_as_output(GPIO &sensor_gpio)
{
    sensor_gpio.setAsOutput();
}

//Set sensor_gpio as input pin
void set_sensor_pin_as_input(GPIO &sensor_gpio)
{
    sensor_gpio.setAsInput();
}

//pin must be set as output_pin prior to this method
// The sensor is triggered by a HIGH pulse of 5 microseconds(for parallax).
//The sensor is triggered by a HIGH pulse of 25 microseconds(for Maxbotix).
// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
void trigger_sensor(GPIO &sensor_gpio, bool isParallax)
{
    if (isParallax) {
        sensor_gpio.setLow();
        delay_us(2);
        sensor_gpio.setHigh();
        delay_us(5);
        sensor_gpio.setLow();
    }
    else {
        sensor_gpio.setLow();
        delay_us(2);
        sensor_gpio.setHigh();
        delay_us(25);
        sensor_gpio.setLow();
    }

}

//Callback function for rising edge interrupt on the pin for left sensor
//Snapshot of the timer is taken at rising edge
void start_time_left_sensor(void)
{
    time_start_left = sys_get_uptime_us();
}

//Callback function for falling edge interrupt on the pin for left sensor
//Snapshot of the timer is taken at falling edge and the distance is calculated
void final_time_left_sensor(void)
{

    uint64_t time_stop_left = sys_get_uptime_us();
    distance_left = (time_stop_left - time_start_left) / DISTANCE_FACTOR;

}

//Callback function for rising edge interrupt on the pin for middle sensor
//Snapshot of the timer is taken at rising edge
void start_time_middle_sensor(void)
{
    time_start_middle = sys_get_uptime_us();
}

//Callback function for falling edge interrupt on the pin for middle sensor
//Snapshot of the timer is taken at falling edge and the distance is calculated
void final_time_middle_sensor(void)
{
    uint64_t time_stop_middle = sys_get_uptime_us();
    distance_middle = (time_stop_middle - time_start_middle) / DISTANCE_FACTOR;
}

//Callback function for rising edge interrupt on the pin for back sensor
//Snapshot of the timer is taken at rising edge
void start_time_back_sensor(void)
{
    time_start_back = sys_get_uptime_us();
}

//Callback function for falling edge interrupt on the pin for back sensor
//Snapshot of the timer is taken at falling edge and the distance is calculated
void final_time_back_sensor(void)
{
    uint64_t time_stop_back = sys_get_uptime_us();
    distance_back = (time_stop_back - time_start_back) / DISTANCE_FACTOR;
}

//Callback function for rising edge interrupt on the pin for right sensor
//Snapshot of the timer is taken at rising edge
void start_time_right_sensor(void)
{
    time_start_right = sys_get_uptime_us();
}

//Callback function for falling edge interrupt on the pin for right sensor
//Snapshot of the timer is taken at falling edge and the distance is calculated
void final_time_right_sensor(void)
{
    uint64_t time_stop_right = sys_get_uptime_us();
    distance_right = (time_stop_right - time_start_right) / DISTANCE_FACTOR;
}

//Enables external interrupt for rising and falling edge on pin_num
void enable_interrupt(uint8_t pin_num)
{
    if (4 == pin_num) //left sensor
            {
        eint3_enable_port2(pin_num, eint_rising_edge, start_time_left_sensor);
        eint3_enable_port2(pin_num, eint_falling_edge, final_time_left_sensor);
    }
    else if (2 == pin_num) //right sensor
            {
        eint3_enable_port2(pin_num, eint_rising_edge, start_time_right_sensor);
        eint3_enable_port2(pin_num, eint_falling_edge, final_time_right_sensor);
    }
    else if (3 == pin_num) //middle sensor
            {
        eint3_enable_port2(pin_num, eint_rising_edge, start_time_middle_sensor);
        eint3_enable_port2(pin_num, eint_falling_edge, final_time_middle_sensor);
    }
    else if (5 == pin_num) //back sensor
            {
        eint3_enable_port2(pin_num, eint_rising_edge, start_time_back_sensor);
        eint3_enable_port2(pin_num, eint_falling_edge, final_time_back_sensor);
    }

}

