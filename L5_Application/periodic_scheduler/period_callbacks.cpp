/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <list>
#include "io.hpp"
#include "periodic_callback.h"
#include "can.h"
#include "_can_dbc/generated_can.h"
#include "uart3.hpp"
#include "geo_gps.h"
#include "geo_controller.h"
#include "geo_compass.h"
using namespace std;

#define PRINT_CHECKPOINTS 1
int print_flag = 0;
// Not using MIA for now. Checkpoints are not received periodically
// const uint32_t             BRIDGE_START_STOP__MIA_MS = 3000;
// const BRIDGE_START_STOP_t  BRIDGE_START_STOP__MIA_MSG = { 0 };

/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_MONITOR_TASK_STACK_SIZE_BYTES = (512 * 3);
static GeoGPS geo_gps;
static GeoCompass geo_compass;
static GeoController geoController;
static Uart3 &serialPort3 = Uart3::getInstance();
static double compass_bearing_angle;

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
    serialPort3.init(38400, 500, 0); /* Init baud rate */
    CAN_init(can1,100,50,50,NULL,NULL);
    CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus( can1);
    geoController.setupdate_checkpoint_flag(true);

    //testing Open
   // static list<double> latitudeList;
    //static list<double> longitudeList;
    /*latitudeList.push_back(37.337135);
    longitudeList.push_back(-121.880138);
    latitudeList.push_back(37.337247);
    longitudeList.push_back(-121.879862);
    latitudeList.push_back(37.337199);
    longitudeList.push_back(-121.879846);
    latitudeList.push_back(37.337040);
    longitudeList.push_back(-121.879722);*/ //near ATM

    /*latitudeList.push_back(37.336139);
    longitudeList.push_back(-121.881849);
    latitudeList.push_back(37.336300);
    longitudeList.push_back(-121.881893);
    latitudeList.push_back(37.336450);
    longitudeList.push_back(-121.881653);
    latitudeList.push_back(37.336556);
    longitudeList.push_back(-121.881457);

    latitudeList.push_back(37.336737);
    longitudeList.push_back(-121.880938);
    SENT:lat:37.335273 long:-121.881261

    //Student Union
    latitudeList.push_back(37.335273);
    longitudeList.push_back(-121.881261);


    geoController.setcheckpoint_latitude(latitudeList);
    geoController.setcheckpoint_longitude(longitudeList);
    geoController.setupdate_checkpoint_flag(false);*/
    //testing Close
    return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}

