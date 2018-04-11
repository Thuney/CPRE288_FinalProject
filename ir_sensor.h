/*
 * Sensor.h
 *
 *  Created on: Mar 6, 2018
 *      Author: jcnovak4
 */

#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

#include "Timer.h"
#include "lcd.h"

void Sensor_init();

unsigned Sensor_reader(void);

#endif /* IR_SENSOR_H_ */
