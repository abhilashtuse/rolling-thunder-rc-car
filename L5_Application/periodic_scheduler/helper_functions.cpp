#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "gpio.hpp"
#include "rt.h"

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
        // dbc_handle_mia_UPDATE_CURRENT_LOCATION
        /*
        * Send coordinates to the app
        */
        // Uart3 &u3 = Uart3::getInstance();
        // u3.putline()
	}
}

void    parse_and_send(char **str)
{
    // static uint8_t n_checkpoints = 0;
    char *buffer = *str;
    const char *t = "\0";
    uint8_t nb_lat = 0;
    uint8_t nb_long = 0;
    // int read_n_checkpoints = 0;

    if (buffer[0] == 'a'){
        if (buffer[1] == '0')
        {
            start_car(0,0,0);
            LE.toggle(2);//stop car command sent
        }
        else
        {
            start_car(1, 1, 1); // manual car start
            LE.toggle(4);//start car command sent
        }
    }
    else if(buffer[0]=='b')
    {
        
    }else if (buffer[0] == 'c'){ //sending checkpoints
        buffer = buffer + 1;
        int size = 0;
        BRIDGE_START_STOP_t checkpoint = {0};
        t = strchrnul(buffer, 'c');
        checkpoint.BRIDGE_FINAL_COORDINATE = (!t || !*t) ? 1 : 0;
        // read_n_checkpoints = () ? 1 : 0;
        if (t - buffer < 24)
            return ;
        if (t - buffer > 24){
        //set checkpoints
            // n_checkpoints = get_two(buffer);
            buffer = buffer + 2;
            size+=2;
        }

        nb_lat = get_two(buffer);
        buffer = buffer + 2;
        nb_long = get_two(buffer);
        buffer = buffer + 2;
        size += 4 +nb_lat+nb_long;
        /*
        * Checkpoints
        */
        // printf("%s\n", buffer);
        char lat[nb_lat] = {0};
        char lng[nb_long] = {0};
        
        
        checkpoint.BRIDGE_START_STOP_cmd = 1;

        //Extract latitude
        strncat(lat, buffer, nb_lat);
        checkpoint.BRIDGE_CHECKPOINT_latitude = atof(lat);
        buffer = buffer + nb_lat;

        //Extract longitude
        strncat(lng, buffer, nb_long);
        checkpoint.BRIDGE_CHECKPOINT_longitude = atof(lng);
        buffer = buffer + nb_long;

        // n_checkpoints--;
        
        // dbc_encode_and_send_BRIDGE_START_STOP(&checkpoint);

        printf("Lat: %f\nLong: %f\nFinal?: %d\n",
        checkpoint.BRIDGE_CHECKPOINT_latitude,checkpoint.BRIDGE_CHECKPOINT_longitude,checkpoint.BRIDGE_FINAL_COORDINATE);

        *str = buffer;
        // memmove(*str, *str + size,)
    }
}