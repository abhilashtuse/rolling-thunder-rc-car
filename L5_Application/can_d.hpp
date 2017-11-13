/*
 * can_dbc.hpp
 *
 *  Created on: Oct 20, 2017
 *      Author: Thrishna
 */

#ifndef CAN_D_HPP_
#define CAN_D_HPP_

void initialize_can();
void can_reset();
bool can_receive();
void can_transmit_message(int left, int middle, int right, int back);
void can_transmit_heartbeat();

#endif /* CAN_D_HPP_ */
