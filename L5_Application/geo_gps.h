/*
 * GeoGPS.h
 *
 *  Created on: Oct 21, 2017
 *      Author: abhilash
 */

#ifndef GEO_GPS_H_
#define GEO_GPS_H_
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <cstdint>

#define MINUTE_LENGTH 9 // Length of minutes data in NMEA

using namespace std;

class GeoGPS {
    double latitude, longitude;
    vector<string> splitStringByComma(string input);
    double DegreesToDecimal(int degrees, double minutes);
    double StringToDouble(string inputString);
    int StringToInt(string inputString);

public:
    GeoGPS(): latitude(0), longitude(0) {}
    double GetLongitude();
    double GetLatitude();
    void SetLongitude(double long_val);
    void SetLatitude(double lat_val);
    void SetValuesGGA(string GGA);
    void ParseGPSString(std::string gps_str);
    double ExtractCoordinate(string raw_coordinate, bool is_lat);
    bool IsValidGGA(const string GGASentence);
};

#endif /* GEO_GPS_H_ */
