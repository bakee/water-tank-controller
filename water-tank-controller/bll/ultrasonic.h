/*
 * ultrasonic.h
 *
 * Created: 7/19/2018 5:32:23 PM
 *  Author: abdullah
 */ 

#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#include <stdint.h>

// 
#define CUSTOM_TO_CM (58 / 32)

void ultrasonic_triggerMeasurement();
uint8_t ultrasonic_isResultReady();
uint16_t ultrasonic_getDistanceInMillimeter();
uint8_t ultrasonic_getLastMeasurements(uint16_t* container);
void ultrasonic_initalize();
void ultrasonic_run();


#endif // _ULTRASONIC_H_
