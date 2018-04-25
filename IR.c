/*
 * @file IR.c
 * @brief Holds the initialization and methods needed to run ir.
 * @author Justin Charette
 * @author Bryan Kalkhoff
 * @date February 28, 2018
 */
#include "lcd.h"
#include "timer.h"
#include <inc/tm4c123gh6pm.h>
#include <math.h>

/**
 * this method initializes the IR sensor.
 * @uthor Justin Charette
 * @author Bryan Kalkhoff
 * @date April 14, 2018
 */
void ir_init(void){
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; // initialize the clock
    SYSCTL_RCGCADC_R |= 0x1; //enabled ADC0
    GPIO_PORTB_AFSEL_R |= 0x01; //sets alternate function
    GPIO_PORTB_DIR_R &= 0b11111110;
    GPIO_PORTB_DEN_R |= 0x00; //makes pin 4 analog
    GPIO_PORTB_AMSEL_R |= 0x01;// disables analog isolation
    GPIO_PORTB_ADCCTL_R = 0x00; //initialize the port trigger source as processor

    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;//disables SS0 so we can congifure it
    ADC0_EMUX_R = ADC_EMUX_EM1_PROCESSOR;//initialize the ADC trigger source as processor
    ADC0_SSMUX1_R |= 0x000A; //sets for the sampling to be done in MUX 7
    ADC0_SSCTL0_R |= (ADC_SSCTL1_IE0 | ADC_SSCTL1_END0); //enabled raw interrupt status
    ADC0_SAC_R |= ADC_SAC_AVG_64X; //enable oversampling to average
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN1; //re-enable ADC0 SS0
}
/**
 * This method reads from the IR and returns the value.
 * @author Justin Charette
 * @author Bryan Kalkhoff
 * @return unsigned value
 * @date April 14, 2018
 *
 */
unsigned ir_read(){
    //disable ADC0SS1 to configure
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;
    //set 1st sample to use channel ADC pin
    ADC0_SSMUX0_R |= 0;
    //re-enable
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;
    //initiate SS1 conversion
    ADC0_PSSI_R = ADC_PSSI_SS1;
    //wait
    while ((ADC_RIS_INR1) == 0) {}
    //clear interrupt
    ADC0_ISC_R = ADC_ISC_IN1;
    int value = ADC0_SSFIFO1_R;
    return value;
}
/**
 * Code was removed because bot17 wasn't used so we didn't need the conversion.
 * @author Bryan Kalkhoff
 * @date April 14, 2018
 */
/*double convert_bot17(int value){
    double new_value =0;
    double val_convert =0;
    val_convert = (.000732)*value;

    new_value = 4.2248*pow(val_convert,1.3522);
    return new_value;
}*/
/**
 * This method converts the value read by the ADC and returns the distance.
 * @author Justin Charette
 * @author Bryan Kalkhoff
 * @return float distance   converted distance
 * @date April 14, 2018
 */
float getDistance() {
    int i=0, increments = 100;
    int isr_int_value = 0;
    //compute running average of 100 samples
    for (i=0; i<increments; i++) {
        timer_waitMillis(1);
        isr_int_value += ir_read(); //reads value and adds it to the total value
    }
    int quantNum = isr_int_value / increments; //gets the average of the 100 cycles
    //equation found for BOT 13
    float distance = 120283*pow(quantNum, -1.197);
    return distance;
}





