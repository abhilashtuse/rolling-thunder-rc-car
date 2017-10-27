#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

using namespace cgreen;

// The file under test
#include "geo_controller.h"

Ensure(test_CalculateDistance)
{
    GeoController geo_controller;
    GeoGPS geo_gps;
    double latitude, longitude, distance;

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.038", true);
    longitude = geo_gps.ExtractCoordinate("01131.000", false);
    distance = geo_controller.CalculateDistance(latitude, longitude, geo_gps);
    assert_that_double(distance, is_equal_to_double(0));

    latitude = geo_gps.ExtractCoordinate("4807.000", true);
    longitude = geo_gps.ExtractCoordinate("01131.000", false);
    distance = geo_controller.CalculateDistance(latitude, longitude, geo_gps);
    assert_that_double(distance, is_equal_to_double(70.445561));

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,S,01131.000,W,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.000", true)*-1;
    longitude = geo_gps.ExtractCoordinate("01131.000", false)*-1;
    distance = geo_controller.CalculateDistance(latitude, longitude, geo_gps);
    assert_that_double(distance, is_equal_to_double(70.445561));
}

Ensure(test_CalculateBearingAngle)
{
    GeoController geo_controller;
    GeoGPS geo_gps;

    double latitude, longitude, bearing_angle;

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.038", true);
    longitude = geo_gps.ExtractCoordinate("01131.000", false);
    bearing_angle = geo_controller.CalculateBearingAngle(latitude, longitude, geo_gps);
    assert_that(bearing_angle == 0);

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.ExtractCoordinate("4807.038", true);
    longitude = geo_gps.ExtractCoordinate("01132.000", false);
    bearing_angle = geo_controller.CalculateBearingAngle(latitude, longitude, geo_gps);
    assert_that_double(bearing_angle, is_equal_to_double(1.570688));
}

TestSuite *geo_controller_test_suite()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, test_CalculateDistance);
    add_test(suite, test_CalculateBearingAngle);
    return suite;
}

