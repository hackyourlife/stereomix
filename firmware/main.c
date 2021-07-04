#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usart.h"
#include "midi.h"
#include "timer.h"
#include "cv.h"
#include "led.h"

#define	SET(port, pin)		port |= pin
#define	CLEAR(port, pin)	port &= ~(pin)

#define	LED_OFF(led)		CLEAR(PORTD, led)
#define	LED_ON(led)		SET(PORTD, led)

#define	LED_MIDI		_BV(PORTD7)

static void init_pins(void)
{
	DDRD |= _BV(DDD7);			/* MIDI LED */
	DDRD &= ~(_BV(DDD3) | _BV(DDD4));	/* CONFIG */
	DDRD &= ~_BV(DDD0);			/* UART RX */
}

static void init_power(void)
{
	PRR |= _BV(PRTWI)	/* disable TWI */
		| _BV(PRTIM1)	/* disable TIMER 1 */
		| _BV(PRSPI);	/* disable SPI */
	ACSR |= _BV(ACD);	/* disable analog comparator */
}

int main(void)
{
	unsigned char c;

	init_power();
	init_pins();
	LED_init();
	CV_init();
	LED_on();

	TIMER0_init();
	TIMER2_init();

	MIDI_init();

	LED_ON(LED_MIDI);

	USART_init(USART_getBaud(31250), USART_8N1);

	TIMER0_on();
	TIMER2_on();
	sei();

	_delay_ms(200);

	LED_OFF(LED_MIDI);

	LED_reset();

	while(1) {
		if(USART_readByte(&c)) {
			counter = 100;
			LED_ON(LED_MIDI);
			MIDI_process(c);
		}

		if(!counter) {
			LED_OFF(LED_MIDI);
		}
	}

	return 0;
}
