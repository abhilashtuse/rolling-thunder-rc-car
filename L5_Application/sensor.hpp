/*
 * sensor.hpp
 *
 *  Created on: Oct 19, 2017
 *      Author: Thrishna
 *
 */

#ifndef SENSOR_HPP_
#define SENSOR_HPP_

#include "gpio.hpp"

void set_sensor_pin_as_output(GPIO &sensor_gpio);
void set_sensor_pin_as_input(GPIO &sensor_gpio);
void trigger_sensor(GPIO &sensor_gpio,bool isParallax);
void enable_interrupt(uint8_t pin_num);
void enable_timer(void);
void time_falling_edge_callback(void);


#endif /* SENSOR_HPP_ */
