/*
 * @file movement.c
 *  @brief This file contains the methods used to move the bot
 *  @author Bryan Kalkhoff
 *  @author Justin Charette
 *  @author Jake Aunan
 *  @date Jan 24, 2018
 *
 */
#include <Timer.h>
#include <lcd.h>
#include "open_interface.h"
#include "movement.h"
#include "uart.h"


/**
 * Moves the bot forward the selected distance and reverses if the light sensor is activated.
 * @author Bryan Kalkhoff
 * @author Jake Aunan
 * @param oi_t *sensor_data
 * @param int distance  distance moved
 * @return int distance
 * @date April 14, 2018
 */
int move_forward(oi_t *sensor_data, int distance) {
    int sum = 0;
    //oi_setWheels(255,238); //sets wheel speed to 100 mm/s
    oi_setWheels(124,110);
    timer_waitMillis(75);
    oi_setWheels(266,240);
    while(sum<distance){
        oi_update(sensor_data); // updates the data from the sensor


        if(sensor_data->cliffFrontLeft)
        {
            uart_sendStr("HIT CLIFF");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if cliff detected move back*/
        else if(sensor_data->cliffFrontRight)
        {
            uart_sendStr("HIT CLIFF");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if cliff detected*/
        else if(sensor_data->cliffLeft)
        {
            uart_sendStr("HIT CLIFF");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if cliff detected*/
        else if(sensor_data->cliffRight)
        {
            uart_sendStr("HIT CLIFF");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if cliff detected*/


        else if(sensor_data->cliffFrontRightSignal >2600)
        {
            uart_sendStr("HIT TAPE");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if tape detected*/
        else if(sensor_data->cliffFrontLeftSignal>2600)
        {
            uart_sendStr("HIT TAPE");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if tape detected*/
        else if(sensor_data->cliffRightSignal>2600)
        {
            uart_sendStr("HIT TAPE");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if tape detected*/
        else if(sensor_data->cliffLeftSignal>2600)
        {
            uart_sendStr("HIT TAPE");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if tape detected*/


        else if(sensor_data->bumpLeft == 1)
        {
            uart_sendStr("HIT OBJECT");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if bump sensor*/

        else if(sensor_data->bumpRight == 1)
        {
            uart_sendStr("HIT OBJECT");
            uart_sendChar('\r');
            uart_sendChar('\n');

            sum+=move_backward(sensor_data, 200);
            break;
        }/*if bump sensor*/

        sum+= sensor_data->distance; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
    //center_bot_forward(sensor_data);
    return sum;
}


/**
 * moves the bot backward.
 * @author Jake Aunan
 * @author Justin Charette
 * @author Bryan Kalkhoff
 * @param oi_t *sensor
 * @param int distance
 * @return distance   the distance traveled
 * @date April 14, 2018
 */
int move_backward(oi_t *sensor_data, int distance)
{
    int sum = 0;
    distance = 0-distance;
    oi_setWheels(-257,-240);
    while(sum>distance){
        oi_update(sensor_data); // updates the data from the sensor

        sum+= sensor_data->distance; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
    //center_bot_backward(sensor_data);
    return sum;
}
/**
 * turns the bot counter clockwise.
 * @author Justin Charette
 * @author Jake Aunan
 * @author Bryan Kalkhoff
 * @param oi_t * sensor
 * @param int distance
 * @date April 14, 2018
 *
 */
void turn_counter_clockwise(oi_t *sensor_data, int degrees)
{
    int angle = 0;
    oi_setWheels(250,-250);

    if(degrees == 90){
        timer_waitMillis(739);
    }/*if degree is 90*/
    if(degrees == 15){
        timer_waitMillis(76);
    }/*if degree is 15*/
    //    while(angle<(degrees)){ //calibrate?
    //        oi_update(sensor_data); // updates the data from the sensor
    //        angle+= sensor_data->angle; // adds new data to the sum of difference
    //    }
    oi_setWheels(0,0); // stops the robot
}
/**
 * turns the bot clockwise.
 * @author Justin Charette
 * @author Jake Aunan
 * @author Bryan Kalkhoff
 * @param oi_t * sensor
 * @param int distance
 * @date April 14, 2018
 */
void turn_clockwise(oi_t *sensor_data, int degrees)
{
    int angle = 0;
    //degrees = 0-degrees;
    oi_setWheels(-250,250);

    if(degrees == 90){
        timer_waitMillis(740);
    }/*if degree is 90*/
    if(degrees == 15){
        timer_waitMillis(77);
    }/*if degree is 15*/
    //    while(angle>(degrees)){ //calibrate?
    //        oi_update(sensor_data); // updates the data from the sensor
    //        angle+= sensor_data->angle; // adds new data to the sum of difference
    //    }
    oi_setWheels(0,0); // stops the robot
}
/**
 * Moves the bot forward.
 * @author Jake Aunan
 * @param oi_t * sensor_data
 * @date April 14,2018
 */
void center_bot_forward(oi_t *sensor_data)
{
    int degrees = -4;
    int angle = 0;
    oi_setWheels(-17, 17);
    while (angle > (degrees))
    { //calibrate?
        oi_update(sensor_data); // updates the data from the sensor
        angle += sensor_data->angle; // adds new data to the sum of difference
    }
    oi_setWheels(0, 0); // stops the robot
}
/**
 * Moves the bot backward.
 * @author Jake Aunan
 * @param oi_t * sensor_data
 * @date April 14,2018
 */
void center_bot_backward(oi_t *sensor_data)
{
    int degrees = 4;
    int angle = 0;
    oi_setWheels(17, -17);
    while (angle < (degrees))
    { //calibrate?
        oi_update(sensor_data); // updates the data from the sensor
        angle += sensor_data->angle; // adds new data to the sum of difference
    }
    oi_setWheels(0, 0); // stops the robot
}
