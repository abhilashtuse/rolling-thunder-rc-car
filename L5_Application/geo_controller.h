/*
 * geo_controller.h
 *
 *  Created on: Oct 26, 2017
 *      Author: abhilash
 */

#ifndef GEO_CONTROLLER_H_
#define GEO_CONTROLLER_H_

#include <cmath>
#include "geo_gps.h"

class GeoController {
    double DegreeToRadian( double degree )
    {
        return ((degree * M_PI) / 180);
    }
public:
    double CalculateBearingAngle(double latitude, double longitude, GeoGPS geo_gps);
    double CalculateDistance(double latitude, double longitude, GeoGPS geo_gps);
};

#endif /* GEO_CONTROLLER_H_ */
