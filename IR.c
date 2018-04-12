/*
 * IR.c
 *
 *  Created on: Feb 28, 2018
 *      Author: charette
 */
#include "lcd.h"
#include "timer.h"
#include <inc/tm4c123gh6pm.h>
#include <math.h>


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
/*double convert_bot17(int value){
    double new_value =0;
    double val_convert =0;
    val_convert = (.000732)*value;

    new_value = 4.2248*pow(val_convert,1.3522);
    return new_value;
}*/

float getDistance() {
    int i=0, increments = 100;
    int isr_int_value = 0;
    //compute running average of 100 samples
    for (i=0; i<increments; i++) {
        timer_waitMillis(1);
        isr_int_value += ir_read(); //reads value and adds it to the total value
    }
    int quantNum = isr_int_value / increments; //gets the average of the 100 cycles
    //equation found from excel
    float distance = 232335*pow(quantNum, -1.286);
    return distance;
}
/*
    int main(void){
    lcd_init();
    ir_init();
    int raw_value =0;
    float convert_value=0;
    while(1){
        raw_value = ir_read();
        timer_waitMillis(400);
        raw_value = ir_read(); // I did this because I found that it would get a value, wouldn't be that accurate but use that for the conversion
        //making it wait for a set amount of time made it so it had the possiblility that the second scan would be more accurate
        convert_value = getDistance();
        //lcd_printf("%d",raw_value);
        lcd_printf("%.02lf cm \n%d",convert_value,raw_value); // the .02 makes it so it only prints to the second decimal place in the float


    }

}
*/





