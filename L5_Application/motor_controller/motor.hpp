/*
 * motor.hpp
 *
 *  Created on: Oct 7, 2017
 *      Author: jskow
 */
#include "lpc_pwm.hpp"
#include "singleton_template.hpp"  // Singleton Template
#include "_can_dbc/generated_can.h"
#include "can.h"
#include "string.h"
#include "io.hpp"
#include "lpc_sys.h"
#include "LPC17xx.h"
#include "sys_config.h"
#include "stdio.h"
#include "math.h"
#include <cmath>
#include "math.h"
#include "inttypes.h"
#include "stdint.h"

/*
LE(1); //on if system started
LE(2); //on if any can_msg is successfully decoded
LE(3); //on if sending Motor_heartbeat over can
LE(4); //on if sending Motor_feedback over can
*/

typedef enum {
    FWD = 1,
    REV = -1,
} Speed_dir;

class Motor : public SingletonTemplate<Motor>
{
    public:
        bool init();
        void get_can_vals(); //to update curr_can_speed, curr_can_angle, prev_can_speed, prev_can_angle
        void set_speed(int count); //convert speed to pwm, and handle (curr_mps_speed != 0 && (prev_can_speed > 0 && curr_can_speed < 0))
        void set_angle(); //convert angle to pwm
        void check_real_speed_update(int count); //to check if curr_mps_speed == curr_can_speed, if not increase prev_speed_val
        bool speed_attained();
        void stop_car();
        void motor_periodic(uint32_t &count); //to be called in periodic function, which alls appropriate functions
        int curr_rps_cnt; //current pedometer count coming from interrupt
        void terminal_update(char a,float an);
        bool system_started;
        float get_curr_rps_speed();
        void motor_pid();
        void pid_update(float kp, float ki);
        void pid_update_kd(float kd);

        int transition_reverse();

		friend void rps_cnt_hdlr(); //to update prev_rps_cnt and curr_rps_cnt;
    private:
        Motor();  ///< Private constructor of this Singleton class
        friend class SingletonTemplate<Motor>;  ///< Friend class used for Singleton Template
        Speed_dir real_speed_dir;
        bool use_prev_speed;
        PWM * MOTOR;
        PWM * SERVO;
        float curr_can_speed; //current received speed from can message
        float curr_can_angle; //current received angle from can message
        float prev_can_speed; //last received speed from can message
        float prev_can_angle; //last received speed from can message
        float prev_speed_val; //last used speed to make curr_mps_speed close to can_speed
        float curr_mps_speed; //current real speed
        int prev_rps_cnt; //previously read pedometer count
        uint64_t cur_clk; //Beginning time of RPM measurement
        int total_count; //total ticks from motor since start of program
        int old_count; //previous number of ticks to make sure motor doesn't go to 0
        int simple_count; //Used to transition motor from forward to reverse
        //PID coefficients
        float kp; //proportional
        float ki; //integral
        float kd; //derivative
        //PID values to store
        float integral_total=0;
        float error_prior=0;
};

//interrupt handler callback
void rps_cnt_hdlr(); //to update prev_rps_cnt and curr_rps_cnt;

//Relevant CAN helper functions for motor controller
bool recv_system_start(); //receive start for the first time
void send_heartbeat();
void send_feedback(); //send voltage and mps speed

//Unit testing
Motor& return_current_instance();
float get_speed_pwm(float speed);
int get_angle_pwm(float angle);

#ifndef L5_APPLICATION_MOTOR_CONTROLLER_MOTOR_HPP_
#define L5_APPLICATION_MOTOR_CONTROLLER_MOTOR_HPP_


#endif /* L5_APPLICATION_MOTOR_CONTROLLER_MOTOR_HPP_ */