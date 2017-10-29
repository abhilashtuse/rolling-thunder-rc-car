/// DBC file: DBC_File.dbc    Self node: 'BRIDGE'  (ALL = 1)
/// This file can be included by a source file, for example: #include "generated.h"
#ifndef __GENEARTED_DBC_PARSER
#define __GENERATED_DBC_PARSER
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/// Extern function needed for dbc_encode_and_send()
extern bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8]);

/// Missing in Action structure
typedef struct {
    uint32_t is_mia : 1;          ///< Missing in action flag
    uint32_t mia_counter_ms : 31; ///< Missing in action counter
} dbc_mia_info_t;

/// CAN message header structure
typedef struct { 
    uint32_t mid; ///< Message ID of the message
    uint8_t  dlc; ///< Data length of the message
} dbc_msg_hdr_t; 

static const dbc_msg_hdr_t MASTER_CONTROL_HDR =                   {  100, 1 };
static const dbc_msg_hdr_t SENSOR_DATA_HDR =                      {  150, 4 };
static const dbc_msg_hdr_t BRIDGE_START_STOP_HDR =                {  200, 8 };
static const dbc_msg_hdr_t GEO_DATA_HDR =                         {  250, 3 };
static const dbc_msg_hdr_t MOTOR_UPDATE_HDR =                     {  300, 3 };
static const dbc_msg_hdr_t GEO_UPDATE_LOCATION_HDR =              {  350, 8 };
static const dbc_msg_hdr_t MOTOR_FEEDBACK_HDR =                   {  400, 2 };
static const dbc_msg_hdr_t UPDATE_CURRENT_LOCATION_HDR =          {  450, 8 };
static const dbc_msg_hdr_t BRIDGE_HB_HDR =                        {  500, 1 };
static const dbc_msg_hdr_t SENSOR_HB_HDR =                        {  510, 1 };
static const dbc_msg_hdr_t GEO_HB_HDR =                           {  520, 1 };
static const dbc_msg_hdr_t MOTOR_HB_HDR =                         {  530, 1 };

/// Enumeration(s) for Message: 'MASTER_CONTROL' from 'MASTER

typedef enum {
    DRIVER_HEARTBEAT_cmd_RESET = 0,
    DRIVER_HEARTBEAT_cmd_START = 1,
} MASTER_CONTROL_cmd_E ;




/// Message: MASTER_CONTROL from 'MASTER
// DLC: 1 byte(s), MID: 100
typedef struct {
    MASTER_CONTROL_cmd_E MASTER_CONTROL_cmd : 1; ///< B0:0   Destination: SENSOR,MOTOR,BRIDGE,GEO

    dbc_mia_info_t mia_info;
} MASTER_CONTROL_t;


/// Message: SENSOR_DATA from 'SENSOR// DLC: 4 byte(s), MID: 150
typedef struct {
    uint8_t SENSOR_left_sensor;               ///< B7:0   Destination: MASTER,BRIDGE
    uint8_t SENSOR_middle_sensor;             ///< B15:8   Destination: MASTER,BRIDGE
    uint8_t SENSOR_right_sensor;              ///< B23:16   Destination: MASTER,BRIDGE
    uint8_t SENSOR_back_sensor;               ///< B31:24   Destination: MASTER,BRIDGE

    dbc_mia_info_t mia_info;
} SENSOR_DATA_t;


/// Message: BRIDGE_START_STOP from 'BRIDGE
// DLC: 8 byte(s), MID: 200
typedef struct {
    uint8_t BRIDGE_START_STOP_cmd : 1;        ///< B0:0   Destination: GEO,MASTER
    float BRIDGE_CHECKPOINT_latitude;         ///< B28:1  Min: -90 Max: 90   Destination: GEO,MASTER
    float BRIDGE_CHECKPOINT_longitude;        ///< B57:29  Min: -180 Max: 180   Destination: GEO,MASTER
    uint8_t BRIDGE_FINAL_COORDINATE : 1;      ///< B58:58   Destination: GEO,MASTER
    uint8_t BRIDGE_COORDINATE_READY : 1;      ///< B59:59   Destination: GEO,MASTER

    dbc_mia_info_t mia_info;
} BRIDGE_START_STOP_t;


/// Message: GEO_DATA from 'GEO
// DLC: 3 byte(s), MID: 250
typedef struct {
    int16_t GEO_bearing_angle;                ///< B8:0  Min: -180 Max: 180   Destination: MASTER
    uint8_t GEO_distance_to_checkpoint;       ///< B16:9   Destination: MASTER
    uint8_t GEO_destination_reached : 1;      ///< B17:17   Destination: MASTER

    dbc_mia_info_t mia_info;
} GEO_DATA_t;


