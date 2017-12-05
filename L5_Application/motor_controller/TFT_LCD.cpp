
#include "TFT_LCD.hpp"
#include "Uart3.hpp"
#include "utilities.h"
#include "io.hpp"
#include <stdio.h>
#include <string.h>

// Commands
#define WRITE_CMD       0x01
#define WRITE_STR       0x02

// Object IDs
#define LEDDIGIT        0x0F
#define SPEEDOMETER     0x10
#define GAUGE           0x0B
#define ANGULARMETER    0x07
#define TEXT            0x15
#define STRING          0x11

Uart3 &u2 = Uart3::getInstance();

char ack;
char chksum;
char byte_h;
char byte_l;
char buff[12];
//char buff[12];
//char buff[12];

/*
 * LCD Initialization
 */
void TFT_LCD_init(void)
{
    u2.init(9600, 50, 50);
    ack = 0x00;
    byte_h = 0x00;
    byte_l = 0x00;
    chksum = 0x00;
}

/*
 *  Function to write to object within LCD
 */
void write_object(char command, char obj_type, char obj_index, char byte2, char byte1, char checksum)
{

    checksum = command ^ obj_type ^ obj_index ^ byte2 ^ byte1;
    u2.putChar(command, 0);
    u2.putChar(obj_type, 0);
    u2.putChar(obj_index, 0);
    u2.putChar(byte2, 0);
    u2.putChar(byte1, 0);
    u2.putChar(checksum, 0);
    u2.getChar(&ack, 0);
    LD.setNumber(1);
    chksum = 0x00;
    byte_h = 0x00;
    byte_l = 0x00;
}

/*
 * Function to write to string within LCD
 */
void write_string(char command, char index, char *string, char checksum)
{
    int len = strlen(string);
    checksum = command ^ index ^ len;
    char *p;
    u2.putChar(command, 0);
    u2.putChar(index, 0);
    u2.putChar(len, 0);
    for (p = string; *p; ++p) {
        u2.putChar(*p, 0);
        checksum ^= *p;
    }
    u2.putChar(checksum, 0);
    u2.getChar(&ack, 0);
    LD.setNumber(2);
    chksum = 0x00;
}
/*
 *  Function to send data to battery gauge
 */
void send_Battery_data(uint8_t bat)
{
    write_object(WRITE_CMD, GAUGE, 0x00, 0x00, bat, chksum);
}

/*
 * Function to send data to sensor gauges
 */
void send_Sensor_data(uint16_t middle_sensor, uint8_t left_sensor, uint8_t right_sensor)
{
    byte_h = (char) ((middle_sensor >> 8) & 0xff);
    byte_l = (char) (middle_sensor & 0x00ff);
    write_object(WRITE_CMD, GAUGE, 0x02, byte_h, byte_l, chksum);
    write_object(WRITE_CMD, GAUGE, 0x01, 0x00, left_sensor, chksum);
    write_object(WRITE_CMD, GAUGE, 0x03, 0x00, right_sensor, chksum);
    memset(buff, 0, 12);
    sprintf(buff, "%d", left_sensor);
    write_string(WRITE_STR, 0x04, buff, chksum);
    memset(buff, 0, 12);
    sprintf(buff, "%d", middle_sensor);
    write_string(WRITE_STR, 0x05, buff, chksum);
    memset(buff, 0, 12);
    sprintf(buff, "%d", right_sensor);
    write_string(WRITE_STR, 0x06, buff, chksum);
    byte_h = 0x00;
    byte_l = 0x00;
}
/*
 * Function to send data to speedometer and LED digits
 */
void send_Motor_data(float speed)
{
    write_object(WRITE_CMD, SPEEDOMETER, 0x00, 0x00, speed, chksum);
    memset(buff, 0, 12);
    sprintf(buff, "%6.3f", speed);
    write_string(WRITE_STR, 0x03, buff, chksum);
}

/*
 * Function to send data to angularmeter
 */
void send_Compass_data(int16_t b_angle)
{
    byte_h = (char) ((b_angle >> 8) & 0xff);
    byte_l = (char) (b_angle & 0x00ff);
    write_object(WRITE_CMD, ANGULARMETER, 0x00, byte_h, byte_l, chksum);
    write_object(WRITE_CMD, LEDDIGIT, 0x00, byte_h, byte_l, chksum);
    byte_h = 0x00;
    byte_l = 0x00;
}

/*
 * Function to send data to LED digit and static text
 */
void send_GPS_data(float lat_val, float lng_val, float dist)
{
    memset(buff, 0, 12);
    sprintf(buff, "%f", lat_val);
    write_string(WRITE_STR, 0x00, buff, chksum);
    memset(buff, 0, 12);
    sprintf(buff, "%f", lng_val);
    write_string(WRITE_STR, 0x01, buff, chksum);
    memset(buff, 0, 12);
    sprintf(buff, "%6.3f", dist);
    write_string(WRITE_STR, 0x02, buff, chksum);
}
