#ifndef __TIMER_H__
#define __TIMER_H__

#include "types.h"

extern u8 counter;

void TIMER0_init(void);
void TIMER0_on(void);
void TIMER0_off(void);

void TIMER2_init(void);
void TIMER2_on(void);
void TIMER2_off(void);

#endif
