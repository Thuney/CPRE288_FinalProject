/*
 * sonar.h
 *
 *  Created on: Apr 4, 2018
 *      Author: adthune
 */

#ifndef SONAR_H_
#define SONAR_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "Timer.h"

volatile unsigned rising_time;
volatile unsigned falling_time;
volatile enum{LOW, HIGH, DONE} state;
volatile uint32_t length; //Length of pulse in cycles

/*
 * Handler function for a captured event on Timer3B
 * 
 * Stores the times of sent and received pings in global variables
 * rising_time and falling_time, and changes the value of state according
 * to the status of the ping event
 */
void Timer3B_Handler();

/*
 * Initializes sonar settings
 *
 * Uses:
 *  GPIO Port B Pin 3 (as output for pulse and as Timer)
 *  Timer 3B (for capturing time events)
 */
void sonar_init();

/*
 * Emits a sonar pulse on GPIO Port B Pin 3 and sets the pin up for receiving the return signal
 */
void emit_sonar_pulse();

/*
 * Send a sonar pulse and convert the time between pulses into a distance in cm.
 * Stores the calculated distance into the global variable 'length' and returns the delta in cycles
 */
float sonar_getDistance();

#endif /* SONAR_H_ */
