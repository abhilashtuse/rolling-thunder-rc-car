#include "rt.h"

float decode_long(char *bytecode)
{
    float longitude = 0;
    while(bytecode && *bytecode)
    {
        longitude *= 10;
        longitude += *bytecode - '0';
    }
    return (longitude);
}

float decode_lat(char *bytecode)
{
    int     sign = 1;
    if (!bytecode)
        return 0;
    if (*bytecode == '-'){
        sign = -1;
        bytecode++;
    }
    return (sign * decode_long(bytecode));
}

// This method needs to be defined once, and AGC will call it for all dbc_encode_and_send_FOO() functions

void    bridge_heartbeat()
{
    BRIDGE_HB_t heartbeat = {0};
    heartbeat.BRIDGE_heartbeat = 1;
    dbc_encode_and_send_BRIDGE_HB(&heartbeat);
}

void rx_can(void)
{
	bool rc;
	can_msg_t can_rx_msg;
	rc = CAN_rx(can1, &can_rx_msg, 0x50);
	if (can_rx_msg.msg_id == 150)
	{
		//do stuff with sensor data from sensor
		// LE.on(1);
	} else if (can_rx_msg.msg_id == 350) {
        //do stuff with motor feedback
        // LE.on(4);
	} else if (can_rx_msg.msg_id == 400) {
        //do stuff with update_current_location from geo
		// LE.off(2);
	}
}