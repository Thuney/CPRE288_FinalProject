/**
 *
 * 	@file  uart.c
 * 	@brief This file holds the methods for uart including initializing and sending.
 *
 *   @author Jake Aunan
 *   @author Ryan Goluchh
 *   @author Bryan Kalkhoff
 *   @author Justin Charette
 *   @author Aaron Thune
 *   @author Jonathan Novak
 *
 *   @date April 14, 2018
 */

#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include <inc/tm4c123gh6pm.h>
#include "WiFi.h"
#include <time.h>

/**
 * @author Aaron Thune
 * @author Jonathan Novak
 * @brief sets all necessary registers to enable the uart 1 module.
 * @date April 14, 2018
 */
void uart_init(void){
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; // initialize the clock
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1; //initialize the UART click
    GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1); // sets the alternate usage for port b
    GPIO_PORTB_PCTL_R |= 0x00000011; // enables rx /tx on port B
    GPIO_PORTB_DEN_R |= (BIT0 | BIT1 | BIT2); // Enables pin 1 and 2 on port B
    GPIO_PORTB_DIR_R &= ~BIT0; //sets pin 0 to rx
    GPIO_PORTB_DIR_R |= (BIT1 | BIT2); //sets pin 1 to Tx

    uint16_t iBRD = 8; //used for Baud Rate equation
    uint16_t fBRD = 44; //used for Baud rate equation
    UART1_CTL_R &= ~(UART_CTL_UARTEN);//turns off UART so that it can be set
    UART1_IBRD_R = iBRD; //sets baud rate
    UART1_FBRD_R = fBRD; //sets baud rate
    UART1_LCRH_R = UART_LCRH_WLEN_8; //sets frame, e data bits, 1 top bit,no parity, no FIFO
    UART1_CC_R = UART_CC_CS_SYSCLK; //uses the system clock for source
    UART1_CTL_R = ((UART_CTL_RXE) | (UART_CTL_TXE) | (UART_CTL_UARTEN)); //turns everything on and enables everything
}

/**@author  Aaron Thune
 * @author Jonathan Novak
 * @brief Sends a single 8 bit character over the uart 1 module.
 * @param data the data to be sent out over uart 1
 * @date April 14, 2018
 */
void uart_sendChar(char data){
    while(UART1_FR_R & 0x20){ //I am getting a weird error telling me UART1 undefined
    } // but only for the while loop, I even get it when I get rid of what's inside the while loop
    UART1_DR_R = data; //sends the data
}

/**
 * @author  Aaron Thune
 * @author Jonathan Novak
 * @brief polling receive an 8 bit character over uart 1 module.
 * @return the character received or a -1 if error occurred
 * @date April 14,2018
 */
int uart_receive(void){
    char rdata = 0;
    time_t endwait;
    time_t start = time(NULL);
    time_t seconds = 0.5;
    endwait = start + seconds;
    while((UART1_FR_R & UART_FR_RXFE)){ // busy wait, while it hasn't received anything it'll continue to be in this loop
        start = time(NULL);
        //ends while loop after certain amount of time to check for button presses
        if (start >= endwait) {
            return 128;
        }
    }
    rdata = (char)(UART1_DR_R & 0xFF); //masks the 4 error bits and only grabs the good stuff
    return rdata; //returns the data it recieved

}

/**
 * @author  Aaron Thune
 * @author Jonathan Novak
 * @brief sends an entire string of character over uart 1 module
 * @param data pointer to the first index of the string to be sent
 * @date April 14, 2018
 */
void uart_sendStr(const char *data){
    int i = 0;
    //iterates over string and uses sendChar
    for (i=0; data[i] != '\0'; i++) {
        uart_sendChar(data[i]);
    }
}
//        if the string is constant then we should know what the length is.
//        My idea was to send it letter be letter until it was done. not sure if this
//        the best way of doing it.
//          I didn't test this, although it should work


// part 1
/*
int main(void) {
    uart_init();
    lcd_init();
    button_init();
    int index = 0;
    //used to clear array after prints
    int i=0;
    int j=0;
    char myArray[21];
    while (1) {
        //once reaches 20 characters, prints array and then clears it
        if (index == 20) {
            lcd_printf("%s", myArray);
            for (i=0; i<20; i++) {
                myArray[i] = ' ';
            }
            index = 0;
        }
        //receives value from putty
        char value = uart_receive();
        //gets value if value is not enter
        if (value != '\r') {
            lcd_printf("char: %c \nindex: %d", value, index);
            myArray[index] = value;
            index++;
        }
        //prints current array if value is enter and clears array
        else {
            lcd_printf("%s", myArray);
            for (j=0; j<20; j++) {
                myArray[j] = ' ';
            }
            index = 0;
        }
    }
}*/

/*
int main(void) {
    uart_init();
    lcd_init();
    button_init();
    char paaa[9] = "12345678";
   //WiFi_start(paaa);
    int index = 0;
    //used to clear array after prints
    int i=0;
    int j=0;
    char myArray[21];
    myArray[20] = '\n';
    int last = 0;
    while (1) {
        //once reaches 20 characters, prints array and then clears it
        if (index == 20) {
            lcd_printf("%s", myArray);
            //automatically goes to next line after characters are sent to char
            uart_sendChar('\r');
            uart_sendChar('\n');
            for (i=0; i<20; i++) {
                myArray[i] = ' ';
            }

            index = 0;
        }
        //receives value from putty
        char value = uart_receive();

        //if no value is received from putty, then we check buttons
        if (value == 128) {
            int next = button_getButton();
            if (next == 1 && last != 1) {
                uart_sendStr("Yes");
            }
            else if (next == 2 && last != 2) {
                uart_sendStr("No");
            }
            else if (next == 3 && last != 3) {
                uart_sendStr("Blue, no green, Ahhhhh!!!");
            }
            else if (next == 4 && last != 4) {
                uart_sendStr("Hi, world");
            }
            else if (next == 5 && last !=5) {
                uart_sendStr("Bye, world :(");
            }
            else if (next == 6 && last != 6) {
                uart_sendStr("sw response");
            }
            last = next;
        }
        else if (value != '\r') {
            lcd_printf("char: %c \nindex: %d", value, index);
            myArray[index] = value;
            //sends value to putty
            uart_sendChar(value);
            index++;
        }
        //prints current array if value is enter and clears array
        else {
            //automatically goes to next line after enter is pressed
            uart_sendChar('\r');
            uart_sendChar('\n');
            lcd_printf("%s", myArray);
            for (j=0; j<20; j++) {
                myArray[j] = ' ';
            }
            index = 0;
        }
    }
}
*/
