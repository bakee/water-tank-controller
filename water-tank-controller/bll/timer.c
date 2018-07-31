/*
* timer.c
*
* Created: 7/19/2018 7:42:39 PM
*  Author: abdullah
*/

#include "../../hal/hal/timer.h"
#include "timer.h"


volatile uint16_t totalMilliSeconds = 0;
volatile uint16_t totalMicroSeconds = 0;

void _updateTime(uint16_t milliSecondsPerTick, uint16_t microSecondsPerTick) {
  totalMilliSeconds += milliSecondsPerTick;
  totalMicroSeconds += microSecondsPerTick;
}

void timer_initialize() {
  hal_timer_onTimer0Overflow = _updateTime;
  hal_timer0_initialize();
}

uint16_t timer_getTickCount() {
  return totalMilliSeconds;
}

void timer_run() {
  if (totalMicroSeconds > 1000) {
    totalMilliSeconds += (totalMicroSeconds / 1000);
    totalMicroSeconds %= 1000;
  }
}
