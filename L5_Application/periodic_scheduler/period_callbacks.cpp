#include <stdint.h>
#include <string.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "can.h"
#include "_can_dbc/generated_can.h"

bool restart_can = false;

void busoff_callback(uint32_t register_value)
{
    (void) register_value;
    restart_can = true;
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
    CAN_init(can1, 100, 25, 25, busoff_callback, dataovr_callback);

    CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can1);

    return true; // Must return true upon success
}

bool start_sent = false;
BRIDGE_START_STOP_t bridge_data = {0};

void period_1Hz(uint32_t count)
{
    if(restart_can == true)
    {
        restart_can = false;
        CAN_reset_bus(can1);
        LE.toggle(1);
    }
    can_msg_t can_msg;
    MASTER_CONTROL_t start_cmd;
    start_cmd.MASTER_CONTROL_cmd = MASTER_cmd_START;

    if(start_sent == false)
    {
        if(CAN_rx(can1, &can_msg, 0))
        {
            // Form the message header from the metadata of the arriving message
            dbc_msg_hdr_t can_msg_hdr;
            can_msg_hdr.dlc = can_msg.frame_fields.data_len;
            can_msg_hdr.mid = can_msg.msg_id;

            // Attempt to decode the message
            dbc_decode_BRIDGE_START_STOP(&bridge_data, can_msg.data.bytes, &can_msg_hdr);

            if(bridge_data.BRIDGE_START_STOP_cmd == 1)
            {
                LE.set(2, true);
                dbc_encode_and_send_MASTER_CONTROL(&start_cmd);
                start_sent = true;
            }
        }
    }
    LE.toggle(1);
}

void period_10Hz(uint32_t count)
{

}

void period_100Hz(uint32_t count)
{
    can_msg_t can_msg;
    SENSOR_DATA_t sensor_data;
    MOTOR_UPDATE_t motor_update;
    motor_update.MOTOR_speed = 5;
    motor_update.MOTOR_turn_angle = 0;

    if(start_sent == true)
    {
        if(CAN_rx(can1, &can_msg, 0))
        {
            LE.toggle(3);

            // Form the message header from the metadata of the arriving message
            dbc_msg_hdr_t can_msg_hdr;
            can_msg_hdr.dlc = can_msg.frame_fields.data_len;
            can_msg_hdr.mid = can_msg.msg_id;

            if(dbc_decode_SENSOR_DATA(&sensor_data, can_msg.data.bytes, &can_msg_hdr))
            {
                if(sensor_data.SENSOR_left_sensor > 1 && sensor_data.SENSOR_left_sensor < 15)
                {
                    motor_update.MOTOR_turn_angle = 30;
                }
                else if(sensor_data.SENSOR_right_sensor > 1 && sensor_data.SENSOR_right_sensor < 15)
                {
                    motor_update.MOTOR_turn_angle = -30;
                }
                else if(sensor_data.SENSOR_middle_sensor > 6 && sensor_data.SENSOR_middle_sensor < 15)
                {
                    motor_update.MOTOR_speed = 0;
                    motor_update.MOTOR_turn_angle = 0;
                }
                else if(sensor_data.SENSOR_back_sensor > 6 && sensor_data.SENSOR_back_sensor < 15)
                {
                    motor_update.MOTOR_speed = 0;
                    motor_update.MOTOR_turn_angle = 0;
                }
                dbc_encode_and_send_MOTOR_UPDATE(&motor_update);
            }
            else if(dbc_decode_BRIDGE_START_STOP(&bridge_data, can_msg.data.bytes, &can_msg_hdr))
            {
                if(bridge_data.BRIDGE_START_STOP_cmd == 0)
                {
                    LE.set(2, false);
                    motor_update.MOTOR_speed = 0;
                    motor_update.MOTOR_turn_angle = 0;
                    dbc_encode_and_send_MOTOR_UPDATE(&motor_update);
                    start_sent = false;
                }
            }
        }
    }
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    //LE.toggle(4);

}
