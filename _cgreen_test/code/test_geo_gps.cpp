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



Ensure(test_GetLongitude)
{
    GeoGPS geo_gps;
    geo_gps.SetLongitude(10);
    assert_equal(geo_gps.GetLongitude(), 10);
}

Ensure(test_GetLatitude)
{
    GeoGPS geo_gps;
    geo_gps.SetLatitude(20);
    assert_equal(geo_gps.GetLatitude(), 20);
}

TestSuite *geo_gps_test_suite()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, test_GetLongitude);
    add_test(suite, test_GetLatitude);

    return suite;
}

