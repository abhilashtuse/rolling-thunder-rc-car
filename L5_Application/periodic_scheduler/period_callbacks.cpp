#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "master_controller.hpp"
#include "can.h"

const uint32_t                             SENSOR_DATA__MIA_MS = 5000;
const SENSOR_DATA_t                        SENSOR_DATA__MIA_MSG = {9,9,9,9};

/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_MONITOR_TASK_STACK_SIZE_BYTES = (512 * 3);

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}

bool restart_can = false;

void busoff_callback(uint32_t register_value)
{
    (void) register_value;
    restart_can = true;
    LE.set(1,true);
    CAN_reset_bus(can1);
}

void dataovr_callback(uint32_t register_value)
{
    (void) register_value;
}

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}



/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
    CAN_init(can1, 100, 50, 50, busoff_callback, dataovr_callback);

    CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can1);

    return true; // Must return true upon success
}

bool start_sent = false;
BRIDGE_START_STOP_t bridge_data = {0};

void period_1Hz(uint32_t count)
{

}

void period_10Hz(uint32_t count)
{

}

SENSOR_DATA_t sensor_data;
GEO_DATA_t geo_data;
MOTOR_FEEDBACK_t motor_feedback;

void period_100Hz(uint32_t count)
{
    can_msg_t can_msg;
    MOTOR_UPDATE_t motor_update = {0};
    motor motor;

    MASTER_CONTROL_t start_cmd;
    start_cmd.MASTER_CONTROL_cmd = DRIVER_HEARTBEAT_cmd_START;

    while(CAN_rx(can1, &can_msg, 0))
    {
        // Form the message header from the metadata of the arriving message
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;

        if(dbc_decode_BRIDGE_START_STOP(&bridge_data, can_msg.data.bytes, &can_msg_hdr))
        {
            if(bridge_data.BRIDGE_START_STOP_cmd == 0)
            {
                motor_update.MOTOR_speed = 0;
                motor_update.MOTOR_turn_angle = 0;
                dbc_encode_and_send_MOTOR_UPDATE(&motor_update);
                start_sent = false;
                break;
            }
            else
            {
                LE.set(2, true);
                dbc_encode_and_send_MASTER_CONTROL(&start_cmd);
                start_sent = true;
            }
        }
        else if(dbc_decode_SENSOR_DATA(&sensor_data, can_msg.data.bytes, &can_msg_hdr))
        {
            if(start_sent)
            {
                LE.toggle(4);
                motor.update_motor(sensor_data, &motor_update,motor_feedback.MOTOR_actual_speed);
                dbc_encode_and_send_MOTOR_UPDATE(&motor_update);
            }
        }
        else if(dbc_decode_GEO_DATA(&geo_data, can_msg.data.bytes, &can_msg_hdr))
        {
            if(start_sent)
            {
                LE.toggle(3);

                if(geo_data.GEO_destination_reached)
                {
                    motor_update.MOTOR_speed = 0;
                    motor_update.MOTOR_turn_angle = 0;
                    dbc_encode_and_send_MOTOR_UPDATE(&motor_update);
                    break;
                }

                motor_update.MOTOR_speed = 1.5;
                motor_update.MOTOR_turn_angle = geo_data.GEO_bearing_angle/6;
                dbc_encode_and_send_MOTOR_UPDATE(&motor_update);
            }
        }
        else if(dbc_decode_MOTOR_FEEDBACK(&motor_feedback, can_msg.data.bytes, &can_msg_hdr))
        {

        }
    }
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    //LE.toggle(4);

}
