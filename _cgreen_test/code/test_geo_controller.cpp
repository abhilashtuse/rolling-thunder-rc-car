#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

using namespace cgreen;

// The file under test
#include "geo_controller.h"

Ensure(test_CalculateDistance_zero)
{
    GeoController geo_controller;
    GeoGPS geo_gps;
    double latitude, longitude, distance;
    list<double> checkpointLatitude;
    list<double> checkpointLongitude;

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.038", true);
    longitude = geo_gps.ExtractCoordinate("01131.000", false);
    checkpointLatitude.push_back(latitude);
    checkpointLongitude.push_back(longitude);
    geo_controller.setcheckpoint_latitude(checkpointLatitude);
    geo_controller.setcheckpoint_longitude(checkpointLongitude);
    distance = geo_controller.CalculateDistance(geo_gps);
    assert_that_double(distance, is_equal_to_double(0));
}

Ensure(test_CalculateDistance_North_East_nonzero)
{
    GeoController geo_controller;
    GeoGPS geo_gps;
    double latitude, longitude, distance;
    list<double> checkpointLatitude;
    list<double> checkpointLongitude;

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.000", true);
    longitude = geo_gps.ExtractCoordinate("01131.000", false);
    checkpointLatitude.push_back(latitude);
    checkpointLongitude.push_back(longitude);
    geo_controller.setcheckpoint_latitude(checkpointLatitude);
    geo_controller.setcheckpoint_longitude(checkpointLongitude);
    distance = geo_controller.CalculateDistance(geo_gps);
    assert_that_double(distance, is_equal_to_double(70.445561));
}

Ensure(test_CalculateDistance_South_West_nonzero)
{
    GeoController geo_controller;
    GeoGPS geo_gps;
    double latitude, longitude, distance;
    list<double> checkpointLatitude;
    list<double> checkpointLongitude;

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,S,01131.000,W,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.000", true)*-1;
    longitude = geo_gps.ExtractCoordinate("01131.000", false)*-1;
    checkpointLatitude.push_back(latitude);
    checkpointLongitude.push_back(longitude);
    geo_controller.setcheckpoint_latitude(checkpointLatitude);
    geo_controller.setcheckpoint_longitude(checkpointLongitude);
    distance = geo_controller.CalculateDistance(geo_gps);
    assert_that_double(distance, is_equal_to_double(70.445561));
}

Ensure(test_CalculateBearingAngle_zero)
{
    GeoController geo_controller;
    GeoGPS geo_gps;

    double latitude, longitude, bearing_angle;
    list<double> checkpointLatitude;
    list<double> checkpointLongitude;

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.038", true);
    longitude = geo_gps.ExtractCoordinate("01131.000", false);
    checkpointLatitude.push_back(latitude);
    checkpointLongitude.push_back(longitude);
    geo_controller.setcheckpoint_latitude(checkpointLatitude);
    geo_controller.setcheckpoint_longitude(checkpointLongitude);
    bearing_angle = geo_controller.CalculateBearingAngle(geo_gps);
    assert_that(bearing_angle == 0);
}

Ensure(test_CalculateBearingAngle_nonzero)
{
    GeoController geo_controller;
    GeoGPS geo_gps;

    double latitude, longitude, bearing_angle;
    list<double> checkpointLatitude;
    list<double> checkpointLongitude;

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.038", true);
    longitude = geo_gps.ExtractCoordinate("01132.000", false);
    checkpointLatitude.push_back(latitude);
    checkpointLongitude.push_back(longitude);
    geo_controller.setcheckpoint_latitude(checkpointLatitude);
    geo_controller.setcheckpoint_longitude(checkpointLongitude);
    bearing_angle = geo_controller.CalculateBearingAngle(geo_gps);
    assert_that_double(bearing_angle, is_equal_to_double(1.570688));
}
TestSuite *geo_controller_test_suite()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, test_CalculateDistance_zero);
    add_test(suite, test_CalculateDistance_North_East_nonzero);
    add_test(suite, test_CalculateDistance_South_West_nonzero);
    add_test(suite, test_CalculateBearingAngle_zero);
    add_test(suite, test_CalculateBearingAngle_nonzero);
    return suite;
}

