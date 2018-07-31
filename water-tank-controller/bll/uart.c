#include "../hal/uart.h"
#include "../hal/interrupt.h"
#include "../hal/gpio.h"

#include "common.h"
#include "buffer.h"
#include "util.h"
#include "uart.h"
#include "timer.h"
#include "ultrasonic.h"

#ifdef DEBUG
#define NEWLINE_CHAR ('\r')
#else
#define NEWLINE_CHAR ('\n')
#endif

volatile uint8_t isTransmitPending = FALSE;

#define RX_BUFFER_SIZE 32
DEFINE_GLOBAL_BUFFER(rxBuffer, RX_BUFFER_SIZE);

#define TX_BUFFER_SIZE 64
DEFINE_GLOBAL_BUFFER(txBuffer, RX_BUFFER_SIZE);

#define DISPLAY_INTERVAL 1000
uint16_t lastDisplayTickCount = 0;

volatile uint8_t isCommandPending = FALSE;
uint8_t _isDisableRequested = FALSE;
uint8_t _isInitialized = FALSE;
volatile uint8_t _isEnableRequested = FALSE;

// Private functions

void _processCommand() {
  char command[10];
  uint8_t i, data, len = 0;
  for (i = 0; i < 10; ++i) {
    buffer_takeCharFromBuffer(&rxBuffer, &data);
    if(data == NEWLINE_CHAR) {
      break;
    }
    command[i] = data;
    len++;
  }
  
  if (stringCompare("disable", command, 7)) {
    _isDisableRequested = TRUE;
    uart_sendString("Disabling UART", 14);
    } else {
    uart_sendString("Unknown", 7);
  }
  
  uart_sendLineBreak();
}

void _transmit() {
  uint8_t data;
  if(buffer_takeCharFromBuffer(&txBuffer, &data)) {
    hal_uart_sendByte(data);
  }
}

void _onByteReceived(uint8_t receivedByte) {
  buffer_putCharToBuffer(&rxBuffer, receivedByte);
  if(receivedByte == NEWLINE_CHAR) {
    isCommandPending = TRUE;
  }
}

inline void _setTransmitPending() {
  isTransmitPending = TRUE;
}

void _onEnableRequest() {
  _isEnableRequested = TRUE;
}

// End of private functions
void uart_sendString(const char* string, uint8_t length) {
  buffer_putStringToBuffer(&txBuffer, string, length);
  _setTransmitPending();
}

void uart_sendChar(char c) {
  buffer_putCharToBuffer(&txBuffer, c);
  _setTransmitPending();
}

void uart_sendNumber(uint16_t number) {
  char distanceString[5];
  uint8_t digitCount = numberToString(number, distanceString);
  uart_sendString(distanceString, digitCount);
}

void uart_sendLineBreak() {
  uart_sendChar(NEWLINE_CHAR);
}

void uart_setupTriggerPin() {
  hal_gpio_initializeInterrupt1Pin();
  hal_interrupt_onInterrupt1 = _onEnableRequest;
  hal_interrupt_enableInterrupt1(Interrupt_Interrupt1Sense_FallingEdge);
}

void uart_run() {
  if (!_isInitialized && _isEnableRequested) {
    uart_initialize();
    _isEnableRequested = FALSE;
  }
  
  if (_isInitialized && _isDisableRequested) {
    if(buffer_isEmpty(&txBuffer)) {
      hal_uart_disable();
      _isDisableRequested = FALSE;
      _isInitialized = FALSE;
    }
  }
  
  if(!_isInitialized) return;
  
  // This section will be executed only if UART is initialized
  if(timer_getTickCount() - lastDisplayTickCount > DISPLAY_INTERVAL) {
    uart_sendString("Distance: ", 10);
    uart_sendNumber(ultrasonic_getDistanceInMillimeter());
    uart_sendString(" mm.", 4);
    uart_sendLineBreak();
    
    lastDisplayTickCount = timer_getTickCount();
  }
  // Send the distance at regular interval
  
  
  if(isTransmitPending) {
    _transmit();
    isTransmitPending = FALSE;
  }
  
  if(isCommandPending) {
    _processCommand();
    isCommandPending = FALSE;
  }
}

void uart_initialize() {
  hal_uart_onTransmitComplete = _transmit;
  hal_uart_onReceiveComplete = _onByteReceived;
  hal_uart_initialize();
  _isInitialized = TRUE;
}