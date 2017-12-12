/*
 * 	This file contains motor controller functions for the Traxxas RC car.
 *  Control is provided for the motor driver and servo motor (left/right).
 *
 * TBD: Go forward/reverse by distance
 *      Speed feedback test with speed_attained function
 */

#include "motor.hpp"

#define speed_margin 0.20

#define max_span_angle 30
#define duty_factor_angl (5.0/30)
#define Traxxas_Max_Speed 33.528 //mps
#define duty_factor_spd (5.0/Traxxas_Max_Speed) //required pwm for 1ms => 15.0 +/- (1ms * duty_factor_spd)
#define neutral_pwm 15.0

float max_pwm = 5.0;
int count = 0;

float speed_step = 0.001;
Motor::Motor()
{
    use_prev_speed = false;
    system_started = 1;
    static PWM motor(PWM::pwm2, 100);
    motor.set(15.0);
    MOTOR = &motor;
    static PWM servo(PWM::pwm3, 100);
    servo.set(15.0);
    SERVO = &servo;
    real_speed_dir = FWD;
    curr_can_speed = 0; //current received speed from can message
    curr_can_angle = 0; //current received angle from can message
    prev_can_speed = 0; //last received speed from can message
    prev_can_angle = 0; //last received speed from can message
    prev_speed_val = 0; //last used speed to make curr_mps_speed close to can_speed
    curr_mps_speed = 0; //current real speed
    prev_rps_cnt = 0; //previously read pedometer count
    curr_rps_cnt = 0; //current pedometer count coming from interrupt
    cur_clk = 0;
    total_count = 0;
    old_count = 0;
    simple_count = 0;
    kp = 0;
    ki = 0;
    kd = 0;
    UPDATE_calculated_latitude = 0;
    UPDATE_calculated_longitude = 0;
    COMPASS_bearing_angle = 0;
    GEO_distance_to_checkpoint = 0;
    left_sensor = 0;
    middle_sensor = 0;
    right_sensor = 0;
    back_sensor = 0;
}

bool Motor::init()
{
    MOTOR->set(neutral_pwm);
    SERVO->set(neutral_pwm);
    real_speed_dir = FWD;
    curr_can_speed = 0; //current received speed from can message
    curr_can_angle = 0; //current received angle from can message
    prev_can_speed = 0; //last received speed from can message
    prev_can_angle = 0; //last received speed from can message
    prev_speed_val = 0; //last used speed to make curr_mps_speed close to can_speed
    curr_mps_speed = 0; //current real speed
    prev_rps_cnt = 0; //previously read pedometer count
    curr_rps_cnt = 0; //current pedometer count coming from interrupt
    kp = 0.01;
    ki = 0.00008;
    kd = 0.006;
    integral_total = 0;
    error_prior = 0;
    UPDATE_calculated_latitude = 0;
    UPDATE_calculated_longitude = 0;
    COMPASS_bearing_angle = 0;
    GEO_distance_to_checkpoint = 0;
    left_sensor = 0;
    middle_sensor = 0;
    right_sensor = 0;
    back_sensor = 0;
    return true;
}

//Bring the car to a use_prev_speed at neutral steering position, reset all Motor class values
void Motor::stop_car()
{
    MOTOR->set(neutral_pwm);
    SERVO->set(neutral_pwm);
    //real_speed_dir = FWD;
    prev_can_speed = 0; //last received speed from can message
    prev_can_angle = 0; //last received speed from can message
    prev_speed_val = 0; //last used speed to make curr_mps_speed close to can_speed
    prev_rps_cnt = 0; //previously read pedometer count
    //curr_rps_cnt = 0; //current pedometer count coming from interrupt

}

void Motor::motor_periodic(uint32_t &count)
{

    this->get_can_vals(count);
    this->set_speed(count);
    this->set_angle();

    //This is the minimum time before we should expect a 0
    //The motor driver should be spinning at least ~.5mps
    //.5mps expects a motor tick every .773 seconds at worst case
    if ((count % 78 == 0)) {
        if (old_count == total_count) {
            curr_mps_speed = 0;
        }
        else {
            old_count = total_count;
        }
    }

}

void Motor::terminal_update(char a, float var)
{
    switch (a) {
        case '1':
            if (var >= 10.0) curr_can_speed = 10.0;
            else if (var <= -10.0) curr_can_speed = -10.0;
            else
                curr_can_speed = var;
            break;
        case '2':
            if (var >= 30.0) curr_can_angle = 30.0;
            else if (var <= -30.0) curr_can_angle = -30.0;
            else
                curr_can_angle = var;
            break;
        default:
            break;
    }
}

