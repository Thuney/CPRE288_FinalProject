/*
 * Sensor.c
 *
 *  Created on: Mar 6, 2018
 *      Author: jcnovak4
 */

#include <Sensor.h>
#include <math.h>

void Sensor_init(void)
{
	//Enable system clock to Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	
	//Enable system clock to ADC 0
    SYSCTL_RCGCADC_R |= 0x1;
	
	//Enable alternate function on GPIO Port B
    GPIO_PORTB_AFSEL_R |= 0x01;
	
	//Set Port B Pin 4 as input
    GPIO_PORTB_DIR_R &= 0b11101111;
	
	//Disable Port B Pin 4 as digital pin
    GPIO_PORTB_DEN_R &= 0b11101111;
	
    GPIO_PORTB_AMSEL_R |= 0x01;
    GPIO_PORTB_ADCCTL_R = 0x00;

    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;
    ADC0_EMUX_R = ADC_EMUX_EM1_PROCESSOR;
    ADC0_SSMUX1_R |= 0x000A;
    ADC0_SSCTL1_R |= (ADC_SSCTL1_IE0 | ADC_SSCTL1_END0);
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;
}


unsigned read_ir(void)
{
    ADC0_PSSI_R=ADC_PSSI_SS1;
    while((ADC0_RIS_R & ADC_RIS_INR1) == 0){

    }
    ADC0_ISC_R |= ADC_ISC_IN1;
    return ADC0_SSFIFO1_R;
}

void main()
{
    lcd_init();
    Sensor_init();
    unsigned dataAvg;
    int i=0;
    while(1){
        timer_waitMillis(100);
        for(i =0;i<15;i++){
            dataAvg += Sensor_reader();
        }
        dataAvg = dataAvg/16;
        double test1 = 1/pow(dataAvg,1.252);
        unsigned data2 = 175804*test1;
        lcd_printf("Value: %d, cm: %d", dataAvg, data2+4);
    }
}
