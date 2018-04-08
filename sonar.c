/*
 * sonar.c
 *
 *  Created on: Apr 4, 2018
 *      Author: adthune
 */

#include "sonar.h"

void TIMER3B_Handler(void)
{
    //Check if capture event has been triggered on Timer 3B
    if(TIMER3_MIS_R & TIMER_MIS_CBEMIS)
    {
        if(state == HIGH) //Outgoing pulse
        {
            //Clear interrupt
            TIMER3_ICR_R = TIMER_ICR_CBECINT;

            //Store time of outgoing pulse
            rising_time = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;

			//Set ping state to low (receiving pulse)
            state = LOW;
        }
        else if(state == LOW) //Incoming pulse
        {
			//Clear interrupt
            TIMER3_ICR_R = TIMER_ICR_CBECINT;
			
			//Store time of incoming pulse
            falling_time = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;
			
			//Set ping state to done
            state = DONE;

			//Disable timer
            TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
        }
    }
}

void sonar_init()
{
    /*
     * GPIO
     */

    //Set system clock to Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    //Set Pin 3 of Port B as an output
    GPIO_PORTB_DIR_R |= 0x08; //0b0000_1000

    //Enable Pin 3 of Port B as a digital pin
    GPIO_PORTB_DEN_R |= 0x08; //0b0000_1000

    //Set Pin 3 of Port B as a GPIO pin (Turn off Alternate Function)
    GPIO_PORTB_AFSEL_R &= ~(0x08); //~0b0000_1000

    //Disable mux signals to turn off alternate function
    GPIO_PORTB_PCTL_R &= ~(0x0000F000);

    //Set the output value of Pin 3 of Port B to LOW
    GPIO_PORTB_DATA_R &= 0xF7; //0b1111_0111

    /*
     * Timer 3B
     */

    //Enable system clock to Timer 3
    SYSCTL_RCGCTIMER_R |= 0x08;

    //Disable timer for configuration
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;

    //Configure Timer3B in 16-bit mode
    TIMER3_CFG_R |= 0x4; //0b0100

    //Enable Timer3B with dual edge select
    TIMER3_CTL_R |= 0xC00; //0b1100_0000_0000

    //Configure Timer3B in capture mode, edge-time mode, and counting up
    TIMER3_TBMR_R = 0b10111;

    //Load starting value into timer (2^32 - 1)
    TIMER3_TBILR_R |= 0xFFFF;

    //Enable capture mode event interrupt
    TIMER3_IMR_R |= 0x400; //0b0100_0000_0000

    //Clear Timer 3B Capture Mode Event Interrupt
    TIMER3_ICR_R |= 0x400; //0b0100_0000_0000

    //Enable Timer3B interrupt
    NVIC_EN1_R |= 0x10; //0b0001_0000

    //Register function to be a handler for the interrupt
    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    //Enable processor to receive all interrupts
    IntMasterEnable();

    //Enable timer 3B
    TIMER3_CTL_R |= TIMER_CTL_TBEN;
}

void emit_sonar_pulse()
{
    //Set Pin 3 of Port B as an output
    GPIO_PORTB_DIR_R |= 0x08; //0b0000_1000

    //Set Pin 3 of Port B as a GPIO pin (Turn off Alternate Function)
    GPIO_PORTB_AFSEL_R &= 0xF7; //0b1111_0111
    GPIO_PORTB_PCTL_R &= ~(0x0000F000);

    //Set the output value of Pin 3 of Port B to HIGH
    GPIO_PORTB_DATA_R |= 0x08; //0b0000_1000

    //Wait 5 microseconds
    timer_waitMicros(5);

    //Set the output value of Pin 3 of Port B to LOW
    GPIO_PORTB_DATA_R &= 0xF7; //0b1111_0111

    //Set Pin 3 of Port B as an input
    GPIO_PORTB_DIR_R &= 0xF7; //0b1111_0111

    //Set Pin 3 of Port B as an alternate function pin (Turn on Alternate Function)
    GPIO_PORTB_AFSEL_R |= 0x08; //0b0000_1000
	
	//Mux control for alternate function
    GPIO_PORTB_PCTL_R |= 0x00007000;

    //Set pulse state to HIGH
    state = HIGH;
}

float sonar_getDistance()
{
    //Send starting pulse
    emit_sonar_pulse();

    //Wait for completion of a ping
    while(state != DONE);

	//Length of pulse in clock cycles	
	length = falling_time - rising_time;
	
	//Compute if overflow occurs
    int overflow = (deltaCycles < 0);

	//Add term to account for overflow (24 1's if overflow)
    length += (overflow<<24);

	//Convert cycles to distance, dividing by 2 to account for return trip
    return ((deltaCycles/16000000)*34000)/2);
}