void compass_calibration()
{
    static int cm3_count = 0;
    static int cm_count = 0;
#if 0 //Not using full calibration mode for now
    if (SW.getSwitch(1) || (cm_count > 0 && cm_count < 9)) // switch to calibration mode
    {
        /* enter the calibration mode by sending a 3 byte sequence of 0xF0,0xF5 and then 0xF6
         * to the command register, these MUST be sent in 3 separate I2C frames, you cannot send
         * them all at once. There MUST be a minimum of 20ms between each I2C frame */
        cm_count++;
        if (cm_count == 1) {
            //printf("\n count: %d cm_count: %d", count, cm_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0xF0);
        }
        else if (cm_count == 4) {
            //printf("\n count: %d cm_count: %d", count, cm_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0xF5);
        }
        else if (cm_count == 7) {
            //printf("\n count: %d cm_count: %d", count, cm_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0xF6);
            LE.set(1, true);
        }
        printf("\n cm_count: %d", cm_count);
    }
#endif
    if (SW.getSwitch(2) && (cm_count > 7 || cm3_count > 7)) //Come out from calibration mode
    {
        //printf("\n count: %d cm_count: %d", count, cm_count);
        I2C2::getInstance().writeReg(0xc0, 0x0, 0xF8);
        cm_count = 0;
        cm3_count = 0;
        LE.set(1, false);
    }

    static int cm2_count = 0;
    if (SW.getSwitch(3) || (cm2_count > 0 && cm2_count < 9)) // Restore factory calibration
    {
        /* enter the calibration mode by sending a 3 byte sequence of 0xF0,0xF5 and then 0xF6
         * to the command register, these MUST be sent in 3 separate I2C frames, you cannot send
         * them all at once. There MUST be a minimum of 20ms between each I2C frame */
        cm2_count++;
        if (cm2_count == 1) {
            //printf("\n count: %d cm2_count: %d", count, cm2_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0x20);
        }
        else if (cm2_count == 4) {
            //printf("\n count: %d cm2_count: %d", count, cm2_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0x2A);
        }
        else if (cm2_count == 7) {
            //printf("\n count: %d cm2_count: %d", count, cm2_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0x60);
            LE.set(3, true);
        }
        //printf("\n cm2_count: %d", cm2_count);
    }

    if (SW.getSwitch(4) || (cm3_count > 0 && cm3_count < 9)) // switch to horizontal calibration mode
    {
        /* enter the calibration mode by sending a 3 byte sequence of 0xF0,0xF5 and then 0xF6
         * to the command register, these MUST be sent in 3 separate I2C frames, you cannot send
         * them all at once. There MUST be a minimum of 20ms between each I2C frame */
        cm3_count++;
        if (cm3_count == 1) {
            //printf("\n count: %d cm_count: %d", count, cm_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0xF0);
        }
        else if (cm3_count == 4) {
            //printf("\n count: %d cm_count: %d", count, cm_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0xF5);
        }
        else if (cm3_count == 7) {
            //printf("\n count: %d cm_count: %d", count, cm_count);
            I2C2::getInstance().writeReg(0xc0, 0x0, 0xF7);
            LE.set(1, true);
        }
        //printf("\n cm_count: %d", cm_count);
    }
}

/**
 * Below are your periodic functions.
 * The argument 'count' is the number of times each periodic task is called.
 */

