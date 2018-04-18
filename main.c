/**
 * @file main.c
 * @brief this file will run the all the commands to the robot.
 * @author Jake Aunan
 * @author Ryan Goluchh
 * @author Bryan Kalkhoff
 * @author Justin Charette
 * @author Aaron Thune
 * @author Jonathan Novak
 *
 * @date April 14,2018
 */

#include <Timer.h>
#include <lcd.h>
#include <inc/tm4c123gh6pm.h>

#include "open_interface.h"
#include "movement.h"
#include "uart.h"
#include "Servo.h"
#include "lcd.h"
#include "Sonar.h"
#include "IR.h"
#include "Math.h"

//holds information about objects
struct object
{
    int rightSideAngle;
    int leftSideAngle;
    int distanceCheck; //right side distance
    char real;
};

//stores values from the sensors
int ir_distance[91];
int sonar_Distance[91];

//stores objects found
struct object objects[10];



/*
 * lab 9 functions and such
 * putting in main for now since its easier
 */

/**
 * This method will move the servo between 0-180 degree while collecting data on potential object.
 * @author Jake Aunan
 * @author Justin Charette
 * @author Bryan Kalkhoff
 * @date April 14, 2018
 */
void sweep() {
    int i=0;
    int total = 0;
    while (i<=90) {
        //used to make values strings to send over UART
        char irARR[100];
        char totalARR[100];
        char sonarARR[100];

        move_servo(total);
        ir_distance[i] = getDistance();
        sonar_Distance[i]=ping_getDistance();

        lcd_printf("angle: %d\nIR: %d\nPing: %d", total, ir_distance[i],sonar_Distance[i]);

//        //stores values as strings for UART
//        sprintf(irARR, "%4d", ir_distance[i]);
//        sprintf(sonarARR, "%4d", sonar_Distance[i]);
//        sprintf(totalARR, "%4d", total);
//
//        //sends data over UART
//        uart_sendStr(totalARR);
//        uart_sendStr("                 ");
//        uart_sendStr(irARR);
//        uart_sendStr("                   ");
//        uart_sendStr(sonarARR);
//        uart_sendChar('\r');
//        uart_sendChar('\n');

        total = total + 2;
        i++;
    }

}

/**
 * This method takes data from the struck and calculates the object's width
 * @author Bryan Kalkhoff
 * @author Jake Aunan
 * @param struct object that holds the info on the object
 * @date April 14,2018
 */
int get_width(struct object o)
{
    double checkAngle;
    int width;
    checkAngle = 0.5*(o.leftSideAngle-o.rightSideAngle);
    width = 2*o.distanceCheck*(sin(checkAngle));
    return width;
}

/**
 * goes through stored sensor data and finds objects
 * @author Jake Aunan
 * @author Ryan Goluchh
 * @date April 14, 2018
 * @return int number of obects
 */
int get_objects()
{
    int objectCount = 0;
    int index;
    int check = 0;
    int i;

    for(index=1; index<=90; index++)
    {
        //check if IR is valid
        if(ir_distance[index]<=60)
        {
            //check for sonar change (means object)
            if((abs(sonar_Distance[index-1] - sonar_Distance[index]) <5) && (check == 0))
            {
                objects[objectCount].rightSideAngle = (index*2);
                objects[objectCount].real = 1;
                objects[objectCount].distanceCheck = sonar_Distance[index];
                check = 1;
            }
        }
        else if((ir_distance[index]>60) && (check == 1))
        {
            objects[objectCount].leftSideAngle = (index*2);
            objectCount++;
            check = 0;
        }
    }

    /*
     * send object data over UART
     */
    char print[50];
    for (i = 0; i < 10; i++)
    {
        if(objects[i].real == 1)
        {
            sprintf(print, "%d", objects[i].rightSideAngle);
            uart_sendStr("object angle: ");
            uart_sendStr(print);
            sprintf(print, "%d", objects[i].distanceCheck);
            uart_sendStr("   object distance: ");
            uart_sendStr(print);
            uart_sendChar('\r');
            uart_sendChar('\n');
        }

    }
    return objectCount;
}


/**
 * Points to the right side of all objects and sends the angle over UART
 * @author Jake Aunan
 * @author Justin Charette
 * @author Ryan Goluchh
 * @date April 14,2018
 */
void point_to_objects()
{
    char i;
    for(i=0; i<10; i++)
    {
        if(objects[i].real == 1)
        {
            move_servo(objects[i].rightSideAngle);

            //debug stuff
            //            lcd_printf("%d", objects[i].rightSideAngle);
            //            char print[50];
            //            sprintf(print, "%d", objects[i].rightSideAngle);
            //            uart_sendChar('\r');
            //            uart_sendChar('\n');
            //            uart_sendStr("object angle: ");
            //            uart_sendStr(print);
            timer_waitMillis(1500);
        }
    }
}

