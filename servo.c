/**
 *
 * 	@file  servo.c
 *
 *   @author Jonathan Novak
 *
 */

#include "servo.h"

//Calibrated values
int max = 37300;	 	//Match value at theta = 180 degrees
int mid = 23031;	 	//Match value at theta = 90 degrees
int min = 8135;		 	//Match value at theta = 0 degrees
double msPerDegree = 0.006172; 	//Approximately (2.25-0.75)/180 (based on datasheet for servo)

//Store current count and degrees
double degrees = 0;
int count = 12000; //Theta = 0 degrees

int upOrDown = 1; //1 if up, -1 if down

void servo_init()
{
	/*
	 *    GPTMCTL – Control
	 *    GPTMCFG – Configuration
	 *    GPTMTnMR – Timer n mode
	 *    GPTMTnPR – Timer n prescale / 8 bits PWM
	 *    GPTMTnILR – Timer n interval load
	 *    GPTMTnPMR – Timer n prescale match
	 *    GPTMTnMATCHR – Timer n match
	 *    GPTMIMR – Interrupt mask
	 *    GPTMRIS – Raw interrupt status
	 *    GPTMICR – Interrupt clear
	 */

	//Enable system clock for Port B
    SYSCTL_RCGCGPIO_R |= 0x2; //0b0010
	
    //Enable alternate function on Port B Pin 5
    GPIO_PORTB_AFSEL_R |= 0x20; //0b0010_0000
	
    //Enable T1CCP1 (Timer 1) on Port B
    GPIO_PORTB_PCTL_R |= 0x00700000;
	
    //Set Port B Pin 5 as output
    GPIO_PORTB_DIR_R |= 0x20; //0b0010_0000
	
    //Enable PB5 as digital pin
    GPIO_PORTB_DEN_R |= 0x20; //0b0010_0000
	
    //Enable system clock to Timer 1
    SYSCTL_RCGCTIMER_R |= 0x02; //0b0000_0010
	
	//Disable Timer1B for configuration
    TIMER1_CTL_R &= ~(0x00000100);
	
	//Set as 16-bit timer
    TIMER1_CFG_R |= 0x4; //0b0000_0100
	
    //PWM enable
    TIMER1_TBMR_R = TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBAMS;
	
    //20 ms period is 320,000 cycles
	
	//0x4E200 = 320,000 (base 10)
    TIMER1_TBILR_R = 0xFA00; //Load value of 64,000 into our interval load register
    TIMER1_TBPR_R = 0x4;     //Prescaler of 4
	
	//Move servo to default position
    set_servo_pos(90);
	
	//enable Timer1B
    TIMER1_CTL_R |= 0x00000100;
}

int calculateDeltaCount(double delta_deg)
{
    return ((((msPerDegree*delta_deg)/20)*320000)/5);
}

void move_servo(int degree)
{	
	//Calculate change in counter value
	count += calculateDeltaCount(degree)
	count = (count > max) ? max : (count < min) ? min : count;

    //Set Match Register Value, adjusting pulse width
	//(320,000/5) - counter
    TIMER1_TBMATCHR_R = ((0x4E200 / 5) - counter);
	//Prescaler of 4
    TIMER1_TBPMR_R =  = 0x4;
	
    //Update current position in degrees
    degrees += degree;

    if (upOrDown == -1)
        lcd_printf("%d count \n%d degrees\ndecrementing", count, (int) degrees);
    else
        lcd_printf("%d count \n%d degrees\nincrementing", count, (int) degrees);
	
	//Wait for servo to move
    timer_waitMillis(300);
}

void set_servo_pos(unsigned degrees)
{
	//Set Match Register Value, adjusting pulse width
	
	//(((320,000 - 12,000) / 5) - counter)
    TIMER1_TBMATCHR_R = (((0x4E200-12000) / 5) - counter);
	//Prescaler of 4
    TIMER1_TBPMR_R = 0x4;
	
	lcd_printf("Position set to %d degrees", degrees);
	
	//Wait for servo to move
    timer_waitMillis(300);
}
