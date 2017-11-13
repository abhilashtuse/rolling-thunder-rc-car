/*
 * 	This file contains motor controller functions for the Traxxas RC car.
 *  Control is provided for the motor driver and servo motor (left/right).
 *
 * TBD: Go forward/reverse by distance
 *      Speed feedback test with speed_attained function
 */

#include "motor.hpp"

#define speed_margin 0.20
//#define speed_step 0.25 //speed increment step for check_speed

#define DIA_m 0.2 //0.05588 //in meters = 2.2 inches

#define DIA_m_mul_PI (DIA_m * 3.14159265359)
#define CIRCUMFERENCE 0.175552197 * (DIA_m/0.05588) //DIA_m_mul_PI metres

#define Traxxas_Max_Speed 33.528 //mps
#define duty_factor (5.0/Traxxas_Max_Speed) //required pwm for 1ms => 15.0 +/- (1ms * duty_factor)
#define neutral_pwm 15.0

float speed_step = 0.001;
Motor::Motor()
{
    use_prev_speed = false;
    system_started = 0;
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
    integral_total=0;
    error_prior=0;
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

    this->get_can_vals();
    this->set_speed(count);
    this->set_angle();

    //This is the minimum time before we should expect a 0
    //The motor driver should be spinning at least ~.5mps
    //.5mps expects a motor tick every .773 seconds at worst case
	if ((count%78==0))
	{
		if (old_count == total_count)
		{
			//printf("found 0 speed\n");
			curr_mps_speed = 0;

		}
		else {
		    old_count = total_count;
		}
	}

}