void period_1Hz(uint32_t count)
{
    if (CAN_is_bus_off(can1)) {
        printf("\nResetting can bus.");
        CAN_reset_bus(can1);
    }
    GEO_HB_t geo_hb_cmd = { 0 };
    geo_hb_cmd.GEO_heartbeat = 1;
    dbc_encode_and_send_GEO_HB(&geo_hb_cmd);
    UPDATE_COMPASS_BEARING_t cur_compass_angle = { 0 };
    cur_compass_angle.COMPASS_bearing_angle = compass_bearing_angle;
    dbc_encode_and_send_UPDATE_COMPASS_BEARING(&cur_compass_angle);

#if PRINT_CHECKPOINTS
    if (geoController.isupdate_checkpoint_flag() == false) {
        list<double> lat = geoController.getcheckpoint_latitude();
        list<double> longi = geoController.getcheckpoint_longitude();
        std::list<double>::iterator lat_it=lat.begin();
        if (print_flag) {
        for (std::list<double>::iterator long_it=longi.begin(); long_it != longi.end(); ++lat_it,++long_it)
            printf("\nCheckpoints received: lat: %f long: %f", *lat_it, *long_it);
        }
        print_flag = 0;
    }
#endif
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

void period_10Hz(uint32_t count)
{
    /* Compass Read */
    uint8_t reg2 = I2C2::getInstance().readReg(0xc0, 0x2);
    uint8_t reg3 = I2C2::getInstance().readReg(0xc0, 0x3);
    compass_bearing_angle = geo_compass.CalculateBearingAngle(reg2, reg3);
    //printf("\n compass_bearing_angle: %f", compass_bearing_angle);
    if (count % 2) {
        char gps_str_arr[200];
        bool result = serialPort3.gets(gps_str_arr, 200, 1);
        std::string gps_str = gps_str_arr;
        // double lat = 37.336057;//gps.GetLatitude();
        //double longi = 121.336057;//gps.GetLongitude();
        //UPDATE_CURRENT_LOCATION_t cur_location = { 0 };
        //  cur_location.UPDATE_calculated_latitude = lat;
        // cur_location.UPDATE_calculated_longitude = longi;
        //dbc_encode_and_send_UPDATE_CURRENT_LOCATION(&cur_location);
        //    gps_str="$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
        //  result=1;
        if (result && gps_str.find("$GPGGA") == 0) {
            //  printf("Received: %s\n", gps_str_arr);
            bool ret = geo_gps.ParseGPSString(gps_str);
            //  double lat = geo_gps.GetLatitude();
            //double longi = geo_gps.GetLongitude();
            if (ret) {
                UPDATE_CURRENT_LOCATION_t cur_location = { 0 };
                cur_location.UPDATE_calculated_latitude = geo_gps.GetLatitude();
                cur_location.UPDATE_calculated_longitude = geo_gps.GetLongitude();
                dbc_encode_and_send_UPDATE_CURRENT_LOCATION(&cur_location);
                // printf("\nSENT:lat:%f long:%f\n", geo_gps.GetLatitude(), geo_gps.GetLongitude());
                //   printf("Lat")

                LE.toggle(3);
            }
            else
                LE.set(3,0);
        }
        if (geoController.isupdate_checkpoint_flag() == false) {
            geoController.ManipulateCheckpointList(geo_gps);
            GEO_DATA_t geo_cmd = { 0 };
            geo_cmd.GEO_bearing_angle = geoController.CalculateHeadingAngle(geo_gps, compass_bearing_angle);
            // printf("\n heading_angle: %d", geo_cmd.GEO_bearing_angle);
            geo_cmd.GEO_distance_to_checkpoint =geoController.CalculateDistance(geo_gps);
            geo_cmd.GEO_destination_reached = geoController.isFinalDestinationReached(geo_cmd.GEO_distance_to_checkpoint) ? 1 : 0;

            // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
            if (dbc_encode_and_send_GEO_DATA(&geo_cmd)){
                LD.setNumber(geo_cmd.GEO_distance_to_checkpoint);
                // printf("\nsent to master: bearing_angle:%f distance:%f dest:%d", geo_cmd.GEO_bearing_angle, geo_cmd.GEO_distance_to_checkpoint, geo_cmd.GEO_destination_reached);
            }
            else {
                LE.toggle(4);
                printf("\ntx failed");
            }
        }
    }
}

void period_100Hz(uint32_t count)
{
    MASTER_CONTROL_t master_can_msg;
    BRIDGE_START_STOP_t checkPoint = { 0 };
    can_msg_t can_msg = { 0 };
    compass_calibration();

    static list<double> latitudeList;
    static list<double> longitudeList;

    while (CAN_rx(can1, &can_msg, 0))
    {
        // Form the message header from the metadata of the arriving message
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;
        switch (can_msg_hdr.mid)
        {
            case 150: //original value 150 change to 1500 for testing
                dbc_decode_BRIDGE_START_STOP(&checkPoint, can_msg.data.bytes, &can_msg_hdr);
                //     printf("\nCheckpoints received: lat: %f long: %f", checkPoint.BRIDGE_CHECKPOINT_latitude, checkPoint.BRIDGE_CHECKPOINT_longitude);
                if(geoController.isupdate_checkpoint_flag()){
                    latitudeList.push_back(checkPoint.BRIDGE_CHECKPOINT_latitude);
                    longitudeList.push_back(checkPoint.BRIDGE_CHECKPOINT_longitude);
                }

                if(checkPoint.BRIDGE_FINAL_COORDINATE==1){
                    geoController.setupdate_checkpoint_flag(false);
                    geoController.setcheckpoint_latitude(latitudeList);
                    geoController.setcheckpoint_longitude(longitudeList);
                    latitudeList.clear();
                    longitudeList.clear();
                    print_flag = 1;
                }
                break;
        }
    }
    // Not doing MIA because Checkpoints are received one time from bridge
    // dbc_handle_mia_BRIDGE_START_STOP(&checkPoint, 10);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    //LE.toggle(4);
}