//Divide into 2 functions, scanf can't handle 3 floats
//Without crashing processor
void Motor::pid_update(float kp, float ki)
{
    this->kp = kp;
    this->ki = ki;
}

void Motor::pid_update_kd(float kd)
{
    this->kd = kd;
}

void Motor::get_can_vals(uint32_t count) //to update curr_can_speed, curr_can_angle, prev_can_speed, prev_can_angle
{
    can_msg_t can_msg;
    MOTOR_UPDATE_t motor_can_msg;
    MASTER_CONTROL_t master_can_msg;
    GEO_DATA_t geo_can_msg;
    UPDATE_CURRENT_LOCATION_t loc_can_msg;
    SENSOR_DATA_t sensor_can_msg;
    UPDATE_COMPASS_BEARING_t compass_bear_ang;

    while (CAN_rx(can1, &can_msg, 0)) {
        // Form the message header from the metadata of the arriving message
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;

        /*if(can_msg_hdr.mid == MASTER_CONTROL_HDR.mid)
         {

         // Attempt to decode the message (brute force, but should use switch/case with MID)
         if (dbc_decode_MASTER_CONTROL(&master_can_msg, can_msg.data.bytes, &can_msg_hdr))
         {
         if (master_can_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_START)
         {
         Motor::getInstance().system_started = 1;
         }
         else if (master_can_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_RESET)
         {
         Motor::getInstance().stop_car();
         Motor::getInstance().system_started = 0;
         }
         LE.on(2);
         }

         }
         else*/
        if (can_msg_hdr.mid == MOTOR_UPDATE_HDR.mid) {
            // Attempt to decode the message (brute force, but should use switch/case with MID)
            if (dbc_decode_MOTOR_UPDATE(&motor_can_msg, can_msg.data.bytes, &can_msg_hdr)) {
                curr_can_speed = ((float) motor_can_msg.MOTOR_speed);
                curr_can_angle = ((float) motor_can_msg.MOTOR_turn_angle);
                LE.on(2);
                LD.setNumber((int)curr_can_speed);
                printf("Curr_can_speed = %f mps\n", Motor::getInstance().curr_can_speed);
                //break;
            }
            else {
                if (count % 10 == 0) curr_can_angle = 0;
            }
        }
        else if (can_msg_hdr.mid == GEO_DATA_HDR.mid) {
            // Attempt to decode the message (brute force, but should use switch/case with MID)
            if (dbc_decode_GEO_DATA(&geo_can_msg, can_msg.data.bytes, &can_msg_hdr)) {

                GEO_distance_to_checkpoint = geo_can_msg.GEO_distance_to_checkpoint;
                //break;
            }
        }
        else if (can_msg_hdr.mid == UPDATE_COMPASS_BEARING_HDR.mid) {
            // Attempt to decode the message (brute force, but should use switch/case with MID)
            if (dbc_decode_UPDATE_COMPASS_BEARING(&compass_bear_ang, can_msg.data.bytes, &can_msg_hdr)) {

                COMPASS_bearing_angle = compass_bear_ang.COMPASS_bearing_angle;
                //break;
            }
        }
        else if (can_msg_hdr.mid == UPDATE_CURRENT_LOCATION_HDR.mid) {
            // Attempt to decode the message (brute force, but should use switch/case with MID)
            if (dbc_decode_UPDATE_CURRENT_LOCATION(&loc_can_msg, can_msg.data.bytes, &can_msg_hdr)) {
                /*float UPDATE_calculated_latitude;         ///< B27:0  Min: -90 Max: 90   Destination: BRIDGE,MOTOR
                 float UPDATE_calculated_longitude;        ///< B56:28  Min: -180 Max: 180   Destination: BRIDGE,MOTOR
                 */
                UPDATE_calculated_latitude = loc_can_msg.UPDATE_calculated_latitude;
                UPDATE_calculated_longitude = loc_can_msg.UPDATE_calculated_longitude;

                //break;
            }
        }
        else if (can_msg_hdr.mid == SENSOR_DATA_HDR.mid) {
            // Attempt to decode the message (brute force, but should use switch/case with MID)
            if (dbc_decode_SENSOR_DATA(&sensor_can_msg, can_msg.data.bytes, &can_msg_hdr)) {
                left_sensor = sensor_can_msg.SENSOR_left_sensor;
                right_sensor = sensor_can_msg.SENSOR_right_sensor;
                middle_sensor = sensor_can_msg.SENSOR_middle_sensor;
                back_sensor = sensor_can_msg.SENSOR_back_sensor;
                //break;
            }
        }
        //dbc_decode_UPDATE_CURRENT_LOCATION
    }

    /*if(dbc_handle_mia_MOTOR_UPDATE(&motor_can_msg, 20000))
     {
     //mia occurred, reset now;
     Motor::getInstance().stop_car();
     }*/

}

