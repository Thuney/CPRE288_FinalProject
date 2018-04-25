/*
 * IR.h
 *
 *  Created on: Feb 28, 2018
 *      Author: charette
 */

#ifndef IR_H_
#define IR_H_
#include <inc/tm4c123gh6pm.h>
#include "timer.h"
void ir_init(void);

unsigned ir_read();

//double convert_bot17(int value);
float getDistance();

#endif /* IR_H_ */
