/*
* ultrasonic.c
*
* Created: 7/19/2018 5:32:13 PM
*  Author: abdullah
*/

#include "../hal/ultrasonic.h"

#include "common.h"
#include "timer.h"
#include "ultrasonic.h"

#define MEASUREMENT_INTERVAL_IN_MILLISECONDS  5000

#define MAX_MEASUREMENTS_TO_STORE 32
uint16_t measurementArray[MAX_MEASUREMENTS_TO_STORE];
uint16_t measurementArrayIndex = 0;
uint16_t lastCalculatedDistanceInMillimeter = 0xFFFF;
volatile uint32_t lastTimerValue = 0;
volatile uint8_t isEchoReceived = FALSE;
uint16_t lastMeasurementTickCount = 0;

void _onEchoReceived(uint16_t distanceInMillimeter) {
  lastCalculatedDistanceInMillimeter = distanceInMillimeter;
  isEchoReceived = TRUE;
}

void _addCurrentMeasurementToArray(uint16_t measurement) {
  if(MAX_MEASUREMENTS_TO_STORE == measurementArrayIndex) {
    measurementArrayIndex = 0;
  }
  
  measurementArray[measurementArrayIndex++] = measurement;
}

uint8_t ultrasonic_getLastMeasurements(uint16_t* container) {
  uint8_t i;
  
  for (i = 0; i < measurementArrayIndex; ++i) {
    container[i]  = measurementArray[i];
  }
  
  measurementArrayIndex = 0;
  return i;
}

void ultrasonic_initalize() {
  hal_ultrasonic_onEchoReceived = _onEchoReceived;
  hal_ultrasonic_initalize();
}

void ultrasonic_triggerMeasurement() {
  isEchoReceived = FALSE;
  hal_ultrasonic_triggerMeasurement();
}

uint8_t ultrasonic_isResultReady() {
  return isEchoReceived;
}

uint16_t ultrasonic_getDistanceInMillimeter() {
  return lastCalculatedDistanceInMillimeter;
}

void ultrasonic_run() {
  // Trigger a measurement if interval reached
  if((timer_getTickCount() - lastMeasurementTickCount) > MEASUREMENT_INTERVAL_IN_MILLISECONDS) {
    ultrasonic_triggerMeasurement();
    lastMeasurementTickCount = timer_getTickCount();
  }
}
