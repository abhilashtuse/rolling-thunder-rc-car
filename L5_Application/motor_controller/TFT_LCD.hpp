#ifndef TFT_LCD_HPP_
#define TFT_LCD_HPP_

#include <stdint.h>

void TFT_LCD_init(void);
void write_object(char command, char obj_type, char obj_index, char byte2, char byte1, char checksum);
void send_Battery_data (uint8_t bat);
void send_Sensor_data(uint16_t middle_sensor, uint8_t left_sensor, uint8_t right_sensor);
void send_Motor_data (int16_t speed);
void send_Compass_data (int16_t b_angle);
void send_GPS_data (float lat_val, float lng_val, float dist);
void write_string (char command, char index, char string, char checksum);
#endif /* TFT_LCD_HPP_ */
