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

#ifdef DEBUG
#define INITIAL_DELAY_MS 2000
#else
#define INITIAL_DELAY_MS 64000 // 64 second initial delay to let the ultrasonic sensor to boot up
#endif // _DEBUG

#define SHORT_DELAY 800

inline void _toggleHeartBeat() {
  if(hal_gpio_isHeartBeatLEDOn()) {
    hal_gpio_turnOffHeartBeatLED();
    } else {
    hal_gpio_turnOnHeartBeatLED();
  }
}

void _initialDelay() {
  uint16_t initialTickCount = timer_getTickCount();
  uint16_t shortDelayCount = initialTickCount;
  while (timer_getTickCount() - initialTickCount < INITIAL_DELAY_MS) {
    if(timer_getTickCount() - shortDelayCount > SHORT_DELAY) {
      _toggleHeartBeat();
      shortDelayCount = timer_getTickCount();
    }
    hal_sleep_enterSleepMode();
  }
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
  
  _initialDelay();
  
  while(1) {
    timer_run();
    ultrasonic_run();
    uart_run();
    relay_run();
    
    hal_gpio_turnOffHeartBeatLED();
    hal_sleep_enterSleepMode();
    hal_gpio_turnOnHeartBeatLED();
  }
}