void Motor::terminal_update(char a,float var)
{
    switch(a)
    {
        case '1':
            if(var >= 10.0)
                curr_can_speed = 10.0;
            else if(var <= -10.0)
                curr_can_speed = -10.0;
            else
                curr_can_speed = var;
            break;
        case '2':
            if(var >= 30.0)
                curr_can_angle = 30.0;
            else if(var <= -30.0)
                curr_can_angle = -30.0;
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

void Motor::get_can_vals() //to update curr_can_speed, curr_can_angle, prev_can_speed, prev_can_angle
{
    can_msg_t can_msg;
    while (CAN_rx(can1, &can_msg, 0))
    {
        // Form the message header from the metadata of the arriving message
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;

        if(can_msg_hdr.mid == MASTER_CONTROL_HDR.mid)
        {
            MASTER_CONTROL_t master_can_msg;
            // Attempt to decode the message (brute force, but should use switch/case with MID)
            if (dbc_decode_MASTER_CONTROL(&master_can_msg, can_msg.data.bytes, &can_msg_hdr))
            {
                if (master_can_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_START)
                {
                    ////////printf("recv start\n");
                    Motor::getInstance().system_started = 1;
                    //Motor::getInstance().motor_periodic();
                }
                else if (master_can_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_RESET)
                {
                    Motor::getInstance().stop_car();
                    ////////printf("recv start\n");
                    Motor::getInstance().system_started = 0;
                }
                LE.on(2);
            }

        }
        else if(can_msg_hdr.mid == MOTOR_UPDATE_HDR.mid)
        {
            MOTOR_UPDATE_t motor_can_msg;
            // Attempt to decode the message (brute force, but should use switch/case with MID)
            if (dbc_decode_MOTOR_UPDATE(&motor_can_msg, can_msg.data.bytes, &can_msg_hdr))
            {
                 curr_can_speed = ((float)motor_can_msg.MOTOR_speed);
                 curr_can_angle = ((float)motor_can_msg.MOTOR_turn_angle);
                 LE.on(2);
                 LD.setNumber((int)curr_can_speed);
            }
        }
    }

}

int Motor::transition_reverse()
{
	//printf("transition reverse %d, cur mps is %f\n", simple_count, curr_mps_speed);
	if (simple_count > 20)
	{
		simple_count = 0;
		return 1;
	} else {
		simple_count++;
	}
	return 0;
}

//Set motor to given speed
//prev_speed_val is speed to set
void Motor::set_speed(int count) //convert speed to pwm, and handle (curr_mps_speed != 0 && (prev_can_speed > 0 && curr_can_speed < 0))
{

	//To reverse, car needs to stop first
   /* if((curr_mps_speed * real_speed_dir) > 0 &&  curr_can_speed < 0)
    {
    		//Stop the car
    		stop_car();

    		//Enter to_negative state
    		transition_reverse();
    		return;

//                if(curr_mps_speed != 0)
//                {
//                    use_prev_speed = 1;
//                    prev_can_speed = 0;
//                    this->check_real_speed_update();
//                    prev_can_speed = curr_can_speed;
//                    return;
//                }
     }
*/
//    if(prev_can_speed == curr_can_speed)
//    {
//        this->check_real_speed_update();
//    }
//    else
        //printf("curr_can_speed = %f\n", curr_can_speed);
        //if(curr_can_speed < 0 && )
        //prev_speed_val = curr_can_speed;
        if(fabsf(prev_speed_val*duty_factor) <= 5.0)
        {
            //printf("\n pwm = %f",neutral_pwm + (prev_speed_val*duty_factor));
            //go more forward/reverse
            MOTOR->set(neutral_pwm + (prev_speed_val*duty_factor));
        }
        else if(prev_speed_val*duty_factor < 0)
        {
            //////printf("\n pwm = %f",neutral_pwm + (prev_speed_val*duty_factor));
            //runnning at max speed possible in reverse
            MOTOR->set(neutral_pwm - (5.0));
        }
        else if(prev_speed_val*duty_factor > 0)
        {
            //////printf("\n pwm = %f",neutral_pwm + (prev_speed_val*duty_factor));
            //runnning at max speed possible in forward
            MOTOR->set(neutral_pwm + (5.0));
        }

        prev_can_speed = curr_can_speed;
        //Check if speed is accurate
        this->check_real_speed_update(count);
}

void Motor::set_angle() //convert angle to pwm
{
    if(prev_can_angle != curr_can_angle)
    {
        //////printf("prev_can_angle = %f -> curr_can_angle = %f", prev_can_angle, curr_can_angle);
        prev_can_angle = curr_can_angle;
    //max angle is 30
    if(fabsf(curr_can_angle*duty_factor) <= 5.0)
        SERVO->set(neutral_pwm + (curr_can_angle * (5.0/30.0)));
    else
        SERVO->set(neutral_pwm + (5.0));
    }
}

//Control loop to change motor speed based on speed read from sensor
void Motor::check_real_speed_update(int count) //to check if curr_mps_speed == curr_can_speed, if not increase prev_speed_val
{
	//printf("check speed\n");
    if(curr_mps_speed != 0.0)
    {
        //printf("\ncurr_mps_speed = %f\n", curr_mps_speed);
    }
//    else
//    {
//        if(use_prev_speed == true)
//        {
//            prev_speed_val = prev_can_speed;
//            use_prev_speed = false;
//        }
//        else
//        {
//            prev_speed_val = curr_can_speed;
//        }
//    }

    //curr_mps_speed = CIRCUMFERENCE*(diff_cnt)/0.1;
    if(curr_can_speed < 0)
    {
        if(curr_mps_speed ==0)
           real_speed_dir = REV;
    }
    else
    {   if(curr_mps_speed ==0)
            real_speed_dir = FWD;
    }


    //curr_mps_speed = real_speed_dir * (3.14*0.04)*(diff_cnt)/0.1;
    LD.setNumber(fabsf(curr_mps_speed));
    //printf("\ndir = %d, cur speed = %f\n", real_speed_dir, (curr_mps_speed*real_speed_dir));
    if(curr_can_speed == 0.0)
	{
		this->stop_car();
		return;
	}

    //jump start the motor
//    if(curr_mps_speed == 0 && curr_can_speed < 0)
//		prev_speed_val-=0.008;
//	else if(curr_mps_speed == 0 && curr_can_speed > 0)
//		prev_speed_val+=0.008;


    //PID loop handles all feedback of the motor speed
    this->motor_pid();

    //printf("check speed\n");
    //Feedback loop if our speed is outside acceptable margin
//    if(this->speed_attained() == false)
//    {
//        float delta = (real_speed_dir * curr_mps_speed) - (curr_can_speed);
//        //printf("\ndelta = %f\n",delta);
//
//        if(!(fabsf(delta) > 0 && fabsf(delta) < speed_margin))
//          {
//                //for quicker speed up/down
//                //if(fabsf(delta)>=speed_step*4)
//                //    prev_speed_val+=speed_step*(fabsf(delta)/speed_step);
//                //else
//            /*if(curr_mps_speed == 0 && curr_can_speed < 0)
//                prev_speed_val-=0.3;
//            else if(curr_mps_speed == 0 && curr_can_speed > 0)
//                prev_speed_val+=0.3;*/
//
////                    if(curr_can_speed < (curr_mps_speed*real_speed_dir))
////                        prev_speed_val-=speed_step;
////                    else if(curr_can_speed > (curr_mps_speed*real_speed_dir))
////                    {
////                    	prev_speed_val+=speed_step;
////          	  	  	}
////
////                    if(fabsf(prev_speed_val) > 10.0 && curr_mps_speed == 0 && curr_can_speed != 0)
////                    {
////                    		 prev_speed_val = 0;//(curr_can_speed>0?1:-1)*0.3;
////					 	 speed_step = 0.5;
////                    }
////					else{
////						speed_step = 0.025;
////					}
//                    //printf("prev_speed_val = %f\n",prev_speed_val);
//
//            } //delta between 0 and margin
//    } //speed attained
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
	err_der = (error - error_prior)/iter_time;

	//Integral error
	integral_total = integral_total + (error * iter_time);

	//Update terms for integral and derivative error
	error_prior = error;

	//New set speed is the output of the PID loop
	new_set_speed = kp*error+ kd*err_der + ki*integral_total;
	prev_speed_val += new_set_speed;
	if (prev_speed_val >= 33)
	{
		prev_speed_val = 33;
	} else if (prev_speed_val <= -33) {
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
    if(fabsf(curr_can_speed - (real_speed_dir*curr_mps_speed)) >=0 && fabsf(curr_can_speed - (real_speed_dir*curr_mps_speed)) <= speed_margin)
        return true;
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
    //printf("got interrupt\n");

    if (M->curr_rps_cnt == 2)
	{

		t_delt = sys_get_uptime_us() - M->cur_clk;
		M->curr_rps_cnt = 0;
//		//Get time delta
//		printf("t_delt: %d %d\n", (uint32_t)t_delt >> 32, (uint32_t)t_delt & 0xFFFFFFFF);
//		printf("cur_clk: %d %d\n", (uint32_t)M->cur_clk >> 32, (uint32_t)M->cur_clk & 0xFFFFFFFF);

		//using 0.045m as diameter of the gear
		//This is the mps of the SPUR GEAR
		M->curr_mps_speed = ((3.1415*0.045*2.0)*(1e+6))/(t_delt);

		//MPS of the wheel is MPS SPUR * 2.72
		//This is the REAL speed of the car based on the wheels spinning
		M->curr_mps_speed *= 2.72;

		M->cur_clk = sys_get_uptime_us();
	} else {
		//start time
		M->curr_rps_cnt++;
	}
    M->total_count++;
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
    can_msg.msg_id                = mid;
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
    while (CAN_rx(can1, &can_msg, 0))
    {
        // Form the message header from the metadata of the arriving message
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;

        // Attempt to decode the message (brute force, but should use switch/case with MID)
        if(can_msg_hdr.mid == MASTER_CONTROL_HDR.mid)
                {
                    MASTER_CONTROL_t master_can_msg;
                    // Attempt to decode the message (brute force, but should use switch/case with MID)
                    if (dbc_decode_MASTER_CONTROL(&master_can_msg, can_msg.data.bytes, &can_msg_hdr))
                    {
                        if (master_can_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_START)
                        {
                            ////////printf("recv start\n");
                            Motor::getInstance().system_started = 1;
                            //Motor::getInstance().motor_periodic();
                        }
                        else if (master_can_msg.MASTER_CONTROL_cmd == DRIVER_HEARTBEAT_cmd_RESET)
                        {
                            Motor::getInstance().stop_car();
                            ////////printf("recv start\n");
                            Motor::getInstance().system_started = 0;
                        }
                        LE.on(2);
                        return true;
                    }

                }
    }
    // Service the MIA counters
    // successful decoding resets the MIA counter, otherwise it will increment to
    // its MIA value and upon the MIA trigger, it will get replaced by your MIA struct
    //rc = dbc_handle_mia_LAB_TEST(&master_can_msg, 1000);  // 1000ms due to 1Hz
    //system_started = 1;
    return false;
}
