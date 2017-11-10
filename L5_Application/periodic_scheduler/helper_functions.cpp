#include "rt.h"
#include <stdio.h>

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
    start_stop.BRIDGE_FINAL_COORDINATE = 0;

    dbc_encode_and_send_BRIDGE_START_STOP(&start_stop);
}

float decode_long(char *bytecode, unsigned int start, unsigned int end)
{
    float longitude = 0;
    while(start < end && bytecode[start])
    {
        longitude *= 10;
        longitude += bytecode[start] - '0';
        start++;
    }
    return (longitude);
}

uint8_t get_two(char *ptr)
{
    uint8_t n_c = 0;
    int    i = 0;

    while (ptr && *ptr && i < 2)
    {
        n_c *= 10;
        n_c += ptr[i] - '0';
        i++;
    }
    return (n_c);
}
// float encode_long(float longitude)
// {
//     char longit[9];
//     while(longitude)
//     {
//         longitude *= 10;
//         longitude += *bytecode - '0';
//     }
//     return (longitude);
// }

float decode_lat(char *bytecode, unsigned int start, unsigned int end)
{
    float latitude = 0;
    while(start < end && bytecode[start])
    {
        latitude *= 10;
        latitude += bytecode[start] - '0';
        start++;
    }
    return (latitude);
}


// void    bridge_heartbeat()
// {
//     BRIDGE_HB_t heartbeat = {0};
//     heartbeat.BRIDGE_heartbeat = 1;
//     dbc_encode_and_send_BRIDGE_HB(&heartbeat);
// }

void rx_can(void)
{
	can_msg_t can_rx_msg;
    CAN_rx(can1, &can_rx_msg, 0x50);
	if (can_rx_msg.msg_id == 150)
	{
		//do stuff with sensor data from sensor
		// LE.on(1);
	} else if (can_rx_msg.msg_id == 350) {
        //do stuff with motor feedback
        // LE.on(4);
	} else if (can_rx_msg.msg_id == 400) {

        UPDATE_CURRENT_LOCATION_t curr_loc = {0};
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_rx_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_rx_msg.msg_id;
        dbc_decode_UPDATE_CURRENT_LOCATION(&curr_loc, can_rx_msg.data.bytes, &can_msg_hdr);
        printf("Current Lat: %f\n", curr_loc.UPDATE_calculated_latitude);
        printf("Current Lng: %f\n", curr_loc.UPDATE_calculated_longitude);
        // dbc_encode_and_send_UPDATE_CURRENT_LOCATION(&curr_loc);

        /*
        * Send coordinates to the app
        */
        // Uart3 &u3 = Uart3::getInstance();
        // u3.putline()
	}
}