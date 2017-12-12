/**
 * @file
 * @ingroup Utilities
 */
#ifndef MASTER_CONTROLLER_H
#define MASTER_CONTROLLER_H

#include "../_can_dbc/generated_can.h"

class motor
{
	public:
	void update_motor(SENSOR_DATA_t sensor_data, MOTOR_UPDATE_t *motor_update, float actual_speed, bool *avoid_obstacle, bool turn_towards);
};

#endif /* MASTER_CONTROLLER_H */
