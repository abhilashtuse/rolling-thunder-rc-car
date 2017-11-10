#include <cgreen/cgreen.h>
// #include <cgreen/mocks.h>
#include <stdio.h>
#include <stdint.h>
#include "testfuncs.h"
#include <stdlib.h>
int first_message = 0;
Describe(period_ten);
BeforeEach(period_ten){first_message = 1;}
AfterEach(period_ten){}

Ensure(period_ten, test_one){
    char *temp = strdup("c10091134.321453-121.987654");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321453));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987654));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_two){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_three){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_four){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_five){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_six){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_seven){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_eight){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_nine){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(0));
    free(temp);
}

Ensure(period_ten, test_ten){
    char *temp = strdup("c091134.321454-121.987659");
    BRIDGE_START_STOP_t ans = period_ten_test(temp);
    assert_that_double(ans.BRIDGE_CHECKPOINT_latitude, is_equal_to_double(34.321454));
    assert_that_double(ans.BRIDGE_CHECKPOINT_longitude, is_equal_to_double(-121.987659));
    assert_that(ans.BRIDGE_FINAL_COORDINATE, is_equal_to(1));
    free(temp);
}

TestSuite *p10_test(){
    TestSuite *suite_two = create_test_suite();
    add_test_with_context(suite_two, period_ten, test_one);
    add_test_with_context(suite_two, period_ten, test_two);
    add_test_with_context(suite_two, period_ten, test_three);
    add_test_with_context(suite_two, period_ten, test_four);
    add_test_with_context(suite_two, period_ten, test_five);
    add_test_with_context(suite_two, period_ten, test_six);
    add_test_with_context(suite_two, period_ten, test_seven);
    add_test_with_context(suite_two, period_ten, test_eight);
    add_test_with_context(suite_two, period_ten, test_nine);
    add_test_with_context(suite_two, period_ten, test_ten);
    return suite_two;
}


BRIDGE_START_STOP_t    period_ten_test(char *buffer)
{
    // Uart3 &u3 = Uart3::getInstance();
    // int first_message = 0;
    char temp[2] = {0};
    char *ptr = buffer;
    static uint8_t n_checkpoints = 0;
    uint8_t nb_lat = 0;
    uint8_t nb_long = 0;
    int success = 0;
    BRIDGE_START_STOP_t checkpoint = {0};

    // printf("FirstMessage: %d\n", first_message);
    if (buffer[0] == 'a'){
        if (buffer[1] == '0')
        {
            // start_car(0,0,0);
            printf("Car stopped.\n");
            // LE.toggle(2);//stop car command sent
        }
        else
        {
            // start_car(1, 1, 1); // manual car start
            printf("Car start\n");
            // LE.toggle(4);//start car command sent
        }
    }else if (buffer[0] == 'c'){ //sending checkpoints
        ptr = ptr + 1;
        if (!first_message){
            n_checkpoints = get_two(ptr);
            ptr = ptr + 2;
            first_message = 1;
        }
        nb_lat = get_two(ptr);
        ptr = ptr + 2;
        nb_long = get_two(ptr);
        ptr = ptr + 2;
        /*
        * Checkpoints
        */
        
        checkpoint.BRIDGE_START_STOP_cmd = 1;
        checkpoint.BRIDGE_CHECKPOINT_latitude = atof(ptr);
        // printf("%f\n", checkpoint.BRIDGE_CHECKPOINT_latitude);
        ptr = ptr + nb_lat;
        checkpoint.BRIDGE_CHECKPOINT_longitude = atof(ptr);
        ptr = ptr + nb_long;
        n_checkpoints--;
        checkpoint.BRIDGE_FINAL_COORDINATE = (n_checkpoints <= 0) ? 1 : 0;
        // dbc_encode_and_send_BRIDGE_START_STOP(&checkpoint);
        // printf("n_checkpoints: %d\nn_lat: %d\nn_long: %d\nLat: %f\nLong: %f\nFinal Coordinate: %d\n",n_checkpoints, nb_lat, nb_long,
        // checkpoint.BRIDGE_CHECKPOINT_latitude,checkpoint.BRIDGE_CHECKPOINT_longitude,checkpoint.BRIDGE_FINAL_COORDINATE);
    }
    return (checkpoint);
}