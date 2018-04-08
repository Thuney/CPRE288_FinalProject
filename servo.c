/**
 *
 * 	@file  servo.c
 *
 *
 *
 *
 *
 *   @author Jonathan Novak
 *
 */

#include "servo.h"
#include "timer.h"
#include "button.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <math.h>

//Calibration
int max = 37300;
int mid = 23031;
int min = 8135;
double a = 0.006172;
double b = -50.20922;

//stores current count and degrees
double degrees = 0;
int count = 0;

int upordown = 1;

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
    TIMER1_TBPR_R = 0x4;    //Prescaler of 4
	
	//Move servo to default position
    move_servo(mid);
	
	//enable Timer1B
    TIMER1_CTL_R |= 0x00000100;

}

double countToDeg(int count)
{
    return (double) a * count + b;
}

int degreesToCount(double deg)
{
    return (deg - b) / a;;
}
/**
 * variables that store the count for each step of a certain degree, 1, 2.5, 5 degrees
 */
int res1;
int res25;
int res5;

/**
 * calculates the count resolution for each step of a certain degree
 */
void calibrate()
{
    res1 = degreesToCount(2) - degreesToCount(1);
    res25 = degreesToCount(3.5) - degreesToCount(1);
    res5 = degreesToCount(6) - degreesToCount(1);
}

int lastButton = 0;
int buttonPressed = 0;

void buttonSwitch(uint8_t button)
{
    switch (button)
    {
    case 6:
        if ((upordown < 0 && count < min) || (upordown >= 0 && count > max))
        {
            return;
        }
        count += res1 * upordown;
        move_servo(count);
        lastButton = 6;
        break;
    case 5:
        if ((upordown < 0 && count < min) || (upordown >= 0 && count > max))
        {
            return;
        }
        count += res25 * upordown;
        move_servo(count);
        lastButton = 5;
        break;
    case 4:
        if ((upordown < 0 && count < min) || (upordown >= 0 && count > max))
        {
            return;
        }
        count += res5 * upordown;
        move_servo(count);
        lastButton = 4;
        break;
    case 3:
        upordown *= -1;
        lastButton = 3;
        break;
    default:
        lastButton = 0;
        break;
    }

    if (upordown == -1) {
        lcd_printf("count: %d \ndegrees: %d \ndecrementing", count, (int) degrees);
    } else {
        lcd_printf("count: %d \ndegrees: %d \nincrementing", count, (int) degrees);
    }
}

void move_servo(unsigned counter)
{
    //set match value
    TIMER1_TBMATCHR_R = (0x4E200 - counter) & 0xFFFF;
    TIMER1_TBPMR_R = (0x4E200 - counter) >> 16;
    //update current position in count and degrees
    count = counter;
    degrees = countToDeg(count);

    if (upordown == -1) {
        lcd_printf("%d count \n%d degrees\ndecrementing", count, (int) degrees);
    } else {
        lcd_printf("%d count \n%d degrees\nincrementing", count, (int) degrees);
    }
    timer_waitMillis(300);
}

//void main(){
//    lcd_init();
//    servo_init();
//    calibrate();
//    button_init();
//    while(1){
//        int button = button_getButton();
//        buttonSwitch(button);
//    }
//}
