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
#include "can.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "uart3.hpp"
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
// This method needs to be defined once, and AGC will call it for all dbc_encode_and_send_FOO() functions
bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}
void    start_car()
{
    BRIDGE_START_STOP_t start_stop = {0};
    start_stop.BRIDGE_START_STOP_cmd = 1;
    start_stop.BRIDGE_CHECKPOINT_latitude = -37.00;
    start_stop.BRIDGE_CHECKPOINT_longitude = 122.09;

    dbc_encode_and_send_BRIDGE_START_STOP(&start_stop);
}

void    bridge_heartbeat()
{
    BRIDGE_HB_t heartbeat = {0};
    heartbeat.BRIDGE_heartbeat = 1;
    dbc_encode_and_send_BRIDGE_HB(&heartbeat);
}
/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{

    CAN_init(can1, 100, 20, 20, NULL, NULL);
    CAN_reset_bus(can1);

    Uart3 &u3 = Uart3::getInstance();
	u3.init(38400);
    // LS.init();
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
    bridge_heartbeat();
    // //

    
}

void period_10Hz(uint32_t count)
{
    // LAB_TEST_t lab_test = { 0 };
    // // BRIDGE_START_STOP_t 
    // lab_test.LAB_TEST_basic = LS.getPercentValue();
    // dbc_encode_and_send_LAB_TEST(&lab_test);

    /*
typedef struct {
    uint8_t BRIDGE_START_STOP_cmd : 1;        ///< B0:0   Destination: GEO,MASTER
    float BRIDGE_CHECKPOINT_latitude;         ///< B28:1  Min: -90 Max: 90   Destination: GEO,MASTER
    float BRIDGE_CHECKPOINT_longitude;        ///< B57:29  Min: -180 Max: 180   Destination: GEO,MASTER
    uint8_t BRIDGE_FINAL_COORDINATE : 1;      ///< B58:58   Destination: GEO,MASTER
    uint8_t BRIDGE_COORDINATE_READY : 1;      ///< B59:59   Destination: GEO,MASTER

    dbc_mia_info_t mia_info;
} BRIDGE_START_STOP_t;
    */
    if (flag==0){
        Uart3 &u3 = Uart3::getInstance();
        char mail[5];
        bool success = false;
        success = u3.getChar(mail, 0);
        // u3.gets
        if(success){
            // start_car();
            double a;
            memcpy(&a, mail, sizeof(double));
            printf("Recieved comm %.3f\n", a);
            LE.toggle(3);
            // flag=1;
        }else{
            // LE.toggle(1);
        }
        
    }
}

void period_100Hz(uint32_t count)
{
    // LE.toggle(3);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    // LE.toggle(4);
}
