#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "cv.h"
#include "led.h"

////////////////////////////////////////////////////////////////////////////////
// TIMER 0: PWM and system clock
////////////////////////////////////////////////////////////////////////////////

u8 counter = 0;

void TIMER0_init(void)
{
	// set direction of OC0A and OC0B to output
	DDRD |= _BV(DDD5) | _BV(DDD6);

	// fastpwm, prescaler = 1 (62.5kHz)
	TCCR0A = _BV(WGM00) | _BV(WGM01);
	TCCR0B = _BV(CS00);

	OCR0A = 0;
	OCR0B = 0;
}

void TIMER0_on(void)
{
	// PWM on OC0A and OC0B
	unsigned char tccr = TCCR0A;
	tccr &= ~(_BV(COM0A0) | _BV(COM0B0));
	tccr |= _BV(COM0A1) | _BV(COM0B1);
	TCCR0A = tccr;

	// enable OVF interrupt
	TIMSK0 |= _BV(TOIE0);
}

void TIMER0_off(void)
{
	// normal port operation (OC0A/OC0B disconnected)
	TCCR0A &= ~(_BV(COM0A0) | _BV(COM0A1) | \
			_BV(COM0B0) | _BV(COM0B1));

	// disable OVF interrupt
	TIMSK0 &= ~_BV(TOIE0);
}

ISR(TIMER0_OVF_vect)
{
	if(counter)
		counter--;
}

////////////////////////////////////////////////////////////////////////////////
// TIMER 2: CV out processing
////////////////////////////////////////////////////////////////////////////////

void TIMER2_init(void)
{
	// fastpwm, prescaler = 32 (~2kHz)
	TCCR2A = _BV(WGM20) | _BV(WGM21);
	TCCR2B = _BV(CS20) | _BV(CS21);
}

void TIMER2_on(void)
{
	// enable OVF interrupt
	TIMSK2 |= _BV(TOIE2);
}

void TIMER2_off(void)
{
	// disable OVF interrupt
	TIMSK0 &= ~_BV(TOIE0);
}

ISR(TIMER2_OVF_vect)
{
	CV_process();
	LED_update();
}