/// Message: MOTOR_UPDATE from 'MASTER
// DLC: 3 byte(s), MID: 300
typedef struct {
    int16_t MOTOR_speed;                      ///< B7:0  Min: -75 Max: 75   Destination: MOTOR
    int8_t MOTOR_turn_angle;                  ///< B13:8  Min: -30 Max: 30   Destination: MOTOR

    dbc_mia_info_t mia_info;
} MOTOR_UPDATE_t;


/// Message: GEO_UPDATE_LOCATION from 'GEO
// DLC: 8 byte(s), MID: 350
typedef struct {
    float GEO_update_latitude;                ///< B27:0  Min: -90 Max: 90   Destination: BRIDGE
    float GEO_update_longitude;               ///< B56:28  Min: -180 Max: 180   Destination: BRIDGE

    dbc_mia_info_t mia_info;
} GEO_UPDATE_LOCATION_t;


/// Message: MOTOR_FEEDBACK from 'MOTOR
// DLC: 2 byte(s), MID: 400
typedef struct {
    uint8_t MOTOR_actual_speed;               ///< B6:0  Min: 0 Max: 75   Destination: BRIDGE,MASTER
    float sensed_battery_voltage;             ///< B12:7  Min: 0 Max: 5   Destination: BRIDGE,MASTER

    dbc_mia_info_t mia_info;
} MOTOR_FEEDBACK_t;


/// Message: UPDATE_CURRENT_LOCATION from 'GEO
// DLC: 8 byte(s), MID: 450
typedef struct {
    float UPDATE_calculated_latitude;         ///< B27:0  Min: -90 Max: 90   Destination: BRIDGE
    float UPDATE_calculated_longitude;        ///< B56:28  Min: -180 Max: 180   Destination: BRIDGE

    dbc_mia_info_t mia_info;
} UPDATE_CURRENT_LOCATION_t;


/// Message: BRIDGE_HB from 'BRIDGE
// DLC: 1 byte(s), MID: 500
typedef struct {
    uint8_t BRIDGE_heartbeat : 2;             ///< B1:0   Destination: MASTER

    dbc_mia_info_t mia_info;
} BRIDGE_HB_t;


/// Message: SENSOR_HB from 'SENSOR
// DLC: 1 byte(s), MID: 510
typedef struct {
    uint8_t SENSOR_heartbeat : 2;             ///< B1:0   Destination: MASTER

    dbc_mia_info_t mia_info;
} SENSOR_HB_t;


/// Message: GEO_HB from 'GEO
// DLC: 1 byte(s), MID: 520
typedef struct {
    uint8_t GEO_heartbeat : 2;                ///< B1:0   Destination: MASTER

    dbc_mia_info_t mia_info;
} GEO_HB_t;


/// Message: MOTOR_HB from 'MOTOR
// DLC: 1 byte(s), MID: 530
typedef struct {
    uint8_t MOTOR_heartbeat : 2;              ///< B1:0   Destination: MASTER

    dbc_mia_info_t mia_info;
} MOTOR_HB_t;


/// @{ These 'externs' need to be defined in a source file of your project
extern const uint32_t                             MASTER_CONTROL__MIA_MS;
extern const MASTER_CONTROL_t                     MASTER_CONTROL__MIA_MSG;
extern const uint32_t                             SENSOR_DATA__MIA_MS;
extern const SENSOR_DATA_t                        SENSOR_DATA__MIA_MSG;
extern const uint32_t                             BRIDGE_START_STOP__MIA_MS;
extern const BRIDGE_START_STOP_t                  BRIDGE_START_STOP__MIA_MSG;
extern const uint32_t                             GEO_DATA__MIA_MS;
extern const GEO_DATA_t                           GEO_DATA__MIA_MSG;
extern const uint32_t                             MOTOR_UPDATE__MIA_MS;
extern const MOTOR_UPDATE_t                       MOTOR_UPDATE__MIA_MSG;
extern const uint32_t                             GEO_UPDATE_LOCATION__MIA_MS;
extern const GEO_UPDATE_LOCATION_t                GEO_UPDATE_LOCATION__MIA_MSG;
extern const uint32_t                             MOTOR_FEEDBACK__MIA_MS;
extern const MOTOR_FEEDBACK_t                     MOTOR_FEEDBACK__MIA_MSG;
extern const uint32_t                             UPDATE_CURRENT_LOCATION__MIA_MS;
extern const UPDATE_CURRENT_LOCATION_t            UPDATE_CURRENT_LOCATION__MIA_MSG;
extern const uint32_t                             BRIDGE_HB__MIA_MS;
extern const BRIDGE_HB_t                          BRIDGE_HB__MIA_MSG;
extern const uint32_t                             SENSOR_HB__MIA_MS;
extern const SENSOR_HB_t                          SENSOR_HB__MIA_MSG;
extern const uint32_t                             GEO_HB__MIA_MS;
extern const GEO_HB_t                             GEO_HB__MIA_MSG;
extern const uint32_t                             MOTOR_HB__MIA_MS;
extern const MOTOR_HB_t                           MOTOR_HB__MIA_MSG;
/// @}


