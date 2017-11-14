/*
 * geo_compass.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: abhilash
 */

#include "geo_compass.h"

double GeoCompass::CalculateBearingAngle(uint8_t reg2, uint8_t reg3)
{
    double bearing_angle = 0;
    uint16_t reg = 0;
    reg = (reg2 << 8);
    reg |= reg3;
    bearing_angle = (reg / 10.0);
//    if (bearing_angle > 180) {
//        bearing_angle -= 360;
//    }
    return bearing_angle;
}

