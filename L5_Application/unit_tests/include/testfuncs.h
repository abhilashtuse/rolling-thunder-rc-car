#ifndef TESTFUNCS_H
# define TESTFUNCS_H

#include <stdint.h>

typedef struct {
    uint8_t BRIDGE_START_STOP_cmd : 1;        ///< B0:0   Destination: GEO,MASTER
    double BRIDGE_CHECKPOINT_latitude;         ///< B28:1  Min: -90 Max: 90   Destination: GEO,MASTER
    double BRIDGE_CHECKPOINT_longitude;        ///< B57:29  Min: -180 Max: 180   Destination: GEO,MASTER
    uint8_t BRIDGE_FINAL_COORDINATE : 1;      ///< B58:58   Destination: GEO,MASTER

    // dbc_mia_info_t mia_info;
} BRIDGE_START_STOP_t;

uint8_t get_two(char *ptr);
float decode_long(char *bytecode, unsigned int start, unsigned int end);
float decode_lat(char *bytecode, unsigned int start, unsigned int end);
BRIDGE_START_STOP_t    period_ten_test(char *buffer);
#endif