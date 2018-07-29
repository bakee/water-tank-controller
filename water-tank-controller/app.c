#include <stdint.h>
#include <avr/io.h>

#include "hal/interrupt.h"
#include "hal/sleep.h"
#include "hal/analog_comparator.h"

#include "bll/util.h"
#include "bll/ultrasonic.h"
#include "bll/timer.h"
#include "bll/uart.h"

#define HEART_BEAT_DDR DDRB
#define HEART_BEAT_PORT PORTB
#define HEART_BEAT_PIN PINB0

#define RELAY_DDR DDRB
#define RELAY_PORT PORTB
#define RELAY_PIN PINB1

#define MINIMUM_WATER_DISTANCE 250 // 25 cm
#define MAXIMUM_WATER_DISTANCE 650 // 65 cm

#ifdef DEBUG
#define INITIAL_DELAY_MS 2000
#else
#define INITIAL_DELAY_MS 64000 // 64 second initial delay to let the ultrasonic sensor to boot up
#endif // _DEBUG


uint8_t _isRelayOn = FALSE;

inline void initializePorts() {
  CLEAR_BIT(HEART_BEAT_PORT, HEART_BEAT_PIN);
  CLEAR_BIT(RELAY_PORT, RELAY_PIN);
  
  SET_BIT(HEART_BEAT_DDR, HEART_BEAT_PIN);
  SET_BIT(RELAY_DDR, RELAY_PIN);
}

inline void _turnRelayOn() {
  SET_BIT(RELAY_PORT, RELAY_PIN);
  _isRelayOn = TRUE;
}

inline void _turnRelayOff() {
  CLEAR_BIT(RELAY_PORT, RELAY_PIN);
  _isRelayOn = FALSE;
}

inline void _turnHeartBeatOn() {
  SET_BIT(HEART_BEAT_PORT, HEART_BEAT_PIN);
}

inline void _turnHeartBeatOff() {
  CLEAR_BIT(HEART_BEAT_PORT, HEART_BEAT_PIN);
}

inline void _toggleHeartBeat() {
  if(IS_BIT_SET(HEART_BEAT_PORT, HEART_BEAT_PIN)) {
    _turnHeartBeatOff();
    } else {
    _turnHeartBeatOn();
  }
}

inline void _enableGPIOPullUps() {
  DDRB = 0x00;
  PORTB = 0xFF;
  
  DDRC = 0x00;
  PORTC = 0xFF;
  
  DDRD = 0x00;
  DDRC = 0xFF;
}

void _initialDelay() {
  uint16_t initialTickCount = timer_getTickCount();
  while (timer_getTickCount() - initialTickCount < INITIAL_DELAY_MS) {
    _toggleHeartBeat();
    hal_sleep_enterSleepMode();
  }
}

inline void initialize() {
  hal_analog_comparator_disable();
  _enableGPIOPullUps();
  initializePorts();
  timer_initialize();
  _initialDelay();
  ultrasonic_initalize();
  
  hal_interrupt_enableGlobalInterrupt();
}

inline void handleRelayLogic() {
  uint16_t distanceToWater = ultrasonic_getDistanceInMillimeter();
  if(_isRelayOn && (distanceToWater < MINIMUM_WATER_DISTANCE)) {
    _turnRelayOff();
    } else if(!_isRelayOn && (distanceToWater > MAXIMUM_WATER_DISTANCE)) {
    _turnRelayOn();
  }
}

int main(void) {
  initialize();
  
  while(1) {
    timer_run();
    ultrasonic_run();
    uart_run();
    
    handleRelayLogic();
    
    _turnHeartBeatOff();
    hal_sleep_enterSleepMode();
    _turnHeartBeatOn();
  }
}
