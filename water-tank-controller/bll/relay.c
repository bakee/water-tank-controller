#include <avr/io.h>

#include "../hal/gpio.h"

#include "common.h"
#include "relay.h"
#include "ultrasonic.h"

#define MINIMUM_WATER_DISTANCE 250 // 25 cm
#define MAXIMUM_WATER_DISTANCE 650 // 65 cm

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
  uint16_t distanceToWater = ultrasonic_getDistanceInMillimeter();
  if(relay_isTurnedOn() && (distanceToWater < MINIMUM_WATER_DISTANCE)) {
    relay_turnOff();
    } else if(!relay_isTurnedOn() && (distanceToWater > MAXIMUM_WATER_DISTANCE)) {
    relay_turnOn();
  }
}