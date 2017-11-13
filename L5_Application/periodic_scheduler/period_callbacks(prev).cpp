/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "josh_main.hpp"
#include "eint.h"
#include "gpio.hpp"
#include <stdio.h>
#include "uart3.hpp"
#include "lpc_pwm.hpp"
#include "can.h"
#include "motor_controller/motor.hpp"

/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_MONITOR_TASK_STACK_SIZE_BYTES = (512 * 3);

GPIO pin2_0(P2_0);

//PWM motor(PWM::pwm1, 100);

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
	bool rc;
	MOTOR = get_motor_pwm(PWM::pwm1);
	SERVO = get_servo_pwm(PWM::pwm2);
	//motor.set(15.0);
	//Enable CAN bus 1
	//bool CAN_init(can_t can, uint32_t baudrate_kbps, uint16_t rxq_size, uint16_t txq_size,
	    //          can_void_func_t bus_off_cb, can_void_func_t data_ovr_cb);
	//rc = CAN_init(can1, 500, 20, 20, NULL, NULL);
	//printf("CAN init rc %d\n", rc);
	//CAN_bypass_filter_accept_all_msgs();
	//CAN_reset_bus(can1);

	//Enable PWM
	//P2.0 for Motor driver
	//PWM motor(PWM::pwm1, 100);
	//P2.1 for left/right
	//PWM steer(PWM::pwm2, 100);

	//Initialize uart3
    //Uart3& u3 = Uart3::getInstance();
    //u3.init(38400, 20, 20); /* Init baud rate */

	//Enable interrupt for GPIO P2.2
	//pin2_0.setAsInput();
	//pin2_0.enablePullDown();
	//eint3_enable_port2(2, eint_rising_edge, switch_interrupts_hdlr_led);
    return true; // Must return true upon success
}

void p2_enable_led(void)
{
	//if GPIO P2.0 is high, enable LED P2.1
	//GPIO pin2_0(P2_0);
	if (pin2_0.read() == 1)
	{
		printf("p2 is high");
		LE.on(1);
	} else {
		LE.off(1);
	}
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}

void send_simple_can(void)
{
	can_msg_t can_tx_msg;
	bool rc;
	can_tx_msg.msg_id = 0x122;
	can_tx_msg.frame_fields.is_29bit = 0;
	can_tx_msg.frame_fields.data_len = 1;       // Send 8 bytes

	if (SW.getSwitch(1))
	{
		can_tx_msg.data.bytes[0] = 0xAA;
		//If switch is pressed, send 0xAA
		rc = CAN_tx(can1, &can_tx_msg, portMAX_DELAY);
		printf("sent message, rc, %d\n", rc);
	} else {
		can_tx_msg.data.bytes[0] = 0x00;
		//If switch not pressed, send 0x00
		CAN_tx(can1, &can_tx_msg, portMAX_DELAY);
	}
}

void rx_simple_can(void)
{
	bool rc;
	can_msg_t can_rx_msg;
	rc = CAN_rx(can1, &can_rx_msg, 0x50);
	if (can_rx_msg.data.bytes[0] == 0xAA)
	{
		printf("Received data 0xAA on CAN\n");
		LE.on(1);
	} else if (can_rx_msg.data.bytes[0] == 0xAA) {
		printf("received data %x\n", can_rx_msg.data.bytes[0]);
	} else {
		LE.off(1);
	}
}

/**
 * Below are your periodic functions.
 * The argument 'count' is the number of times each periodic task is called.
 */
float duty_cycle=15;
void period_1Hz(uint32_t count)
{
	//If CAN bus turns off, re-enable it
//	if (CAN_is_bus_off(can1))
//	{
//		printf("Can bus is off\n");
//		CAN_reset_bus(can1);
//	}

    //LE.toggle(1);
    //Check on P2.0 to enable/disable LED 1
    //p2_enable_led();
    //Send hello world over uart3
    //uart3_task();
	//stop_car();
//	if (count < 6)
//	{
//		stop_car();
//	}
	//1 second - > go forward
//	if (count == 6)
//	{
//		printf("go forward\n");
//		motor->set(16.0);
//	}
//	if (count == 10)
//	{
//		printf("reset\n");
//		motor->set(15.0);
//	}
	if (count == 5)
	{
		duty_cycle = 10;
		printf("go backward\n");
		//motor->set(duty_cycle);
		SERVO->set(duty_cycle);
	}
	if (count == 10)
	{
		duty_cycle = 15.0;
//		if (duty_cycle != 15.0)
//		{
//			duty_cycle -= 0.5;
//		}
		printf("reset\n");
		//motor->set(duty_cycle);
		SERVO->set(duty_cycle);
	}
	if (count == 15)
	{
		duty_cycle = 19.9;
		SERVO->set(duty_cycle);
	}
	if (count == 20)
	{
		duty_cycle = 15.0;
		SERVO->set(duty_cycle);
	}
//	//1 second - > go reverse
//	if (count == 7)
//		go_reverse(0);
//	//1 second - > break
//	if (count == 8)
//		stop_car();
//	//1 second -> turn left
//	if (count == 9)
//		go_left(0);
//	//1 second -> turn right
//	if (count == 10)
//		go_right(0);
//	if (count == 11)
//		stop_car();
}

void period_10Hz(uint32_t count)
{
	//send_simple_can();
	//rx_simple_can();
    //LE.toggle(2);
}

void period_100Hz(uint32_t count)
{
	//switch_drive_p2();
    //LE.toggle(3);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    LE.toggle(4);
}