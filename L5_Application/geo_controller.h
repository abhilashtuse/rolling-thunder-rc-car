/*
 * geo_controller.h
 *
 *  Created on: Oct 26, 2017
 *      Author: abhilash
 */

#ifndef GEO_CONTROLLER_H_
#define GEO_CONTROLLER_H_

#include <cmath>
#include <list>
#include "geo_gps.h"

class GeoController {

    bool updateCheckpointFlag;
    list<double> checkpointLatitude;
    list<double> checkpointLongitude;
    const double distance_threshold = 2.0;

    double DegreeToRadian( double degree )
    {
        return ((degree * M_PI) / 180);
    }
public:

    bool ManipulateCheckpointList(GeoGPS geo_gps);
    bool isFinalDestinationReached(double distance);
    double CalculateBearingAngle(GeoGPS geo_gps);
    double CalculateDistance(GeoGPS geo_gps);
    double CalculateHeadingAngle(GeoGPS geo_gps, double compassBearingAngle);

    const list<double>& getcheckpoint_latitude() const
    {
        return checkpointLatitude;
    }

    void setcheckpoint_latitude(const list<double>& checkpoint_latitude)
    {
        checkpointLatitude = checkpoint_latitude;
    }

    const list<double>& getcheckpoint_longitude() const
    {
        return checkpointLongitude;
    }

    void setcheckpoint_longitude(const list<double>& checkpoint_longitude)
    {
        checkpointLongitude = checkpoint_longitude;
    }

    bool isupdate_checkpoint_flag() const
    {
        return updateCheckpointFlag;
    }

    void setupdate_checkpoint_flag(bool update_checkpoint_flag= true)
    {
        updateCheckpointFlag = update_checkpoint_flag;
    }
};

#endif /* GEO_CONTROLLER_H_ */
