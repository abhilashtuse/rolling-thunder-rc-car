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
#include <stdio.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "gpio.hpp"
#include "rt.h"
/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);
int flag = 0;
char mail[20];
bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}

void    start_car(float latitude, float longitude, int start)
{
    BRIDGE_START_STOP_t start_stop = {0};
    start_stop.BRIDGE_START_STOP_cmd = start;
    start_stop.BRIDGE_CHECKPOINT_latitude = latitude;
    start_stop.BRIDGE_CHECKPOINT_longitude = longitude;
    start_stop.BRIDGE_COORDINATE_READY = 1;
    start_stop.BRIDGE_FINAL_COORDINATE = 1;

    dbc_encode_and_send_BRIDGE_START_STOP(&start_stop);
}

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{

    CAN_init(can1, 100, 20, 20, NULL, NULL);
    CAN_reset_bus(can1);
    CAN_bypass_filter_accept_all_msgs();
    /*
     * Bluetooth Connection
     * Baud rate of device used is 115200
     */
    Uart3 &u3 = Uart3::getInstance();
	u3.init(115200);
    // LS.init();./
    return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}

/**
 * Below are your periodic functions.
 * The argument 'count' is the number of times each periodic task is called.
 */
void period_1Hz(uint32_t count)
{
    if (CAN_is_bus_off(can1))
        CAN_reset_bus(can1);
    // bridge_heartbeat();
}

void period_10Hz(uint32_t count)
{
    
    if (flag==0){
       bzero(mail, 20);
    }
    // static float latitude = 0;
    // static float longitude = 0;
    Uart3 &u3 = Uart3::getInstance();
    char temp[2];
    bool success = false;
    success = u3.getChar(temp, 0);
    LE.toggle(3);
    if(success){
        LE.toggle(1);
        strcat(mail,temp);
        flag+=1;
    }
    if (flag > 0 && !success)
    {
        mail[flag] = '\0';
        printf("Full payload: %s\n", mail);
        if (mail[0] == 'a'){
            if (mail[1] == '0')
            {
                LE.toggle(2);
                start_car(0,0,0);
            }
            else
            {
                LE.toggle(4);
                start_car(1,1,1);
            }
        }
        // else if (mail[0] - '0' == CAR_STOP){
        //     printf("Car Stopped\n");
        //     start_car(0, 0, 0);
        // }
        // COORD_TURN_LEFT 0x05
        // COORD_TURN_RIGHT 0x06
        // COORD_REDUCE_SPEED 0x07
        flag = 0;
    }
}

void period_100Hz(uint32_t count)
{
    // LE.toggle(3);
}

void period_1000Hz(uint32_t count)
{
    // LE.toggle(4);
}
