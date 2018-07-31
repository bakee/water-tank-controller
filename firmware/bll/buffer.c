/*
* buffer.c
*
* Created: 7/25/2018 9:00:16 AM
*  Author: abdullah
*/

#include "common.h"
#include "buffer.h"

void _incrementBufferPointer(uint8_t* bufferPointer, const uint8_t bufferSize) {
  (*bufferPointer)++;
  if(*bufferPointer >= bufferSize) {
    *bufferPointer -= bufferSize;
  }
}

void _incrementReadPointer(Buffer* buffer) {
  _incrementBufferPointer(&(buffer->readPointer), buffer->bufferSize);
}

void _incrementWritePointer(Buffer* buffer) {
  _incrementBufferPointer(&(buffer->writePointer), buffer->bufferSize);
}

uint8_t buffer_isEmpty(Buffer* buffer) {
  if (buffer->readPointer == buffer->writePointer) {
    return TRUE; // Empty buffer
  }
  
  return FALSE;
}

uint8_t buffer_takeCharFromBuffer(Buffer* buffer, uint8_t* data) {
  if (buffer_isEmpty(buffer)) {
    return FALSE;
  }
  
  *data = buffer->data[buffer->readPointer];
  _incrementReadPointer(buffer);
  return TRUE;
}

void buffer_putCharToBuffer(Buffer* buffer, uint8_t data) {
  buffer->data[buffer->writePointer] = data;
  _incrementWritePointer(buffer);
}

void buffer_putStringToBuffer(Buffer* buffer, const char* data, uint8_t len) {
  uint8_t i;
  for (i = 0; i < len; ++i) {
    buffer_putCharToBuffer(buffer, data[i]);
  }
}