/*
 * util.h
 *
 * Created: 7/19/2018 6:48:46 PM
 *  Author: abdullah
 */ 
#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>

#include "common.h"

uint8_t stringCompare(const char* str1, const char* str2, uint8_t len);
uint8_t numberToString(uint16_t number, char* string);
uint16_t stringToNumber(const char* string);

#endif // _UTIL_H_
