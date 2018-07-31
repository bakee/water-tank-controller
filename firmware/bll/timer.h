/*
 * timer.h
 *
 * Created: 7/19/2018 7:42:50 PM
 *  Author: abdullah
 */ 


#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#include "common.h"

void timer_initialize();
void timer_run();
uint16_t timer_getTickCount();


#endif /* _TIMER_H_ */
