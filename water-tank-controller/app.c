#include <stdint.h>
#include <avr/io.h>

#include "hal/interrupt.h"
#include "hal/sleep.h"
#include "hal/analog_comparator.h"
#include "hal/gpio.h"

#include "bll/util.h"
#include "bll/ultrasonic.h"
#include "bll/timer.h"
#include "bll/uart.h"
#include "bll/relay.h"

#define SHORT_DELAY 800


static uint16_t _initializingHearbeatTimer = 0;

inline void _toggleHeartBeat() {
  if(hal_gpio_isHeartBeatLEDOn()) {
    hal_gpio_turnOffHeartBeatLED();
    } else {
    hal_gpio_turnOnHeartBeatLED();
  }
}

void _showInitializingHearbeat(){
  if(timer_getTickCount() - _initializingHearbeatTimer > SHORT_DELAY) {
    _toggleHeartBeat();
    _initializingHearbeatTimer = timer_getTickCount();
  }
  hal_sleep_enterSleepMode();
}

void _showRunningHearbeat() {
  hal_gpio_turnOffHeartBeatLED();
  hal_sleep_enterSleepMode();
  hal_gpio_turnOnHeartBeatLED();
}

inline void initialize() {
  hal_analog_comparator_disable();
  hal_gpio_enableGPIOPullUps();
  hal_gpio_initializeHeartbeatLED();
  relay_initialize();
  timer_initialize();
  ultrasonic_initalize();
  uart_setupTriggerPin();
  
  hal_interrupt_enableGlobalInterrupt();
}

int main(void) {
  initialize();
  
  while(1) {
    timer_run();
    relay_run();
    ultrasonic_run();
    uart_run();
    
    if(ultrasonic_isReady()) {
      _showRunningHearbeat();
      } else {
      _showInitializingHearbeat();
    }
  }
}
