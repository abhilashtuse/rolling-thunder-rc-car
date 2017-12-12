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
#include "can.h"
#include "_can_dbc/generated_can.h"


/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_MONITOR_TASK_STACK_SIZE_BYTES = (512 * 3);
static bool start_from_master = false;

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
    CAN_init(can1,100,10,10,NULL,NULL);
    CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus( can1);
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
    if (CAN_is_bus_off(can1)) {
        printf("\nResetting can bus.");
        CAN_reset_bus(can1);
    }
    if (start_from_master) {
        GEO_HB_t geo_hb_cmd = { 0 };
        geo_hb_cmd.GEO_heartbeat = 1;
        // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
        if (dbc_encode_and_send_GEO_HB(&geo_hb_cmd))
            printf("\nsent hb");
        else
            printf("\ntx failed");
    }
}

void period_10Hz(uint32_t count)
{
    if (start_from_master) {
        GEO_DATA_t geo_cmd = { 0 };
        geo_cmd.GEO_bearing_angle = 20;
        geo_cmd.GEO_destination_reached = 0;
        geo_cmd.GEO_distance_to_checkpoint = 100;

        // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
        if (dbc_encode_and_send_GEO_DATA(&geo_cmd))
            printf("\nsent");
        else
            printf("\ntx failed");
    }
}

void period_100Hz(uint32_t count)
{
    can_msg_t can_msg = { 0 };
    if (!start_from_master && CAN_rx(can1, &can_msg, 0))
    {
        MASTER_CONTROL_t master_can_msg;

        // Form the message header from the metadata of the arriving message
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;

        // Attempt to decode the message (brute force, but should use switch/case with MID)
        dbc_decode_MASTER_CONTROL(&master_can_msg, can_msg.data.bytes, &can_msg_hdr);
        if (master_can_msg.MASTER_CONTROL_cmd == 1) {
            start_from_master = true;
            LE.set(1, true);
        }
    }
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    LE.toggle(4);
}
