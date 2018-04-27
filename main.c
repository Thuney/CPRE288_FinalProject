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
    int rightSideAngle;//holds right side angle
    int leftSideAngle; //holds left side angle
    int distanceCheck; //right side distance
    char real; //holds the information that it is real
};

//stores values from the sensors
int ir_distance[91];
int sonar_Distance[91];

//stores objects found
struct object objects[10];


/*
 *Puts the song into the interface.
 *@author Justin Charette
 *@author Bryan Kalkhoff
 *@date April 18, 2018
 */
void load_song(){
    unsigned char song[] = {55, 65};//sets the 2 notes
    unsigned char songDuration[] = {15, 30}; //sets the time held for each note
    oi_loadSong(1, 2, song, songDuration);//load the song to the interface
}



/**
 * Plays the song that was loaded to open interface.
 * @author Justin Charette
 * @author Bryan Kalkhoff
 * @date April 18, 2018
 */
void play_song(){
    oi_play_song(1); //plays the first song
}

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
        /*char irARR[100];
        char totalARR[100];
        char sonarARR[100];*/

        move_servo(total);
        ir_distance[i] = getDistance();
        sonar_Distance[i]=ping_getDistance();

        //lcd_printf("angle: %d\nIR: %d\nPing: %d", total, ir_distance[i],sonar_Distance[i]);

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

/**
 * This method takes data from the struck and calculates the object's width.
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
    width = 2*o.distanceCheck*(sin (checkAngle));
    return abs(width);
}

/**
 * goes through stored sensor data and finds objects.
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
    char print[50];

    for(index=1; index<=90; index++)
    {
        //check if IR is valid
        if(ir_distance[index]<70)
        {
            //check for sonar change (means object)
            if((abs(sonar_Distance[index-1] - sonar_Distance[index]) <5) && (check == 0))
            {
                objects[objectCount].rightSideAngle = (index*2);//sets the angle
                objects[objectCount].real = 1;//says that there is an object
                objects[objectCount].distanceCheck = sonar_Distance[index];//gets the distance of the object
                check = 1;//sets check to be 1
            }
        }
        else if((ir_distance[index]>=70) && (check == 1))//if the sensor is in the middle of an object and the IR is over a certain value
        {
            objects[objectCount].leftSideAngle = (index*2);//set the left angle
            objectCount++;//add one to objectcount
            check = 0;//reset check to 0
        }
    }

    /*
     * send object data over UART.
     * @author Jake Aunan
     * @author Ryan Goluchh
     * @date April 14, 2018
     */
    //Iterates through the array
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
 * Points to the right side of all objects and sends the angle over UART.
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
        }/*if there was an object*/
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
 * then points to the object.
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

    //for loop that goes through the array to find the min distance
    for (i = 1; i < 10; i++)
    {

        if(objects[i].real == 1) {//if it's a real object

            if (objects[i].distanceCheck < min.distanceCheck)
            {
                //set the new min distance
                min = objects[i];
                minIndex = i;
            }//if the distance is less then the current minimal distance
        }/*if the code is the middle of an object*/

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
 * Moves the bot and returns the distance traveled if there was a valid command.
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
    char time=0;
    char print[50];


    //sweep
    //author @Justin Charette (4/14)
    if(move == '5') {
        *command = 1;
    }
    //moves forward..dist
    if(move == '8'){
        *command = 1;
        uart_sendStr("forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        distance = (move_forward(sensor_data, 350))/10;


        sprintf(print, "%d", distance);
        uart_sendStr("distance: ");
        uart_sendStr(print);
        uart_sendStr("cm");
        uart_sendChar('\r');
        uart_sendChar('\n');
    }/*if move==8*/

    //moves forward..shorter dist5, not sweep
    if(move == '1'){
        uart_sendStr("short forward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        distance = (move_forward(sensor_data, 150))/10;


        sprintf(print, "%d", distance);
        uart_sendStr("distance: ");
        uart_sendStr(print);
        uart_sendStr("cm");
        uart_sendChar('\r');
        uart_sendChar('\n');
    }/*if move ==1*/

    //moves backward..dist
    else if(move == '2'){
        *command = 1;
        uart_sendStr("backward");
        uart_sendChar('\r');
        uart_sendChar('\n');
        distance = (move_backward(sensor_data, 200))/10;


        sprintf(print, "%d", distance);
        uart_sendStr("distance: ");
        uart_sendStr(print);
        uart_sendStr("cm");
        uart_sendChar('\r');
        uart_sendChar('\n');
    }/*if move ==2*/
    //turns left 90
    else if(move == '4'){
        *command = 1;
        uart_sendStr("left 90 degrees");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_counter_clockwise(sensor_data, 90);
    }/*if move ==4*/
    //turns right 90
    else if(move == '6'){
        *command = 1;
        uart_sendStr("right 90 degrees");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_clockwise(sensor_data, 90);
    }
    //turns left 15
    else if(move == '7'){
        uart_sendStr("left 15 degrees");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_counter_clockwise(sensor_data, 15);
    }/*if move ==7*/
    //turns right 15
    else if(move == '9'){
        uart_sendStr("right 15 degrees");
        uart_sendChar('\r');
        uart_sendChar('\n');
        turn_clockwise(sensor_data, 15);
    }
    else if(move=='0'){
        while(time<10){
            timer_waitMillis(1000);
            play_song();
            time++;
        }/*while time is less then 10 keep playing*/

    }/*if move ==0*/
    return distance;
}

///**
// * This function computes the distance between objects to
// * help check if we are in the goal zone since the distance between
// * all 4 pillars in the goal zone is ~61 cm.
// *
// * @author Ryan Goluch
// */
//int compute_dist_between_obj()
//{
//
//}

/**
 * main.c
 * 
 * This program runs by moving the bot around with commands sent over UART
 * After each command is sent the bot will move and then sweep the area in front.
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
    /**
     * Variables used to debug removed to get rid of warning- Bryan Kalkhoff.
     * @date April 25, 2018
     */
    /*  int objectCount; //number of objects found after a sweep
        int index; //not used currently*/

    int distance; //distance moved after each command
    char move; //holds the character command sent for movement
    char command = 0; //0 if a command hasn't been issued, 1 if it has. Used to know if it should sweep or not.
    int objectCount;

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
    load_song();
    lcd_printf("     FOLLOW ME\nHELP IS ON THE WAY");

    //runs the program
    while(1)
    {
        move = uart_receive(); //holds move command
        distance = handle_input(move, sensor_data, &command); //attempts to move bot with character sent over

        //performs a sweep and sends data if a move was made from a valid command
        if(command == 1){
            uart_sendStr("Sweeping...");
            uart_sendChar('\r');
            uart_sendChar('\n');
            sweep(); //performs sweep
            objectCount = get_objects(); //finds objects based on sweep data
            //point_to_objects();//debugging
            uart_sendStr("..Finished");
            uart_sendChar('\r');
            uart_sendChar('\n');
            uart_sendChar('\r');
            uart_sendChar('\n');
            command = 0; //sets command back to 0 so it does not continually sweep

            clear_objects();
            play_song();
        }/*if command is 1*/
    }
}

