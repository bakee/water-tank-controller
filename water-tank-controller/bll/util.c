/*
* util.h
*
* Created: 7/19/2018 6:48:55 PM
*  Author: abdullah
*/

#include "util.h"

uint8_t stringCompare(const char* str1, const char* str2, uint8_t len) {
  uint8_t i;
  for(i = 0; i < len; ++i) {
    if(str1[i] != str2[i]) return FALSE;
  }

  return TRUE;
}

// Returns number of digits
uint8_t numberToString(uint16_t number, char* string) {
  const uint8_t numberBase = 10; // For decimal
  uint8_t i;
  char digits[5] = {0};
  
  for(i = 0; i < 5; ++i) {
    digits[i] = number % numberBase;
    number /= numberBase;
    if(0 == number) {
      break;
    }
  }
  
  const uint8_t actualDigitsCount = i + 1;
  
  for (i = 0; i < actualDigitsCount; ++i) {
    string[i] = '0' + digits[actualDigitsCount - i - 1];
  }
  
  return actualDigitsCount;
}

uint16_t stringToNumber(const char* string) {
  // Handle maximum 5 digits
  uint16_t number = 0, i;
  for (i = 0; i < 5; ++i) {
    uint8_t currentDigit = string[i];
    if(currentDigit < '0' || currentDigit > '9' ) {
      break;
    }
    
    number *= 10;
    number += (currentDigit - '0');
  }
  
  return number;
}