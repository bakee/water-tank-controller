#ifndef _RELAY_H_
#define _RELAY_H_

void relay_initialize();
void relay_run();

void relay_turnOn();
void relay_turnOff();
uint8_t relay_isTurnedOn();

#endif /* _RELAY_H_ */