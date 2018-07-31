/*
* buffer.h
*
* Created: 7/25/2018 9:00:26 AM
*  Author: abdullah
*/

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdint.h>

#define DEFINE_BUFFER(name, size) do {\
char name ## _data[size];\
Buffer name = {name ## _data, size, 0, 0};\
} while(0)

#define DEFINE_GLOBAL_BUFFER(name, size) \
char name ## _data[size];\
Buffer name = {name ## _data, size, 0, 0}

//
// About the buffers:
//
// The readPointer will point to the byte to be sent and the writePointer points to the position where value to be written.
// When they point to the same location, then nothing to be sent. If something is written by the application then the writePointer will move and
// readPointer will be used to read from the buffer from its current location and by advancing by one until it reaches the writePointer.
//

typedef struct {
  char* data;
  const uint8_t bufferSize;
  uint8_t readPointer;
  uint8_t writePointer;
} Buffer;

uint8_t buffer_isEmpty(Buffer* buffer);
uint8_t buffer_takeCharFromBuffer(Buffer* buffer, uint8_t* data);
void buffer_putCharToBuffer(Buffer* buffer, uint8_t data);
void buffer_putStringToBuffer(Buffer* buffer, const char* data, uint8_t len);

#endif /* _BUFFER_H_ */