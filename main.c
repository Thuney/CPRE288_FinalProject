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

//sweeps 0-180 degrees taking in sensor data every 2 degrees
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

                //stores values as strings for UART
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

//calculates width of an object and returns it
//DOESNT WORK
int get_width(struct object o)
{
    double checkAngle;
    int width;
    checkAngle = 0.5*(o.leftSideAngle-o.rightSideAngle);
    width = 2*o.distanceCheck*(sin(checkAngle));
    return width;
}

//goes through stored sensor data and finds objects
//returns number of obects
int get_objects()
{
    int objectCount = 0;
    int index;
    int check = 0;
    int i;

    for(index=1; index<=90; index++)
    {
        //check if IR is valid
        if(ir_distance[index]<=70)
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
        else if((ir_distance[index]>70) && (check == 1))
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
    for (i = 1; i < 10; i++)
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

//points to the right side of all objects and sends angle over UART
// used for debugging
void point_to_objects()
{
    int i;
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

//gets the closest object and returns its index in object array
//also points to it
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
/*
 * Moves the bot and returns distance traveled if there was a valid command
 */
int handle_move(char move, oi_t *sensor_data, char *command)
{
    int distance = 0;

    //moves forward..dist
    if(move == '8'){
        *command = 1;
        uart_sendStr("forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        distance = move_forward(sensor_data, 200);
    }
    //moves backward..dist
    else if(move == '2'){
        *command = 1;
        uart_sendStr("backward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        distance = move_backward(sensor_data, 200);
    }
    //turns left 90, then forward..dist
    else if(move == '4'){
        *command = 1;
        uart_sendStr("left 90, then forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_counter_clockwise(sensor_data, 90);
        distance = move_forward(sensor_data, 200);
    }
    //turns right 90, then forward..dist
    else if(move == '6'){
        *command = 1;
        uart_sendStr("right 90, then forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_clockwise(sensor_data, 90);
        distance = move_forward(sensor_data, 200);
    }
    //turns left 45, then forward..dist
    else if(move == '7'){
        *command = 1;
        uart_sendStr("left 45, then forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_counter_clockwise(sensor_data, 45);
        distance = move_forward(sensor_data, 200);
    }
    //turns right 45, then forward..dist
    else if(move == '9'){
        *command = 1;
        uart_sendStr("right 45, then forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_clockwise(sensor_data, 45);
        distance = move_forward(sensor_data, 200);
    }
    return distance;
}

/**
 * main.c
 * 
 * This program runs by moving the bot around with commands sent over UART.
 * After each command is sent the bot will move and then sweep the area in front
 * of it for objects and display that data on PuTTy.
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
        distance = handle_move(move, sensor_data, &command); //attempts to move bot with character sent over
		
		//performs a sweep and sends data if a move was made from a valid command
        if(command == 1){
            uart_sendStr("Sweeping..."); 
            sweep(); //performs sweep
            uart_sendChar('\r');
            uart_sendChar('\n');
            objectCount = get_objects(); //finds objects based on sweep data
            uart_sendStr("..Finished");
            uart_sendChar('\r');
            uart_sendChar('\n');
            //point_to_objects();//debugging
            command = 0; //sets command back to 0 so it does not continually sweep
        }
    }
}