int Motor::transition_reverse()
{
    if (simple_count > 20) {
        simple_count = 0;
        return 1;
    }
    else {
        simple_count++;
    }
    return 0;
}

//Set motor to given speed
//prev_speed_val is speed to set
void Motor::set_speed(int count) //convert speed to pwm, and handle (curr_mps_speed != 0 && (prev_can_speed > 0 && curr_can_speed < 0))
{

    if (fabsf(prev_speed_val * duty_factor_spd) <= max_pwm && fabsf(prev_speed_val * duty_factor_spd) >= 0) {
        //go more forward/reverse
        MOTOR->set(neutral_pwm + (prev_speed_val * duty_factor_spd));
    }
    /*else if(prev_speed_val*duty_factor_spd < 0)
     {
     //runnning at max speed possible in reverse
     MOTOR->set(neutral_pwm - (max_pwm));
     }
     else if(prev_speed_val*duty_factor_spd > 0)
     {
     //runnning at max speed possible in forward
     MOTOR->set(neutral_pwm + (max_pwm));
     }*/

    prev_can_speed = curr_can_speed;
    //Check if speed is accurate
    this->check_real_speed_update(count);
}

void Motor::set_angle() //convert angle to pwm
{
    if (prev_can_angle != curr_can_angle) {
        prev_can_angle = curr_can_angle;
        //max angle is 30
        if (fabsf(curr_can_angle * duty_factor_spd) <= 5.0) SERVO->set(neutral_pwm + (curr_can_angle * (5.0 / 30.0)));
        else
            SERVO->set(neutral_pwm + (5.0));
    }
}

//Control loop to change motor speed based on speed read from sensor
void Motor::check_real_speed_update(int count) //to check if curr_mps_speed == curr_can_speed, if not increase prev_speed_val
{
    if (curr_can_speed < 0) {
        if (curr_mps_speed == 0) real_speed_dir = REV;
    }
    else {
        if (curr_mps_speed == 0) real_speed_dir = FWD;
    }

    //curr_mps_speed = real_speed_dir * (3.14*0.04)*(diff_cnt)/0.1;
    LD.setNumber(fabsf(curr_mps_speed));

    if(curr_can_speed == 0.0)
    {
		//to hard break, first set to MAX opposite speed (ex: full REVERSE if going forward)
//		if (this->prev_can_speed > 0)
//		{
//			//max reverse
//			MOTOR->set(10);
//			vTaskDelayMs(4);
//		} else if (this->prev_can_speed < 0) {
//			//max forward
//			MOTOR->set(20);
//			vTaskDelayMs(2);
//		}
		//Delay shorter than length of shortest periodic (10ms)
		//udelay(10);
        this->stop_car();
        return;
    }

    //jump start the motor
    /*if(curr_mps_speed == 0 && curr_can_speed < 0)
     prev_speed_val-=0.008;
     else if(curr_mps_speed == 0 && curr_can_speed > 0)
     prev_speed_val+=0.008;*/

    //PID loop handles all feedback of the motor speed
    this->motor_pid();

}

//PID uses factors to tune current speed
//Error function is E(T) = DESIRED - READ_SPEED
//Integral is sum of integral plus the current error * time (delta_t)
//Derivative is the error - previous error / time (delta_t)
void Motor::motor_pid()
{
    float kp = this->kp, ki = this->ki, kd = this->kd;
    float new_set_speed;
    //Proportional, integral and derivative error
    float error, err_der;
    float read_speed = curr_mps_speed * real_speed_dir;
    //each iteration is 10ms, .01 seconds
    float iter_time = .01;

    //ERROR is desired speed - read speed
    error = curr_can_speed - read_speed;

    //Derivative error
    err_der = (error - error_prior) / iter_time;

    //Integral error
    integral_total = integral_total + (error * iter_time);

    //Update terms for integral and derivative error
    error_prior = error;

    //New set speed is the output of the PID loop
    new_set_speed = kp * error + kd * err_der + ki * integral_total;
    prev_speed_val += new_set_speed;
    if (prev_speed_val >= 33) {
        prev_speed_val = 33;
    }
    else if (prev_speed_val <= -33) {
        prev_speed_val = 0;
    }
    //In order to reverse, sometimes we need to reset the speed to 0
    //Reverse speeds only work if you step from 0
    if(fabsf(prev_speed_val) > 10.0 && curr_mps_speed == 0 && curr_can_speed != 0)
    {
        prev_speed_val = 0;
    }
}

