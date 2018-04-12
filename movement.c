/*
 * movement.c
 *
 *  Created on: Jan 24, 2018
 *      Author: bryank
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
int move_forward(oi_t *sensor_data, int distance) {
    int sum = 0;
    oi_setWheels(250,250); //sets wheel speed to 100 mm/s
    while(sum<distance){
        oi_update(sensor_data); // updates the data from the sensor

        //cliff detection, moves back and stops if detected
        if(sensor_data->cliffFrontLeft)
        {
            sum+=move_backward(sensor_data, 100);
            break;
        }
        else if(sensor_data->cliffFrontRight)
        {
            sum+=move_backward(sensor_data, 100);
            break;
        }
        else if(sensor_data->cliffLeft)
        {
            sum+=move_backward(sensor_data, 100);
            break;
        }
        else if(sensor_data->cliffRight)
        {
            sum+=move_backward(sensor_data, 100);
            break;
        }

        //tape detection, moves back and stops if detected
        else if(sensor_data->cliffFrontRightSignal >2600)
        {
            sum+=move_backward(sensor_data, 100);
            break;
        }
        else if(sensor_data->cliffFrontLeftSignal>2600)
        {
            sum+=move_backward(sensor_data, 100);
            break;
        }
        else if(sensor_data->cliffRightSignal>2600)
        {
            sum+=move_backward(sensor_data, 100);
            break;
        }
        else if(sensor_data->cliffLeftSignal>2600)
        {
            sum+=move_backward(sensor_data, 100);
            break;
        }

        sum+= sensor_data->distance; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
    return distance;
}

/*
 * moves the bot backward the selected distance
 * NO CLIFF/TAPE CHECK
 * returns the distance moved
 */
int move_backward(oi_t *sensor_data, int distance) {
    int sum = 0;
    distance = 0-distance;
    oi_setWheels(-250,-250);
    while(sum>distance){
        oi_update(sensor_data); // updates the data from the sensor

        sum+= sensor_data->distance; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
    return distance;
}

void turn_counter_clockwise(oi_t *sensor_data, int degrees) {
    int angle = 0;
    oi_setWheels(300,-300);
    while(angle<(degrees)){ //calibrate?
        oi_update(sensor_data); // updates the data from the sensor
        angle+= sensor_data->angle; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
}

void turn_clockwise(oi_t *sensor_data, int degrees) {
    int angle = 0;
    degrees = 0-degrees;
    oi_setWheels(-300,300);
    while(angle>(degrees)){ //calibrate?
        oi_update(sensor_data); // updates the data from the sensor
        angle+= sensor_data->angle; // adds new data to the sum of difference
    }
    oi_setWheels(0,0); // stops the robot
}
