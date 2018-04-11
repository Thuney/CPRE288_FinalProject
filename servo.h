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

#include "driverlib/interrupt.h"
#include <math.h>
#include "Timer.h"

/*
 * Initializes servo settings
 *
 * Uses:
 *  GPIO Port B Pin 5 (as output as a PWM mode Timer)
 *  Timer 1B (for capturing time events)
 */
void servo_init();

/*
 * Convert a change in degree to a change in prescaled timer counts
 */
int calculateDeltaCount(double delta_deg);

/*
 * Move the servo the specified (signed) number of degrees away from its current position
 */
void move_servo(int degrees);

/*
 * Set the servo to a specified (unsigned) degree measurement
 */
void set_servo(unsigned degrees);


#endif /* SERVO_H_ */
