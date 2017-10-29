#include "genera_ted_can.h.h"

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

void    bridge_heartbeat()
{
    BRIDGE_HB_t heartbeat = {0};
    heartbeat.BRIDGE_heartbeat = 1;
    dbc_encode_and_send_BRIDGE_HB(&heartbeat);
}