bool Motor::speed_attained()
{
    if (fabsf(curr_can_speed - (real_speed_dir * curr_mps_speed)) >= 0 && fabsf(curr_can_speed - (real_speed_dir * curr_mps_speed)) <= speed_margin) return true;
    else
        return false;
}

float Motor::get_curr_rps_speed()
{
    return curr_mps_speed;
}

/////////HELPER functions////////

void rps_cnt_hdlr() //to update prev_rps_cnt and curr_rps_cnt;
{
    uint64_t t_delt = 0;
    Motor *M = &Motor::getInstance();

    if (M->curr_rps_cnt == 2) {

        t_delt = sys_get_uptime_us() - M->cur_clk;
        M->curr_rps_cnt = 0;
        //Get time delta
        //using 0.045m as diameter of the gear
        //This is the mps of the SPUR GEAR
        M->curr_mps_speed = ((3.1415 * 0.045 * 2.0) * (1e+6)) / (t_delt);

        //MPS of the wheel is MPS SPUR * 2.72
        //This is the REAL speed of the car based on the wheels spinning
        M->curr_mps_speed *= 2.72;

        M->cur_clk = sys_get_uptime_us();
    }
    else {
        //start time
        M->curr_rps_cnt++;
    }
    M->total_count++;
}

float battery_voltage()
{
	//12 bit ADC, max value is 4096
	float maxADC = 4096;
	//Voltage divider ratio from battery to ADC (to get battery voltage < 3.3)
	float volt_div = 460/1460;
	//percentage the adc reads compared to the input voltage of 3.3
    float adc5 = adc0_get_reading(5) / maxADC;
    //battery voltage is 8.4*adc percentage
    float battery_voltage = adc5 / volt_div;
    printf("battery_voltage: %f\n", battery_voltage);
    return battery_voltage;
}
void update_TFT()
{
    Motor *M = &Motor::getInstance();

    send_Battery_data((uint8_t )battery_voltage());
    //send_Battery_data(90);
    vTaskDelayMs(10);
    send_Compass_data(M->COMPASS_bearing_angle);
    vTaskDelayMs(30);
    send_Sensor_data(M->middle_sensor, M->left_sensor, M->right_sensor);
    //send_Sensor_data(10, 10, 10);
    vTaskDelayMs(10);
    send_Motor_data(M->curr_mps_speed);
    vTaskDelayMs(30);
    send_GPS_data(M->UPDATE_calculated_latitude, M->UPDATE_calculated_longitude, M->GEO_distance_to_checkpoint);

}

void send_heartbeat()
{
    MOTOR_HB_t can_msg;
    can_msg.MOTOR_heartbeat = 0x1;
    dbc_encode_and_send_MOTOR_HB(&can_msg);
    LE.on(3);
}

void send_feedback()
{
    MOTOR_FEEDBACK_t can_msg;
    can_msg.MOTOR_actual_speed = Motor::getInstance().get_curr_rps_speed();
    can_msg.sensed_battery_voltage = 0;
    dbc_encode_and_send_MOTOR_FEEDBACK(&can_msg);
    LE.on(4);
}

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);
    return CAN_tx(can1, &can_msg, 0);
}

//Scan for start/stop command from master node
bool recv_system_start()
{

    MASTER_CONTROL_t master_can_msg;
    can_msg_t can_msg;
    bool rc;
    while (CAN_rx(can1, &can_msg, 0)) {
        // Form the message header from the metadata of the arriving message
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;

        // Attempt to decode the message (brute force, but should use switch/case with MID)
        if (can_msg_hdr.mid == MASTER_CONTROL_HDR.mid) {
            //MASTER_CONTROL_t master_can_msg;
            // Attempt to decode the message (brute force, but should use switch/case with MID)
            if (dbc_decode_MASTER_CONTROL(&master_can_msg, can_msg.data.bytes, &can_msg_hdr)) {
                if (master_can_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_START) {
                    Motor::getInstance().system_started = 1;
                }
                else if (master_can_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_RESET) {
                    Motor::getInstance().stop_car();
                    Motor::getInstance().system_started = 0;
                }
                LE.on(2);
                return true;
            }

        }
    }
    /*if(dbc_handle_mia_MASTER_CONTROL(&master_can_msg, 100))
     {
     //mia occurred, reset now;
     Motor::getInstance().stop_car();
     }
     */
    return false;
}

//UNIT TESTING
Motor& return_current_instance()
{
    return Motor::getInstance();
}

float get_speed_pwm(float speed)
{
    return (neutral_pwm + (speed * duty_factor_spd));
}

float get_angle_pwm(int angle)
{
    return (neutral_pwm + (angle * duty_factor_angl));
}
