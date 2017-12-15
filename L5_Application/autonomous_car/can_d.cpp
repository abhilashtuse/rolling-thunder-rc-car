/*
 * can_dbc.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: Thrishna
 */

#include "string.h"
#include "utilities.h"
#include "_can_dbc/generated_can.h"
#include "can.h"
#include "io.hpp"

const uint32_t            MASTER_CONTROL__MIA_MS = 3000;
const MASTER_CONTROL_t      MASTER_CONTROL__MIA_MSG = {DRIVER_HEARTBEAT_cmd_RESET,DRIVER_HEARTBEAT_cmd_RESET};
MASTER_CONTROL_t master_can_start_msg;

void initialize_can()
{
    CAN_init(can1,100,50,50,NULL,NULL);
    CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can1);
}

void can_reset()
 {
    if(CAN_is_bus_off(can1))
        {
            CAN_reset_bus(can1);
        }
 }

bool can_recieve()
{
    bool isStart = false;
    can_msg_t can_msg;
    //CAN_rx(can1, &can_msg, 0);
    // Empty all of the queued, and received messages within the last 10ms (100Hz callback frequency)
      while (CAN_rx(can1, &can_msg, 0))
       {
                   // Form the message header from the metadata of the arriving message
                   dbc_msg_hdr_t can_msg_hdr;
                   can_msg_hdr.dlc = can_msg.frame_fields.data_len;
                   can_msg_hdr.mid = can_msg.msg_id;

                   // Attempt to decode the message (brute force, but should use switch/case with MID)
                   if(dbc_decode_MASTER_CONTROL(&master_can_start_msg, can_msg.data.bytes, &can_msg_hdr))

                   {
                       if(master_can_start_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_START)
                       {
                           isStart = true;
                       }
                       else if(master_can_start_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_RESET)
                       {
                           isStart = false;
                       }
                   }

                   //u0_dbg_printf("%d",sensor_can_msg.SENSOR_light_percent);

         }

     dbc_handle_mia_MASTER_CONTROL(&master_can_start_msg, 100);
     return isStart;

}


bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}

void can_transmit_message(int left, int middle, int right, int back)
{

       SENSOR_DATA_t sensor_cmd = { 0 };
       sensor_cmd.SENSOR_left_sensor = left;
       sensor_cmd.SENSOR_middle_sensor = middle;
       sensor_cmd.SENSOR_right_sensor = right;
       sensor_cmd.SENSOR_back_sensor = back;

       // This function will encode the CAN data bytes, and send the CAN msg using dbc_app_send_can_msg()
       dbc_encode_and_send_SENSOR_DATA(&sensor_cmd);
}

void can_transmit_heartbeat()
{

       SENSOR_HB_t sensor_cmd = { 0 };
       sensor_cmd.SENSOR_heartbeat = 1;


       // This function will encode the CAN data bytes, and send the CAN msg using dbc_app_send_can_msg()
       dbc_encode_and_send_SENSOR_HB(&sensor_cmd);
}












