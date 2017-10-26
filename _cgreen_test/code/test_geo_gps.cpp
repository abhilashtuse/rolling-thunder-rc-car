/*
 * GeoGPS_test.cpp
 *
 *  Created on: Oct 21, 2017
 *      Author: abhilash
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

using namespace cgreen;

// The file under test
#include "geo_gps.h"


Ensure(test_ExtractCoordinate)
{
    GeoGPS geo_gps;
    double latitude = geo_gps.ExtractCoordinate("4124.8963", true);
    assert_that_double(latitude, is_equal_to_double(41.414938333));
    double longitude = geo_gps.ExtractCoordinate("14124.8963", false);
    assert_that_double(longitude, is_equal_to_double(141.414938333));
}

Ensure(test_ParseGPSString)
{
    double latitude, longitude;
    GeoGPS geo_gps;

    geo_gps.ParseGPSString("$GPRMC,123519,4807.038,S,01131.000,W,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.GetLatitude();
    assert_that(latitude == 0);
    longitude = geo_gps.GetLongitude();
    assert_that(longitude == 0);

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.GetLatitude();
    assert_that_double(latitude, is_equal_to_double(48.1173));
    longitude = geo_gps.GetLongitude();
    assert_that_double(longitude, is_equal_to_double(11.516666667));

    geo_gps.ParseGPSString("$GPGGA,123519,4807.038,S,01131.000,W,1,08,0.9,545.4,M,46.9,M,,*47");
    latitude = geo_gps.GetLatitude();
    assert_that_double(latitude, is_equal_to_double(-48.1173));
    longitude = geo_gps.GetLongitude();
    assert_that_double(longitude, is_equal_to_double(-11.516666667));
}

Ensure(test_IsValidGGA)
{
    GeoGPS geo_gps;
    bool result = geo_gps.IsValidGGA("$GPGGA,123519,4807.038,S,01131.000,W,1,08,0.9,545.4,M,46.9,M,,*47");
    assert_true(result);
    result = geo_gps.IsValidGGA("$GPGGA,123519,4807.038,S,01131.000,W,0,08,0.9,545.4,M,46.9,M,,*47");
    assert_false(result);
    result = geo_gps.IsValidGGA("$GPGGA,123519,4807.038,S,01131.000,W,1,0,0.9,545.4,M,46.9,M,,*47");
    assert_false(result);

}
TestSuite *geo_gps_test_suite()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, test_ExtractCoordinate);
    add_test(suite, test_ParseGPSString);
    add_test(suite, test_IsValidGGA);

    return suite;
}

