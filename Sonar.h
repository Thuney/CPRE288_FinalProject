/*
 * Sonar.h
 *
 *  Created on: Mar 7, 2018
 *      Author: charette
 */

#ifndef SONAR_H_
#define SONAR_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"
#include "driverlib/interrupt.h"


#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

#define PING_FACTOR 0.0010625f //Factor to get distance in cm

volatile enum {RISE, FALL, DONE} state;
volatile unsigned risingEdge;
volatile unsigned fallingEdge;
volatile unsigned pulseLength;
volatile unsigned overflow;

/**
*   This method captures the time from when the sensor sends a pulse and it returns.
*/
void TIMER3B_Handler(void);

/**
*   This method initiates the ping sensor.
*/
void sonar_init();

/**
*   This method operates the ping sensor.
*/
void send_pulse();

/**
*   This method collects the ping sensor data.
*/
float ping_getDistance();


#endif /* SONAR_H_ */
