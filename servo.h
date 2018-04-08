/*
*
*   servo.h
*
*   Used to set up the RS232 connector and WIFI module
*   uses UART1 at 115200
*
*/

#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

#include <inc/tm4c123gh6pm.h>

#include "Timer.h"
#include "lcd.h"
#include "button.h"

double countToDeg(int count);
int degreesToCount(double deg);
void calibrate();
void servo_init();
void buttonSwitch(uint8_t button);
void move_servo(unsigned counter);


#endif