/// Encode MASTER
// 'MASTER_CONTROL' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MASTER_CONTROL(uint8_t bytes[8], MASTER_CONTROL_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->MASTER_CONTROL_cmd)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01)); ///< 1 bit(s) starting from B0

    return MASTER_CONTROL_HDR;
}

/// Encode and send for dbc_encode_MASTER_CONTROL() message
static inline bool dbc_encode_and_send_MASTER_CONTROL(MASTER_CONTROL_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MASTER_CONTROL(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode SENSOR's //ENSOR_DATA' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_SENSOR_DATA(uint8_t bytes[8], SENSOR_DATA_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->SENSOR_left_sensor)))) & 0xff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0

    raw = ((uint32_t)(((from->SENSOR_middle_sensor)))) & 0xff;
    bytes[1] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B8

    raw = ((uint32_t)(((from->SENSOR_right_sensor)))) & 0xff;
    bytes[2] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B16

    raw = ((uint32_t)(((from->SENSOR_back_sensor)))) & 0xff;
    bytes[3] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B24

    return SENSOR_DATA_HDR;
}

/// Encode and send for dbc_encode_SENSOR_DATA() message
static inline bool dbc_encode_and_send_SENSOR_DATA(SENSOR_DATA_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_SENSOR_DATA(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode BRIDGE
// 'BRIDGE_START_STOP' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_BRIDGE_START_STOP(uint8_t bytes[8], BRIDGE_START_STOP_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->BRIDGE_START_STOP_cmd)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01)); ///< 1 bit(s) starting from B0

    if(from->BRIDGE_CHECKPOINT_latitude < -90) { from->BRIDGE_CHECKPOINT_latitude = -90; } // Min value: -90
    if(from->BRIDGE_CHECKPOINT_latitude > 90) { from->BRIDGE_CHECKPOINT_latitude = 90; } // Max value: 90
    raw = ((uint32_t)(((from->BRIDGE_CHECKPOINT_latitude - (-90.000000)) / 1e-06) + 0.5)) & 0xfffffff;
    bytes[0] |= (((uint8_t)(raw) & 0x7f) << 1); ///< 7 bit(s) starting from B1
    bytes[1] |= (((uint8_t)(raw >> 7) & 0xff)); ///< 8 bit(s) starting from B8
    bytes[2] |= (((uint8_t)(raw >> 15) & 0xff)); ///< 8 bit(s) starting from B16
    bytes[3] |= (((uint8_t)(raw >> 23) & 0x1f)); ///< 5 bit(s) starting from B24

    if(from->BRIDGE_CHECKPOINT_longitude < -180) { from->BRIDGE_CHECKPOINT_longitude = -180; } // Min value: -180
    if(from->BRIDGE_CHECKPOINT_longitude > 180) { from->BRIDGE_CHECKPOINT_longitude = 180; } // Max value: 180
    raw = ((uint32_t)(((from->BRIDGE_CHECKPOINT_longitude - (-180.000000)) / 1e-06) + 0.5)) & 0x1fffffff;
    bytes[3] |= (((uint8_t)(raw) & 0x07) << 5); ///< 3 bit(s) starting from B29
    bytes[4] |= (((uint8_t)(raw >> 3) & 0xff)); ///< 8 bit(s) starting from B32
    bytes[5] |= (((uint8_t)(raw >> 11) & 0xff)); ///< 8 bit(s) starting from B40
    bytes[6] |= (((uint8_t)(raw >> 19) & 0xff)); ///< 8 bit(s) starting from B48
    bytes[7] |= (((uint8_t)(raw >> 27) & 0x03)); ///< 2 bit(s) starting from B56

    raw = ((uint32_t)(((from->BRIDGE_FINAL_COORDINATE)))) & 0x01;
    bytes[7] |= (((uint8_t)(raw) & 0x01) << 2); ///< 1 bit(s) starting from B58

    raw = ((uint32_t)(((from->BRIDGE_COORDINATE_READY)))) & 0x01;
    bytes[7] |= (((uint8_t)(raw) & 0x01) << 3); ///< 1 bit(s) starting from B59

    return BRIDGE_START_STOP_HDR;
}

/// Encode and send for dbc_encode_BRIDGE_START_STOP() message
static inline bool dbc_encode_and_send_BRIDGE_START_STOP(BRIDGE_START_STOP_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_BRIDGE_START_STOP(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode GEO
// 'GEO_DATA' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_GEO_DATA(uint8_t bytes[8], GEO_DATA_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    if(from->GEO_bearing_angle < -180) { from->GEO_bearing_angle = -180; } // Min value: -180
    if(from->GEO_bearing_angle > 180) { from->GEO_bearing_angle = 180; } // Max value: 180
    // Stuff a real signed number into the DBC 9-bit signal
    raw = ((uint32_t)(((from->GEO_bearing_angle)))) & 0x1ff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0x01)); ///< 1 bit(s) starting from B8

    raw = ((uint32_t)(((from->GEO_distance_to_checkpoint)))) & 0xff;
    bytes[1] |= (((uint8_t)(raw) & 0x7f) << 1); ///< 7 bit(s) starting from B9
    bytes[2] |= (((uint8_t)(raw >> 7) & 0x01)); ///< 1 bit(s) starting from B16

    raw = ((uint32_t)(((from->GEO_destination_reached)))) & 0x01;
    bytes[2] |= (((uint8_t)(raw) & 0x01) << 1); ///< 1 bit(s) starting from B17

    return GEO_DATA_HDR;
}

