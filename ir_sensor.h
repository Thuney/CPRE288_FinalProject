/*
 * Sensor.h
 *
 *  Created on: Mar 6, 2018
 *      Author: jcnovak4
 */

#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>

/*
 * Initialize IR sensor and relevant components
 * Uses:
 *  GPIO Port B Pin 4 as ADC
 *  ADC0 for sample conversions
 */
void ir_init(void);

/*
 * Read a distance value from the IR sensor
 */
unsigned read_ir(void);

#endif /* IR_SENSOR_H_ */
