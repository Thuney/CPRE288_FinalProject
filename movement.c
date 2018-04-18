/*
 * @file movement.c
 *  @brief This file contains the methods used to move the bot
 *  @author Bryan Kalkhoff
 *  @author Justin Charaette
 *  @author Jake Aunan
 *  @date Jan 24, 2018
 *
 */
#include <Timer.h>
#include <lcd.h>
#include "open_interface.h"
#include "movement.h"

/*
 * moves the bot forward the selected distance
 * if it hits the tape/cliff it reverses
 * returns the distance moved
 *
 * NEEDS BUMP SENSORS
 */
/**
 * Moves the bot forward the selected distance and reverses if the light sensor is activated
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
    oi_setWheels(249,220);
    while(sum<distance){
        oi_update(sensor_data); // updates the data from the sensor

        //cliff detection, moves back and stops if detected
        if(sensor_data->cliffFrontLeft)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }
        else if(sensor_data->cliffFrontRight)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }
        else if(sensor_data->cliffLeft)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }
        else if(sensor_data->cliffRight)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }

        //tape detection, moves back and stops if detected
        else if(sensor_data->cliffFrontRightSignal >2600)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }
        else if(sensor_data->cliffFrontLeftSignal>2600)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }
        else if(sensor_data->cliffRightSignal>2600)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }
        else if(sensor_data->cliffLeftSignal>2600)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }

        //Bump detection
        else if(sensor_data->bumpLeft == 1)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }

        else if(sensor_data->bumpRight == 1)
        {
            sum+=move_backward(sensor_data, 200);
            break;
        }

        sum+= sensor_data->distance; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
    center_bot_forward(sensor_data);
    return distance;
}

/*
 * moves the bot backward the selected distance
 * NO CLIFF/TAPE CHECK
 * returns the distance moved
 */
/**
 * moves the bot backward
 * @author Jake Aunan
 * @author Justin Charaette
 * @author Bryan Kalkhoff
 * @param oi_t *sensor
 * @param int distance
 * @return distance   the distance travled
 * @date April 14, 2018
 */
int move_backward(oi_t *sensor_data, int distance)
{
    int sum = 0;
    distance = 0-distance;
    oi_setWheels(-249,-220);
    while(sum>distance){
        oi_update(sensor_data); // updates the data from the sensor

        sum+= sensor_data->distance; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
    center_bot_backward(sensor_data);
    return distance;
}
/**
 * turns the bot counter clockwise
 * @author Justin Charaette
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
    oi_setWheels(300,-300);
    while(angle<(degrees)){ //calibrate?
        oi_update(sensor_data); // updates the data from the sensor
        angle+= sensor_data->angle; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
}
/**
 * turns the bot clockwise
 * @author Justin Charaette
 * @author Jake Aunan
 * @author Bryan Kalkhoff
 * @param oi_t * sensor
 * @param int distance
 * @date April 14, 2018
 */
void turn_clockwise(oi_t *sensor_data, int degrees)
{
    int angle = 0;
    degrees = 0-degrees;
    oi_setWheels(-300,300);
    while(angle>(degrees)){ //calibrate?
        oi_update(sensor_data); // updates the data from the sensor
        angle+= sensor_data->angle; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
}

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
