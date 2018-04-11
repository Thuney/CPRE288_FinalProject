/*
 * Sensor.h
 *
 *  Created on: Mar 6, 2018
 *      Author: jcnovak4
 */

#ifndef SENSOR_H_
#define SENSOR_H_
#include "Timer.h"
#include "lcd.h"
void Sensor_init();
unsigned Sensor_reader(void);
#endif /* SENSOR_H_ */
