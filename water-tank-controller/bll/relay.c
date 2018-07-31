#include <avr/io.h>

#include "../hal/gpio.h"

#include "common.h"
#include "relay.h"
#include "ultrasonic.h"

#define MINIMUM_VALID_DISTANCE 200 // 20 cm
#define MAXIMUM_VALID_DISTANCE 2000 // 2 meter

#define MINIMUM_WATER_DISTANCE 250 // 25 cm
#define MAXIMUM_WATER_DISTANCE 650 // 65 cm

#ifdef DEBUG
#define MEASUREMENT_INTERVAL_SLOW 1000 // 1 minute
#define MEASUREMENT_INTERVAL_FAST 1000 // 10 second
#else
#define MEASUREMENT_INTERVAL_SLOW 60000 // 1 minute
#define MEASUREMENT_INTERVAL_FAST 10000 // 10 second
#endif

void relay_turnOn() {
  hal_gpio_turnOnRelay();
}

void relay_turnOff() {
  hal_gpio_turnOffRelay();
}

uint8_t relay_isTurnedOn() {
  return hal_gpio_isRelayOn();
}

void relay_initialize() {
  hal_gpio_initializeRelay();
}

void relay_run() {
  // Wait for ultrasonic module to be ready to get a valid measurement
  if(!ultrasonic_isReady()) {
    return;
  }
  
  uint16_t distanceToWater = ultrasonic_getDistanceInMillimeter();
  
  // Filter erroneous measurements
  if(distanceToWater < MINIMUM_VALID_DISTANCE || distanceToWater > MAXIMUM_VALID_DISTANCE) {
    distanceToWater = MINIMUM_WATER_DISTANCE;
  }
  
  if(relay_isTurnedOn() && (distanceToWater <= MINIMUM_WATER_DISTANCE)) {
    relay_turnOff();
    ultrasonic_setMeasurementInterval(MEASUREMENT_INTERVAL_SLOW);
    } else if(!relay_isTurnedOn() && (distanceToWater > MAXIMUM_WATER_DISTANCE)) {
    relay_turnOn();
    ultrasonic_setMeasurementInterval(MEASUREMENT_INTERVAL_FAST);
  }
}