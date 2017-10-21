/*
 * GeoGPS.h
 *
 *  Created on: Oct 21, 2017
 *      Author: abhilash
 */

#ifndef GEO_GPS_H_
#define GEO_GPS_H_
#include <string>

class GeoGPS {
    float latitude, longitude;
public:
    float GetLongitude();
    float GetLatitude();
    void SetLongitude(float long_val);
    void SetLatitude(float lat_val);
    void ParseGPSString(std::string gps_str);
};

#endif /* GEO_GPS_H_ */
