/*
 * geo_compass.h
 *
 *  Created on: Oct 27, 2017
 *      Author: abhilash
 */

#ifndef GEO_COMPASS_H_
#define GEO_COMPASS_H_

#include <cstdint>

class GeoCompass {
public:
    double CalculateBearingAngle(uint8_t reg2, uint8_t reg3);
};

#endif /* GEO_COMPASS_H_ */