/// Encode and send for dbc_encode_GEO_DATA() message
static inline bool dbc_encode_and_send_GEO_DATA(GEO_DATA_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_GEO_DATA(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode MASTER
// 'MOTOR_UPDATE' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MOTOR_UPDATE(uint8_t bytes[8], MOTOR_UPDATE_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    if(from->MOTOR_speed < -75) { from->MOTOR_speed = -75; } // Min value: -75
    if(from->MOTOR_speed > 75) { from->MOTOR_speed = 75; } // Max value: 75
    // Stuff a real signed number into the DBC 8-bit signal
    raw = ((uint32_t)(((from->MOTOR_speed)))) & 0xff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0

    if(from->MOTOR_turn_angle < -30) { from->MOTOR_turn_angle = -30; } // Min value: -30
    if(from->MOTOR_turn_angle > 30) { from->MOTOR_turn_angle = 30; } // Max value: 30
    // Stuff a real signed number into the DBC 6-bit signal
    raw = ((uint32_t)(((from->MOTOR_turn_angle)))) & 0x3f;
    bytes[1] |= (((uint8_t)(raw) & 0x3f)); ///< 6 bit(s) starting from B8

    return MOTOR_UPDATE_HDR;
}

/// Encode and send for dbc_encode_MOTOR_UPDATE() message
static inline bool dbc_encode_and_send_MOTOR_UPDATE(MOTOR_UPDATE_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MOTOR_UPDATE(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode GEO
// 'GEO_UPDATE_LOCATION' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_GEO_UPDATE_LOCATION(uint8_t bytes[8], GEO_UPDATE_LOCATION_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    if(from->GEO_update_latitude < -90) { from->GEO_update_latitude = -90; } // Min value: -90
    if(from->GEO_update_latitude > 90) { from->GEO_update_latitude = 90; } // Max value: 90
    raw = ((uint32_t)(((from->GEO_update_latitude - (-90.000000)) / 1e-06) + 0.5)) & 0xfffffff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B8
    bytes[2] |= (((uint8_t)(raw >> 16) & 0xff)); ///< 8 bit(s) starting from B16
    bytes[3] |= (((uint8_t)(raw >> 24) & 0x0f)); ///< 4 bit(s) starting from B24

    if(from->GEO_update_longitude < -180) { from->GEO_update_longitude = -180; } // Min value: -180
    if(from->GEO_update_longitude > 180) { from->GEO_update_longitude = 180; } // Max value: 180
    raw = ((uint32_t)(((from->GEO_update_longitude - (-180.000000)) / 1e-06) + 0.5)) & 0x1fffffff;
    bytes[3] |= (((uint8_t)(raw) & 0x0f) << 4); ///< 4 bit(s) starting from B28
    bytes[4] |= (((uint8_t)(raw >> 4) & 0xff)); ///< 8 bit(s) starting from B32
    bytes[5] |= (((uint8_t)(raw >> 12) & 0xff)); ///< 8 bit(s) starting from B40
    bytes[6] |= (((uint8_t)(raw >> 20) & 0xff)); ///< 8 bit(s) starting from B48
    bytes[7] |= (((uint8_t)(raw >> 28) & 0x01)); ///< 1 bit(s) starting from B56

    return GEO_UPDATE_LOCATION_HDR;
}

/// Encode and send for dbc_encode_GEO_UPDATE_LOCATION() message
static inline bool dbc_encode_and_send_GEO_UPDATE_LOCATION(GEO_UPDATE_LOCATION_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_GEO_UPDATE_LOCATION(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode MOTOR
// 'MOTOR_FEEDBACK' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MOTOR_FEEDBACK(uint8_t bytes[8], MOTOR_FEEDBACK_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    // Not doing min value check since the signal is unsigned already
    if(from->MOTOR_actual_speed > 75) { from->MOTOR_actual_speed = 75; } // Max value: 75
    raw = ((uint32_t)(((from->MOTOR_actual_speed)))) & 0x7f;
    bytes[0] |= (((uint8_t)(raw) & 0x7f)); ///< 7 bit(s) starting from B0

    if(from->sensed_battery_voltage < 0) { from->sensed_battery_voltage = 0; } // Min value: 0
    if(from->sensed_battery_voltage > 5) { from->sensed_battery_voltage = 5; } // Max value: 5
    raw = ((uint32_t)(((from->sensed_battery_voltage) / 0.1) + 0.5)) & 0x3f;
    bytes[0] |= (((uint8_t)(raw) & 0x01) << 7); ///< 1 bit(s) starting from B7
    bytes[1] |= (((uint8_t)(raw >> 1) & 0x1f)); ///< 5 bit(s) starting from B8

    return MOTOR_FEEDBACK_HDR;
}

/// Encode and send for dbc_encode_MOTOR_FEEDBACK() message
static inline bool dbc_encode_and_send_MOTOR_FEEDBACK(MOTOR_FEEDBACK_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MOTOR_FEEDBACK(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode GEO
// 'UPDATE_CURRENT_LOCATION' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_UPDATE_CURRENT_LOCATION(uint8_t bytes[8], UPDATE_CURRENT_LOCATION_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    if(from->UPDATE_calculated_latitude < -90) { from->UPDATE_calculated_latitude = -90; } // Min value: -90
    if(from->UPDATE_calculated_latitude > 90) { from->UPDATE_calculated_latitude = 90; } // Max value: 90
    raw = ((uint32_t)(((from->UPDATE_calculated_latitude - (-90.000000)) / 1e-06) + 0.5)) & 0xfffffff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B8
    bytes[2] |= (((uint8_t)(raw >> 16) & 0xff)); ///< 8 bit(s) starting from B16
    bytes[3] |= (((uint8_t)(raw >> 24) & 0x0f)); ///< 4 bit(s) starting from B24

    if(from->UPDATE_calculated_longitude < -180) { from->UPDATE_calculated_longitude = -180; } // Min value: -180
    if(from->UPDATE_calculated_longitude > 180) { from->UPDATE_calculated_longitude = 180; } // Max value: 180
    raw = ((uint32_t)(((from->UPDATE_calculated_longitude - (-180.000000)) / 1e-06) + 0.5)) & 0x1fffffff;
    bytes[3] |= (((uint8_t)(raw) & 0x0f) << 4); ///< 4 bit(s) starting from B28
    bytes[4] |= (((uint8_t)(raw >> 4) & 0xff)); ///< 8 bit(s) starting from B32
    bytes[5] |= (((uint8_t)(raw >> 12) & 0xff)); ///< 8 bit(s) starting from B40
    bytes[6] |= (((uint8_t)(raw >> 20) & 0xff)); ///< 8 bit(s) starting from B48
    bytes[7] |= (((uint8_t)(raw >> 28) & 0x01)); ///< 1 bit(s) starting from B56

    return UPDATE_CURRENT_LOCATION_HDR;
}

/// Encode and send for dbc_encode_UPDATE_CURRENT_LOCATION() message
static inline bool dbc_encode_and_send_UPDATE_CURRENT_LOCATION(UPDATE_CURRENT_LOCATION_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_UPDATE_CURRENT_LOCATION(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode BRIDGE
// 'BRIDGE_HB' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_BRIDGE_HB(uint8_t bytes[8], BRIDGE_HB_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->BRIDGE_heartbeat)))) & 0x03;
    bytes[0] |= (((uint8_t)(raw) & 0x03)); ///< 2 bit(s) starting from B0

    return BRIDGE_HB_HDR;
}

/// Encode and send for dbc_encode_BRIDGE_HB() message
static inline bool dbc_encode_and_send_BRIDGE_HB(BRIDGE_HB_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_BRIDGE_HB(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode SENSOR
// 'SENSOR_HB' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_SENSOR_HB(uint8_t bytes[8], SENSOR_HB_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->SENSOR_heartbeat)))) & 0x03;
    bytes[0] |= (((uint8_t)(raw) & 0x03)); ///< 2 bit(s) starting from B0

    return SENSOR_HB_HDR;
}

/// Encode and send for dbc_encode_SENSOR_HB() message
static inline bool dbc_encode_and_send_SENSOR_HB(SENSOR_HB_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_SENSOR_HB(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode GEO
// 'GEO_HB' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_GEO_HB(uint8_t bytes[8], GEO_HB_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->GEO_heartbeat)))) & 0x03;
    bytes[0] |= (((uint8_t)(raw) & 0x03)); ///< 2 bit(s) starting from B0

    return GEO_HB_HDR;
}

/// Encode and send for dbc_encode_GEO_HB() message
static inline bool dbc_encode_and_send_GEO_HB(GEO_HB_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_GEO_HB(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode MOTOR
// 'MOTOR_HB' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MOTOR_HB(uint8_t bytes[8], MOTOR_HB_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->MOTOR_heartbeat)))) & 0x03;
    bytes[0] |= (((uint8_t)(raw) & 0x03)); ///< 2 bit(s) starting from B0

    return MOTOR_HB_HDR;
}

/// Encode and send for dbc_encode_MOTOR_HB() message
static inline bool dbc_encode_and_send_MOTOR_HB(MOTOR_HB_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MOTOR_HB(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Decode MASTER
// 'MASTER_CONTROL' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_MASTER_CONTROL(MASTER_CONTROL_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != MASTER_CONTROL_HDR.dlc || hdr->mid != MASTER_CONTROL_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]) & 0x01)); ///< 1 bit(s) from B0
    to->MASTER_CONTROL_cmd = (MASTER_CONTROL_cmd_E)((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode SENSOR's 'SENSOR_DATA' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_SENSOR_DATA(SENSOR_DATA_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != SENSOR_DATA_HDR.dlc || hdr->mid != SENSOR_DATA_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    to->SENSOR_left_sensor = ((raw));
    raw  = ((uint32_t)((bytes[1]))); ///< 8 bit(s) from B8
    to->SENSOR_middle_sensor = ((raw));
    raw  = ((uint32_t)((bytes[2]))); ///< 8 bit(s) from B16
    to->SENSOR_right_sensor = ((raw));
    raw  = ((uint32_t)((bytes[3]))); ///< 8 bit(s) from B24
    to->SENSOR_back_sensor = ((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode BRIDGE
// 'BRIDGE_START_STOP' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_BRIDGE_START_STOP(BRIDGE_START_STOP_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != BRIDGE_START_STOP_HDR.dlc || hdr->mid != BRIDGE_START_STOP_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]) & 0x01)); ///< 1 bit(s) from B0
    to->BRIDGE_START_STOP_cmd = ((raw));
    raw  = ((uint32_t)((bytes[0] >> 1) & 0x7f)); ///< 7 bit(s) from B1
    raw |= ((uint32_t)((bytes[1]))) << 7; ///< 8 bit(s) from B8
    raw |= ((uint32_t)((bytes[2]))) << 15; ///< 8 bit(s) from B16
    raw |= ((uint32_t)((bytes[3]) & 0x1f)) << 23; ///< 5 bit(s) from B24
    to->BRIDGE_CHECKPOINT_latitude = ((raw * 1e-06) + (-90.000000));
    raw  = ((uint32_t)((bytes[3] >> 5) & 0x07)); ///< 3 bit(s) from B29
    raw |= ((uint32_t)((bytes[4]))) << 3; ///< 8 bit(s) from B32
    raw |= ((uint32_t)((bytes[5]))) << 11; ///< 8 bit(s) from B40
    raw |= ((uint32_t)((bytes[6]))) << 19; ///< 8 bit(s) from B48
    raw |= ((uint32_t)((bytes[7]) & 0x03)) << 27; ///< 2 bit(s) from B56
    to->BRIDGE_CHECKPOINT_longitude = ((raw * 1e-06) + (-180.000000));
    raw  = ((uint32_t)((bytes[7] >> 2) & 0x01)); ///< 1 bit(s) from B58
    to->BRIDGE_FINAL_COORDINATE = ((raw));
    raw  = ((uint32_t)((bytes[7] >> 3) & 0x01)); ///< 1 bit(s) from B59
    to->BRIDGE_COORDINATE_READY = ((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode GEO
// 'GEO_DATA' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_GEO_DATA(GEO_DATA_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != GEO_DATA_HDR.dlc || hdr->mid != GEO_DATA_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    raw |= ((uint32_t)((bytes[1]) & 0x01)) << 8; ///< 1 bit(s) from B8
    if (raw & (1 << 8)) { // Check signed bit
        to->GEO_bearing_angle = ((((0xFFFFFFFF << 8) | raw)));
    } else {
        to->GEO_bearing_angle = ((raw));
    }
    raw  = ((uint32_t)((bytes[1] >> 1) & 0x7f)); ///< 7 bit(s) from B9
    raw |= ((uint32_t)((bytes[2]) & 0x01)) << 7; ///< 1 bit(s) from B16
    to->GEO_distance_to_checkpoint = ((raw));
    raw  = ((uint32_t)((bytes[2] >> 1) & 0x01)); ///< 1 bit(s) from B17
    to->GEO_destination_reached = ((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode MASTER
// 'MOTOR_UPDATE' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_MOTOR_UPDATE(MOTOR_UPDATE_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != MOTOR_UPDATE_HDR.dlc || hdr->mid != MOTOR_UPDATE_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    if (raw & (1 << 7)) { // Check signed bit
        to->MOTOR_speed = ((((0xFFFFFFFF << 7) | raw)));
    } else {
        to->MOTOR_speed = ((raw));
    }
    raw  = ((uint32_t)((bytes[1]) & 0x3f)); ///< 6 bit(s) from B8
    if (raw & (1 << 5)) { // Check signed bit
        to->MOTOR_turn_angle = ((((0xFFFFFFFF << 5) | raw)));
    } else {
        to->MOTOR_turn_angle = ((raw));
    }

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode GEO
// 'GEO_UPDATE_LOCATION' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_GEO_UPDATE_LOCATION(GEO_UPDATE_LOCATION_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != GEO_UPDATE_LOCATION_HDR.dlc || hdr->mid != GEO_UPDATE_LOCATION_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    raw |= ((uint32_t)((bytes[1]))) << 8; ///< 8 bit(s) from B8
    raw |= ((uint32_t)((bytes[2]))) << 16; ///< 8 bit(s) from B16
    raw |= ((uint32_t)((bytes[3]) & 0x0f)) << 24; ///< 4 bit(s) from B24
    to->GEO_update_latitude = ((raw * 1e-06) + (-90.000000));
    raw  = ((uint32_t)((bytes[3] >> 4) & 0x0f)); ///< 4 bit(s) from B28
    raw |= ((uint32_t)((bytes[4]))) << 4; ///< 8 bit(s) from B32
    raw |= ((uint32_t)((bytes[5]))) << 12; ///< 8 bit(s) from B40
    raw |= ((uint32_t)((bytes[6]))) << 20; ///< 8 bit(s) from B48
    raw |= ((uint32_t)((bytes[7]) & 0x01)) << 28; ///< 1 bit(s) from B56
    to->GEO_update_longitude = ((raw * 1e-06) + (-180.000000));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode MOTOR
// 'MOTOR_FEEDBACK' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_MOTOR_FEEDBACK(MOTOR_FEEDBACK_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != MOTOR_FEEDBACK_HDR.dlc || hdr->mid != MOTOR_FEEDBACK_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]) & 0x7f)); ///< 7 bit(s) from B0
    to->MOTOR_actual_speed = ((raw));
    raw  = ((uint32_t)((bytes[0] >> 7) & 0x01)); ///< 1 bit(s) from B7
    raw |= ((uint32_t)((bytes[1]) & 0x1f)) << 1; ///< 5 bit(s) from B8
    to->sensed_battery_voltage = ((raw * 0.1));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode GEO
// 'UPDATE_CURRENT_LOCATION' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_UPDATE_CURRENT_LOCATION(UPDATE_CURRENT_LOCATION_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != UPDATE_CURRENT_LOCATION_HDR.dlc || hdr->mid != UPDATE_CURRENT_LOCATION_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    raw |= ((uint32_t)((bytes[1]))) << 8; ///< 8 bit(s) from B8
    raw |= ((uint32_t)((bytes[2]))) << 16; ///< 8 bit(s) from B16
    raw |= ((uint32_t)((bytes[3]) & 0x0f)) << 24; ///< 4 bit(s) from B24
    to->UPDATE_calculated_latitude = ((raw * 1e-06) + (-90.000000));
    raw  = ((uint32_t)((bytes[3] >> 4) & 0x0f)); ///< 4 bit(s) from B28
    raw |= ((uint32_t)((bytes[4]))) << 4; ///< 8 bit(s) from B32
    raw |= ((uint32_t)((bytes[5]))) << 12; ///< 8 bit(s) from B40
    raw |= ((uint32_t)((bytes[6]))) << 20; ///< 8 bit(s) from B48
    raw |= ((uint32_t)((bytes[7]) & 0x01)) << 28; ///< 1 bit(s) from B56
    to->UPDATE_calculated_longitude = ((raw * 1e-06) + (-180.000000));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode BRIDGE
// 'BRIDGE_HB' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_BRIDGE_HB(BRIDGE_HB_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != BRIDGE_HB_HDR.dlc || hdr->mid != BRIDGE_HB_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]) & 0x03)); ///< 2 bit(s) from B0
    to->BRIDGE_heartbeat = ((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode SENSOR
// 'SENSOR_HB' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_SENSOR_HB(SENSOR_HB_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != SENSOR_HB_HDR.dlc || hdr->mid != SENSOR_HB_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]) & 0x03)); ///< 2 bit(s) from B0
    to->SENSOR_heartbeat = ((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode GEO
// 'GEO_HB' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_GEO_HB(GEO_HB_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != GEO_HB_HDR.dlc || hdr->mid != GEO_HB_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]) & 0x03)); ///< 2 bit(s) from B0
    to->GEO_heartbeat = ((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode MOTOR
// 'MOTOR_HB' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_MOTOR_HB(MOTOR_HB_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != MOTOR_HB_HDR.dlc || hdr->mid != MOTOR_HB_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]) & 0x03)); ///< 2 bit(s) from B0
    to->MOTOR_heartbeat = ((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Handle the MIA for MASTER
// MASTER_CONTROL message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_MASTER_CONTROL(MASTER_CONTROL_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MASTER_CONTROL__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MASTER_CONTROL__MIA_MSG;
        msg->mia_info.mia_counter_ms = MASTER_CONTROL__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's SENSOR_DATA message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_SENSOR_DATA(SENSOR_DATA_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_DATA__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_DATA__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_DATA__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for BRIDGE
// BRIDGE_START_STOP message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_BRIDGE_START_STOP(BRIDGE_START_STOP_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BRIDGE_START_STOP__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BRIDGE_START_STOP__MIA_MSG;
        msg->mia_info.mia_counter_ms = BRIDGE_START_STOP__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO
// GEO_DATA message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_GEO_DATA(GEO_DATA_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GEO_DATA__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GEO_DATA__MIA_MSG;
        msg->mia_info.mia_counter_ms = GEO_DATA__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MASTER
// MOTOR_UPDATE message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_MOTOR_UPDATE(MOTOR_UPDATE_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTOR_UPDATE__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTOR_UPDATE__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTOR_UPDATE__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO
// GEO_UPDATE_LOCATION message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_GEO_UPDATE_LOCATION(GEO_UPDATE_LOCATION_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GEO_UPDATE_LOCATION__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GEO_UPDATE_LOCATION__MIA_MSG;
        msg->mia_info.mia_counter_ms = GEO_UPDATE_LOCATION__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MOTOR
// MOTOR_FEEDBACK message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_MOTOR_FEEDBACK(MOTOR_FEEDBACK_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTOR_FEEDBACK__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTOR_FEEDBACK__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTOR_FEEDBACK__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO
// UPDATE_CURRENT_LOCATION message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_UPDATE_CURRENT_LOCATION(UPDATE_CURRENT_LOCATION_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= UPDATE_CURRENT_LOCATION__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = UPDATE_CURRENT_LOCATION__MIA_MSG;
        msg->mia_info.mia_counter_ms = UPDATE_CURRENT_LOCATION__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for BRIDGE
// BRIDGE_HB message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_BRIDGE_HB(BRIDGE_HB_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BRIDGE_HB__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BRIDGE_HB__MIA_MSG;
        msg->mia_info.mia_counter_ms = BRIDGE_HB__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR
// SENSOR_HB message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_SENSOR_HB(SENSOR_HB_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_HB__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_HB__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_HB__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO
// GEO_HB message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_GEO_HB(GEO_HB_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GEO_HB__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GEO_HB__MIA_MSG;
        msg->mia_info.mia_counter_ms = GEO_HB__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MOTOR
// MOTOR_HB message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_MOTOR_HB(MOTOR_HB_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTOR_HB__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTOR_HB__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTOR_HB__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

#endif
