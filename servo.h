/**
*   @file servo.h
*   @brief This file contains the methods necessary to operate the servo.
*/

#ifndef SERVO_H_
#define SERVO_H_

#include "timer.h"
#include <inc/tm4c123gh6pm.h>

//period in seconds * clock (16MHz)
//volatile unsigned top = 320000; // pulse period in cycles (20 ms)
volatile signed pulse_width;

/**
*   This method initiates servo operation.
*/
void servo_init();

/**
*   This method moves the servo.
*   @param degree   The degree to move the servo to
*/
void move_servo(float degree);

#endif /* SERVO_H_ */
