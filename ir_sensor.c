/*
 * Sensor.c
 *
 *  Created on: Mar 6, 2018
 *      Author: jcnovak4
 */

#include "ir_sensor.h"

void ir_init(void)
{
    /*
     * GPIO Port B
     */

	//Enable system clock to Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	
	//Enable alternate function on GPIO Port B Pin 4
    GPIO_PORTB_AFSEL_R |= 0x10; //0b0001_0000
	
	//Set Port B Pin 4 as input
    GPIO_PORTB_DIR_R &= ~(0x10);
	
	//Disable Port B Pin 4 as digital pin
    GPIO_PORTB_DEN_R &= ~(0x10);
	
    //Disable analog isolation on Port B Pin 4
    GPIO_PORTB_AMSEL_R |= 0x10;

    //Clear Port B ADCCTL register, setting none of Port B pins as trigger for ADC
    //Note from page datasheet:
    //"If the Port B GPIOADCCTL register is cleared, PB4 can still be used as an external trigger
    //for the ADC"
    GPIO_PORTB_ADCCTL_R &= 0x00;

    /*
     * ADC0
     */

    //Enable system clock to ADC 0
    SYSCTL_RCGCADC_R |= 0x1;

    //Disable ADC0 Sample Sequencer 1 for configuration
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;

    //Set trigger source for all sample sequencers as processor
    ADC0_EMUX_R &= ADC_EMUX_EM1_PROCESSOR;

    //Set input source of the first sample on SS1 as ADC0 channel 10
    ADC0_SSMUX1_R |= 0x000A;

    //Enable interrupt for end of first sample conversion, and set first sample as 'end of sequence'
    ADC0_SSCTL1_R |= (ADC_SSCTL1_IE0 | ADC_SSCTL1_END0);

    //Set ADC0 to average 64 samples per sequencer result
    ADC0_SAC_R |= ADC_SAC_AVG_64X;

    //Re-enable ADC0 SS1
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;
}


unsigned read_ir(void)
{
    ADC0_PSSI_R |= ADC_PSSI_SS1;

    while(!(ADC0_RIS_R & ADC_RIS_INR1));

    ADC0_ISC_R |= ADC_ISC_IN1;
    return ADC0_SSFIFO1_R;
}
