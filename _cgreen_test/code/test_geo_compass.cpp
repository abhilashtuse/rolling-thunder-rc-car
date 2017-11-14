#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

using namespace cgreen;

// The file under test
#include "geo_compass.h"


Ensure(test_CalculateCompassBearingAngle)
{
    GeoCompass geo_compass;
    double bearing_angle;
    bearing_angle = geo_compass.CalculateBearingAngle(3, 133);
    assert_that_double(bearing_angle, is_equal_to_double(90.1));
    bearing_angle = geo_compass.CalculateBearingAngle(7, 208);
    assert_that_double(bearing_angle, is_equal_to_double(200));
    bearing_angle = geo_compass.CalculateBearingAngle(3, 57);
    assert_that_double(bearing_angle, is_equal_to_double(82.5));
}

TestSuite *geo_compass_test_suite()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, test_CalculateCompassBearingAngle);
    return suite;
}

