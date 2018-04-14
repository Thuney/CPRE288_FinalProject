/*
 *  @file Sonar.c
 *
 *  @date Mar 7, 2018
 *      @author Justin Charaette
 *      @author Bryan Kalkhoff
 *      @brief This files contains all of the methods for sonar, including initialization and other methods for retrieving data.
 */

#include "Sonar.h"
#include "lcd.h"
#include "timer.h"
#include <inc/tm4c123gh6pm.h>


/**
 * This method initiates the ping sensor
 * @author Justin Charaette
 * @date March 7, 2018
 */
void sonar_init(void){
    //Turn on clock to TIMER3
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

    //Port B (bit1) system clock
    SYSCTL_RCGCGPIO_R |= 0x02;

    //Pin 3 init
    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;

    //Configure the timer for input capture mode
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;

    //Edge select
    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;

    //Set to a 16 bit counter
    TIMER3_CFG_R |= TIMER_CFG_16_BIT;

    //Capture mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP;

    //Edge-time mode
    TIMER3_TBMR_R |= TIMER_TBMR_TBCMR;

    //count up.
    TIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;

    //Set upper bound
    TIMER3_TBILR_R |= 0xFFFF;

    //enable capture input
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;

    //clears interrupt flags
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

    //105 and 142 interrupt #
    //Timer 3B: 36 Register 5, set Enable (EN1) BIT4 or 0x10
    NVIC_EN1_R |= 0x10;

    //register TIMER3B interrupt handler
    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    //initialize global interrupts
    IntMasterEnable();

    //TIMER3_CTL_R |= TIMER_CTL_TBEN;   //Enables TIMER3B
}

/**
 *   This method operates the ping sensor.
 */
/**
 * This method operates the ping sensor
 * @author Justin Charaette
 * @date March 7, 2018
 */
void send_pulse(){
    //alternative and port control (AFSEL and CTL)
    //alt function off
    GPIO_PORTB_AFSEL_R &= ~(0x08);

    //off
    GPIO_PORTB_PCTL_R &= ~(0x0000F000);

    // set PB3 as output
    GPIO_PORTB_DIR_R |= 0x08;

    // set PB3 to high
    GPIO_PORTB_DATA_R |= 0x08;

    // wait at least 5 microseconds based on data sheet
    timer_waitMicros(5);

    // set PB3 to low
    GPIO_PORTB_DATA_R &= 0xF7;
    state = RISE;

    // set PB3 as input
    GPIO_PORTB_DIR_R &= 0xF7;

    //alt function on
    GPIO_PORTB_AFSEL_R |= 0x08;

    //enable pin3 alternate function 7 T3CCP1
    GPIO_PORTB_PCTL_R |= 0x00007000;

    //enable TIMER3B and start counting
    TIMER3_CTL_R |= TIMER_CTL_TBEN;// 0x100
}

/**
 *   This method captures the time from when the sensor sends a pulse and it returns.
 */
/**
 * This method captures the time from when the sensor sends a pulse
 * @author Justin Charaette
 * @date March 7, 2018
 */
void TIMER3B_Handler(void)
{
    // fixed condition
    if(TIMER3_MIS_R & TIMER_MIS_CBEMIS) {
        if(state == RISE)
        {
            //clear capture interrupt flag
            TIMER3_ICR_R = TIMER_ICR_CBECINT;

            //captures time of rising edge event
            risingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;

            //now capturing falling edge
            state = FALL;
        }
        else if(state == FALL)
        {
            //clear capture interrupt flag
            TIMER3_ICR_R = TIMER_ICR_CBECINT;

            //capture time of falling edge
            fallingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;
            state = DONE;

            //disable timerB
            TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
        }
    }
}


/**
 * This method collects the ping sensor data.
 * @author Justin Charaette
 * @Bryan Kalkhoff
 * date March 7, 2018
 */
float ping_getDistance()
{
    send_pulse();

    //Wait for completion of a ping
    while(state != DONE);

    //Compute if overflow occurs
    int overflow = (fallingEdge < risingEdge);

    //Length of pulse in clock cycles
    //Add term to account for overflow (16 1's if overflow)
    pulseLength = fallingEdge - risingEdge + ((overflow << 16) - 1);

    //Convert cycles to distance, dividing by 2 to account for return trip
    return ((((float)pulseLength/16000000)*34300)/2.0);


    //    send_pulse();
    //
    //    //wait until end of pulse
    //    while(state != DONE)
    //    {
    //
    //    }
    //
    //    //Correction for overflow
    //    overflow += (fallingEdge < risingEdge);
    //
    //    pulseLength = ((unsigned long)overflow<<24) + fallingEdge - risingEdge;
    //    float pulse = pulseLength / 2078.0f;
    //    lcd_printf("distance: %.2f cm\npulse width: %d\npulse width: %.1f ms\noverflow: %d", pulseLength * PING_FACTOR, pulseLength, pulse, overflow);
    //
    //    //calculate distance in cm
    //    return pulseLength * PING_FACTOR;
}

//part 1, sends pulse at regular intervals
/*
int main() {
    lcd_init();
    sonar_init();
    while (1) {
        timer_waitMillis(1000);
        send_pulse();
    }
    return 0;
}

//part 3, continuous distance measurement
int main() {
    lcd_init();
    sonar_init();
    while (1) {
        timer_waitMillis(500);
        ping_getDistance();
    }
}*/
