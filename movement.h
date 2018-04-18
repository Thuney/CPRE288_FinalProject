/*
 * movement.h
 *
 *  Created on: Jan 24, 2018
 *      Author: bryank
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <Timer.h>
#include <lcd.h>
#include "open_interface.h"

int move_forward(oi_t *sensor, int distance);

int move_backward(oi_t *sensor_data, int distance);

void turn_clockwise(oi_t *sensor, int degrees);

void turn_counter_clockwise(oi_t *sensor_data, int degrees);

void center_bot_forward(oi_t *sensor_data);

void center_bot_backward(oi_t *sensor_data);

#endif /* MOVEMENT_H_ */
