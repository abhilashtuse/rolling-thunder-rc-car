/*
 * GeoGPS.cpp
 *
 *  Created on: Oct 21, 2017
 *      Author: abhilash
 */

#include <cstdlib>
#include "geo_gps.h"

void GeoGPS::SetLongitude(double long_val)
{
    longitude = long_val;
}

void GeoGPS::SetLatitude(double lat_val)
{
    latitude = lat_val;
}

double GeoGPS::GetLongitude()
{
    return longitude;
}

double GeoGPS::GetLatitude()
{
    return latitude;
}

// Checks if GGA sentence is valid.
bool GeoGPS::IsValidGGA(const string GGASentence)
{
    bool returnBool = true;
    vector<std::string> elementVector = splitStringByComma(GGASentence);

    if (elementVector[0] != "$GPGGA")               returnBool = false;
    if (elementVector.size() != 15)                 returnBool = false;
    if (atoi(elementVector[6].c_str()) == 0)        returnBool = false;
    /* Below checks not working */
    //if (elementVector[4].length() < MINUTE_LENGTH)  returnBool = false; // Confirm with field testing
    //if (elementVector[2].length() < MINUTE_LENGTH)  returnBool = false;
    if (atoi(elementVector[7].c_str()) == 0)        returnBool = false;

    return returnBool;
}

vector<string> GeoGPS::splitStringByComma(string input)
{
    vector<string>  returnVector;
    stringstream    ss(input);
    string          element;

    while(std::getline(ss, element, ',')) {
        returnVector.push_back(element);
    }
    return returnVector;
}

double GeoGPS::DegreesToDecimal(int degrees, double minutes)
{
    double returnDouble = 0;
    returnDouble = degrees + minutes / 60;
    return returnDouble;
}

// Input: GGA NMEA sentence
// Output: set values in class.
void GeoGPS::ParseGPSString(string GGA)
{
    if (IsValidGGA(GGA)) {
        vector<std::string> elementVector = splitStringByComma(GGA);
        // Assert we have a GGA sentence
        if (elementVector[0] == "$GPGGA") {
            latitude = ExtractCoordinate(elementVector[2], true);
            if (elementVector[3] == "S")
                latitude = -latitude;
            longitude = ExtractCoordinate(elementVector[4], false);
            if (elementVector[5] == "W")
                longitude = -longitude;
        }
    }
}

int GeoGPS::StringToInt(string inputString)
{
    //If string empty, return 0.
    int returnValue = 0;
    std::istringstream istr(inputString);

    istr >> returnValue;

    return (returnValue);

}

double GeoGPS::StringToDouble(string inputString)
{
    //If string empty, return 0.
    double returnValue = 0;
    std::istringstream istr(inputString);

    istr >> returnValue;

    return (returnValue);

}

double GeoGPS::ExtractCoordinate(string raw_coordinate, bool is_lat)
{
    double decimalDegrees = 0;
    uint8_t degrees = 0;
    double minutes = 0;
    uint8_t degree_len = (is_lat == true) ? 2 : 3;

    degrees = StringToInt(raw_coordinate.substr(0, degree_len).c_str());
    minutes = StringToDouble(raw_coordinate.substr(degree_len, raw_coordinate.length() - degree_len));

    // Convert degrees and minutes into decimal
    decimalDegrees = DegreesToDecimal(degrees, minutes);

    return decimalDegrees;
}