/**
 * clears object array
 * @author Jake Aunan
 * @date April 14,2018
 */
void clear_objects()
{
    char i;
    for(i=0; i<10; i++){
        objects[i].rightSideAngle = 0;
        objects[i].leftSideAngle = 0;
        objects[i].distanceCheck = 0;
        objects[i].real = 0;
    }
}


/**
 * Gets the closes object and return its index in object array
 * then points to the object
 * @author Justin Charette
 * @author Jake Aunan
 * @author Bryan Kalkhoff
 * @date April 14, 2018
 */
int get_min()
{
    int i;
    int minIndex;
    int midPoint;
    struct object min = objects[0];

    for (i = 1; i < 10; i++)
    {
        if(objects[i].real == 1)
        {
            if (objects[i].distanceCheck < min.distanceCheck)
            {
                min = objects[i];
                minIndex = i;
            }
        }

    }

    //sends midpoint angle of closest object over UART for debugging
    midPoint = ((min.leftSideAngle - min.rightSideAngle)/2)+min.rightSideAngle;
    //    lcd_printf("%d", midPoint);
    //    char print[50];
    //    sprintf(print, "%d", midPoint);
    //    uart_sendChar('\r');
    //    uart_sendChar('\n');
    //    uart_sendStr("min angle: ");
    //    uart_sendStr(print);
    move_servo(midPoint);

    return minIndex;
}

/**
 * Moves the bot and returns the distance traveled if there was a valid command
 * @author Jake Aunan
 * @author Ryan Goluchh,
 * @param char move
 * @param oi_t *senssor_data
 * @param char *command
 * @date April 14, 2018
 */
int handle_input(char move, oi_t *sensor_data, char *command)
{
    int distance = 0;

    //sweep
    //author @Justin Charette (4/14)
    if(move == '5') {
        *command = 1;
    }
    //moves forward..dist
    if(move == '8'){
        //*command = 1;
        uart_sendStr("forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        distance = move_forward(sensor_data, 350);
    }
    //moves backward..dist
    else if(move == '2'){
        //*command = 1;
        uart_sendStr("backward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        distance = move_backward(sensor_data, 200);
    }
    //turns left 90
    else if(move == '4'){
        //*command = 1;
        uart_sendStr("left 90 degrees");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_counter_clockwise(sensor_data, 90);
    }
    //turns right 90
    else if(move == '6'){
        //*command = 1;
        uart_sendStr("right 90 degrees");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_clockwise(sensor_data, 90);
    }
    //turns left 45, then forward..dist
    else if(move == '7'){
        //*command = 1;
        uart_sendStr("left 45, then forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_counter_clockwise(sensor_data, 45);
        distance = move_forward(sensor_data, 500);
    }
    //turns right 45, then forward..dist
    else if(move == '9'){
        //*command = 1;
        uart_sendStr("right 45, then forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_clockwise(sensor_data, 45);
        distance = move_forward(sensor_data, 500);
    }
    return distance;
}

/**
 * main.c
 * 
 * This program runs by moving the bot around with commands sent over UART.
 * After each command is sent the bot will move and then sweep the area in front
 * of it for objects and display that data on PuTTy.
 *
 *
 * ALL CALIBRATION FOR BOT 13
 *
 * @author Jake Aunan
 * @author Ryan Goluchh
 * @author Bryan Kalkhoff
 * @author Justin Charette
 * @author Aaron Thune
 * @author Jonathan Novak
 * @date April 14, 2018
 */



int main(void)

{
    //initializes variables
    int objectCount; //number of objects found after a sweep
    int index; //not used currently
    char move; //holds the character command sent for movement
    int distance; //distance moved after each command
    char command = 0; //0 if a command hasn't been issued, 1 if it has. Used to know if it should sweep or not.

    //initializes peripherals
    servo_init();
    ir_init();
    sonar_init();
    move_servo(0);
    lcd_init();
    uart_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); //the sensors are initialized
    oi_setWheels(0,0); // stops the robot

    //runs the program
    while(1)
    {
        move = uart_receive(); //holds move command
        distance = handle_input(move, sensor_data, &command); //attempts to move bot with character sent over

        //performs a sweep and sends data if a move was made from a valid command
        if(command == 1){
            uart_sendStr("Sweeping...");
            sweep(); //performs sweep
            uart_sendChar('\r');
            uart_sendChar('\n');
            objectCount = get_objects(); //finds objects based on sweep data
            point_to_objects();
            uart_sendStr("..Finished");
            uart_sendChar('\r');
            uart_sendChar('\n');
            uart_sendChar('\r');
            uart_sendChar('\n');
            //point_to_objects();//debugging
            command = 0; //sets command back to 0 so it does not continually sweep

            void clear_objects();
        }
    }
}

