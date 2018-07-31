/*
 * ultrasonic.h
 *
 * Created: 7/19/2018 5:32:23 PM
 *  Author: abdullah
 */ 

#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#include <stdint.h>

void ultrasonic_triggerMeasurement();
uint8_t ultrasonic_isResultReady();
void ultrasonic_setMeasurementInterval(uint16_t interval);
uint16_t ultrasonic_getDistanceInMillimeter();
uint8_t ultrasonic_getLastMeasurements(uint16_t* container);
uint8_t ultrasonic_isReady();
void ultrasonic_initalize();
void ultrasonic_run();

#endif // _ULTRASONIC_H_
