/*
 * GeoGPS.cpp
 *
 *  Created on: Oct 21, 2017
 *      Author: abhilash
 */

#include "geo_gps.h"

void GeoGPS::SetLongitude(float long_val)
{
    longitude = long_val;
}

void GeoGPS::SetLatitude(float lat_val)
{
    latitude = lat_val;
}

float GeoGPS::GetLongitude()
{
    return longitude;
}

float GeoGPS::GetLatitude()
{
    return latitude;
}

void GeoGPS::ParseGPSString(std::string gps_str)
{
    latitude = 0;
    longitude = 0;
}
