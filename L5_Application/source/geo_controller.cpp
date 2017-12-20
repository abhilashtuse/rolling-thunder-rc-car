/*
 * geo_controller.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: abhilash
 */

#include <iostream>
#include <stdio.h>
#include "geo_controller.h"

bool GeoController::isFinalDestinationReached(double distance) {
    if (checkpointLatitude.size() == 1 && checkpointLongitude.size() == 1 && distance <= distance_threshold) {
        updateCheckpointFlag = true;
        checkpointLatitude.clear();
        checkpointLongitude.clear();
        return true;
    }
    return false;
}

bool GeoController::ManipulateCheckpointList(GeoGPS geo_gps)
{
    double distance = CalculateDistance(geo_gps);
    if (distance <= distance_threshold && checkpointLatitude.size() > 1 && checkpointLongitude.size() > 1) {
        printf("\n ManipulateCheckpointList before: %f %f",checkpointLatitude.front(),checkpointLongitude.front());
        checkpointLatitude.pop_front();
        checkpointLongitude.pop_front();
        printf("\n ManipulateCheckpointList after: %f %f",checkpointLatitude.front(),checkpointLongitude.front());
        return true;
    }
    return false;
}
/*
 * Bearing (θ)(radians) = atan2(sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ)

      where,
             φ1,λ1 is the start point,
             φ2,λ2 the end point
             Δλ is the difference in longitude
 *
 * For two same lat and long this function returns 1.57 degree bearing angle.
 * */

double GeoController::CalculateBearingAngle(GeoGPS geo_gps)
{
    //printf("\n1:lat:%f long:%f\n", geo_gps.GetLatitude(), geo_gps.GetLongitude());
    //printf("\n2:lat:%f long:%f\n", latitude, longitude);
    //printf("\nBefore:lat:%f long:%f\n", checkpointLatitude.front(), checkpointLongitude.front());
    double latitude = DegreeToRadian(checkpointLatitude.front());
    double longitude = DegreeToRadian(checkpointLongitude.front());
    double gps_latitude = DegreeToRadian(geo_gps.GetLatitude());
    double gps_longitude = DegreeToRadian(geo_gps.GetLongitude());

    double diff_longitude = longitude - gps_longitude;
    double bearing = atan2(sin(diff_longitude) * cos(latitude),
            (cos(gps_latitude) * sin(latitude)) -
            sin(gps_latitude) * cos(latitude) * cos(diff_longitude));

    return (bearing * 180)/M_PI;
}

/*
 * Distance = R ⋅ c
 c = 2 ⋅ atan2(√a, √(1−a))
 a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)

 where,
       φ is latitude,
       λ is longitude,
       R is earth’s radius (mean radius = 6,371km),

 Note: Angles need to be in radians to pass to trignometric functions
 * For two same lat and long this function returns 0.02 meter distance. */
double GeoController::CalculateDistance(GeoGPS geo_gps)
{

    //printf("\n2:lat:%f long:%f\n", latitude, longitude);
    //  printf("\nBefore:lat:%f long:%f\n", checkpointLatitude.front(), checkpointLongitude.front());
    //    string ch_lat = geo_gps.DoubleToString(checkpointLatitude.front());
    //    string ch_long = geo_gps.DoubleToString(checkpointLongitude.front());
    //    double d_lat = geo_gps.StringToDouble(ch_lat);
    //    double d_long = geo_gps.StringToDouble(ch_long);
    //printf("\nAfter:lat:%f long:%f\n", d_lat, d_long);

    //   double latitude = DegreeToRadian(d_lat);
    // double longitude = DegreeToRadian(d_long);
    //  printf("\nGPS:lat:%f long:%f\n", geo_gps.GetLatitude(), geo_gps.GetLongitude());
    //printf("\nBRIDGE:lat:%f long:%f\n", checkpointLatitude.front(), checkpointLongitude.front());
    double latitude = DegreeToRadian(checkpointLatitude.front());
    double longitude = DegreeToRadian(checkpointLongitude.front());

    //printf("\n3:lat:%f long:%f\n", latitude, longitude);
    double gps_latitude = DegreeToRadian(/*48.1167);//*/geo_gps.GetLatitude());
    double gps_longitude = DegreeToRadian(/*11.5167);*/geo_gps.GetLongitude());

    double earth_radius_m = 6373000;
    double diff_latitude = latitude - gps_latitude;
    double diff_longitude = longitude - gps_longitude;
    double a = (sin(diff_latitude / 2) * sin(diff_latitude / 2)) +
            (cos(gps_latitude) * cos(latitude) *
                    sin(diff_longitude / 2) * sin(diff_longitude / 2));

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return earth_radius_m * c;
}

double GeoController::CalculateHeadingAngle(GeoGPS geo_gps, double compassBearingAngle)
{
    double geoBearingAngle = CalculateBearingAngle(geo_gps);
    double headingAngle= geoBearingAngle-compassBearingAngle;
    // printf("\n Gps bearing angle: %f    Compass Bearing Angle:%f", geoBearingAngle, compassBearingAngle);

    if (headingAngle > 0 && headingAngle <= 180) {
        // headingAngle = headingAngle;
    }
    else if (headingAngle > 180 && headingAngle < 360) {
        headingAngle = (360 - headingAngle) *(-1);
    }
    else if (headingAngle < 0 && headingAngle >= -180) {
        //  headingAngle = (360 - headingAngle) *(-1);
    }
    else if (headingAngle < -180 && headingAngle > -360) {
        headingAngle = (360 + headingAngle);
    }
    //printf("\n Gps bearing angle: %f  Compass Bearing Angle:%f  Heading Angle: %f ", geoBearingAngle, compassBearingAngle, headingAngle );
    //printf("\n Heading Angle: %f ", headingAngle);
    return headingAngle;
}
