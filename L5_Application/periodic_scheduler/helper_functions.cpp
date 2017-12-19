#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "io.hpp"
#include "periodic_callback.h"
#include "gpio.hpp"
#include "genera_ted_can.h"
#include "lpc_sys.h"

const uint32_t UPDATE_CURRENT_LOCATION__MIA_MS = 3000;
UPDATE_CURRENT_LOCATION_t UPDATE_CURRENT_LOCATION__MIA_MSG = {0};

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}

void    start_car(double latitude, double longitude, int start)
{
    BRIDGE_START_STOP_t start_stop = {0};
    start_stop.BRIDGE_START_STOP_cmd = start;
    start_stop.BRIDGE_CHECKPOINT_latitude = latitude;
    start_stop.BRIDGE_CHECKPOINT_longitude = longitude;
    start_stop.BRIDGE_FINAL_COORDINATE = 0;

    dbc_encode_and_send_BRIDGE_START_STOP(&start_stop);
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

void    bridge_heartbeat()
{
    BRIDGE_HB_t heartbeat = {0};
    heartbeat.BRIDGE_heartbeat = 1;
    dbc_encode_and_send_BRIDGE_HB(&heartbeat);
}

void    convert_to_app_format(char **str, double lat, double lng)
{
    char ltr[10] = {0};
    char lntr[12] = {0};
    strcat(*str, "0911");
    sprintf(ltr, "%f", lat);
    sprintf(lntr, "%f", lng);
    strcat(*str, ltr);
    strcat(*str, lntr);
    strcat(*str, "e\0");
}

void rx_can(void)
{
	can_msg_t can_rx_msg;
    UPDATE_CURRENT_LOCATION_t curr_loc = {0};
    while(CAN_rx(can1, &can_rx_msg, 0))
    {
        if (can_rx_msg.msg_id == 400) {
            dbc_msg_hdr_t can_msg_hdr;
            can_msg_hdr.dlc = can_rx_msg.frame_fields.data_len;
            can_msg_hdr.mid = can_rx_msg.msg_id;
            dbc_decode_UPDATE_CURRENT_LOCATION(&curr_loc, can_rx_msg.data.bytes, &can_msg_hdr);

            UPDATE_CURRENT_LOCATION__MIA_MSG.UPDATE_calculated_latitude = curr_loc.UPDATE_calculated_latitude;
            UPDATE_CURRENT_LOCATION__MIA_MSG.UPDATE_calculated_longitude = curr_loc.UPDATE_calculated_longitude;

            char str[30] = {0};
            char *to_app = str;
            convert_to_app_format(&to_app, curr_loc.UPDATE_calculated_latitude, curr_loc.UPDATE_calculated_longitude);
            /*
            * Send coordinates to the app
            */
            // printf("To app: %s\n", to_app);
            Uart3 &u3 = Uart3::getInstance();
            u3.putline(to_app);
        }
        else if(dbc_handle_mia_UPDATE_CURRENT_LOCATION(&UPDATE_CURRENT_LOCATION__MIA_MSG, 10))
        {
            char str[30] = {0};
            char *to_app = str;
            if (UPDATE_CURRENT_LOCATION__MIA_MSG.UPDATE_calculated_latitude > 0 &&
                UPDATE_CURRENT_LOCATION__MIA_MSG.UPDATE_calculated_longitude > 0){
                convert_to_app_format(&to_app, UPDATE_CURRENT_LOCATION__MIA_MSG.UPDATE_calculated_latitude,
                UPDATE_CURRENT_LOCATION__MIA_MSG.UPDATE_calculated_longitude);
                // printf("To app: %s\n", to_app);
                Uart3 &u3 = Uart3::getInstance();
                u3.putline(to_app);
            }
        }
    }
}

void    parse_and_send(char **str)
{
    char *buffer = *str;
    const char *t = "\0";
    uint8_t nb_lat = 0;
    uint8_t nb_long = 0;
    static uint8_t n_checkpoints = 0;

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
    else if (buffer[0] == 'c'){ //sending checkpoints
        buffer = buffer + 1;
        
        int size = 0;
        BRIDGE_START_STOP_t checkpoint = {0};
        t = strchrnul(buffer, 'c');
        // checkpoint.BRIDGE_FINAL_COORDINATE = (!t || !*t) ? 1 : 0;
        if (t - buffer < 24)
            return ;
        if (t - buffer > 24){
        //set checkpoints
            n_checkpoints = get_two(buffer);
            LD.setNumber(n_checkpoints);
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

        n_checkpoints--;
        checkpoint.BRIDGE_FINAL_COORDINATE = (n_checkpoints <= 0) ? 1 : 0;

        dbc_encode_and_send_BRIDGE_START_STOP(&checkpoint);
        

        // printf("Lat: %f\nLong: %f\nFinal?: %d\n",
        // checkpoint.BRIDGE_CHECKPOINT_latitude,checkpoint.BRIDGE_CHECKPOINT_longitude,checkpoint.BRIDGE_FINAL_COORDINATE);

        *str = buffer;

        if (checkpoint.BRIDGE_FINAL_COORDINATE)
        {
            sys_reboot();
        }
    }       
}