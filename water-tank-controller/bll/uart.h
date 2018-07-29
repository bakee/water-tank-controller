/*
 * uart.h
 *
 * Created: 7/18/2018 12:19:55 AM
 *  Author: abdullah
 */ 

#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

 void uart_initialize();
 void uart_run();
 void uart_sendChar(char c);
 void uart_sendString(const char* string, uint8_t length);
 void uart_sendNumber(uint16_t number);
 void uart_sendLineBreak();
 
 #endif // _UART_H_
