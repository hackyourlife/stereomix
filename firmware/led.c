#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "types.h"
#include "led.h"
#include "tables.h"

static u8 channel_status;
static u8 level_l;
static u8 level_r;

static u8 adc_channel;
static u8 adc_channel_cnt;

static u8 log64(u8 x)
{
	if(x >= 64) {
		return 255;
	} else {
		return pgm_read_byte(logtable + x);
	}
}

void LED_init(void)
{
	/* channel status LEDs */
	DDRB |= _BV(DDB0) | _BV(DDB1) | _BV(DDB2);
	DDRD |= _BV(DDD2);

	/* level indicator */
	DDRB |= _BV(DDB3) | _BV(DDB4) | _BV(DDB5);
	DDRC |= _BV(DDC4) | _BV(DDC5);

	/* configure ADC */
	ADMUX = _BV(REFS0) | _BV(REFS1)	/* AREF = 1.1V */
		| _BV(ADLAR)	/* ADC left adjust result */
		| 6;		/* MUX channel 6 */
	ADCSRB = 0; /* free running mode */
	ADCSRA = _BV(ADEN)	/* ADC enable */
		| _BV(ADATE)	/* ADC auto trigger enable */
		| _BV(ADIE)	/* ADC interrupt enable */
		| _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); /* PS=fclk/128 */

	adc_channel = 6;
	adc_channel_cnt = 0;
}

ISR(ADC_vect)
{
	u8 value = log64(ADCH);

	if(adc_channel_cnt >= 250) {
		adc_channel_cnt = 0;
		adc_channel ^= 1;
		ADMUX = (ADMUX & 0xF0) | adc_channel;
	} else {
		if(adc_channel_cnt == 0) {
			/* discard first conversion result */
			adc_channel_cnt++;
			return;
		} else {
			adc_channel_cnt++;
		}
	}


	if(adc_channel & 1)
		LED_setLevelR(value);
	else
		LED_setLevelL(value);
}

void LED_on(void)
{
	LED_setLevelL(255);
	LED_setLevelR(255);
	LED_setChannelStatus(0xFF);
}

void LED_reset(void)
{
	LED_setLevelL(0);
	LED_setLevelR(0);
	LED_setChannelStatus(0);

	/* start conversion */
	ADCSRA |= _BV(ADSC);
}

static u8 LED_getBits(u8 level)
{
	u8 quantized = level >> 5;
	u8 bits = 0;
	if(quantized > 0)
		bits |= _BV(0);
	if(quantized > 1)
		bits |= _BV(1);
	if(quantized > 2)
		bits |= _BV(2);
	if(quantized > 3)
		bits |= _BV(3);
	if(quantized > 4)
		bits |= _BV(4);
	if(quantized > 5)
		bits |= _BV(5);
	if(quantized > 6)
		bits |= _BV(6);
	if(level > 250)
		bits |= _BV(7);
	return bits;
}

void LED_setLevelL(u8 level)
{
	level_l = LED_getBits(level);
}

void LED_setLevelR(u8 level)
{
	level_r = LED_getBits(level);
}

u8 LED_getChannelStatus(void)
{
	return channel_status;
}

void LED_setChannelStatus(u8 status)
{
	channel_status = status;
	PORTB = (PORTB & 0xF8) | (status & 0x07);
	PORTD = (PORTD & ~_BV(PORTD2)) | ((status >> 1) & 0x04);
}

static u8 cnt = 0;
static u8 level_l_bits;
static u8 level_r_bits;

void LED_update(void)
{
	if(!(cnt & 0x07)) {
		level_l_bits = level_l;
		level_r_bits = level_r;
	}

	u8 level_bits = ((level_l_bits & 1) << 4)
		| ((level_r_bits & 1) << 5);

	PORTB = (PORTB & ~(_BV(PORTB3) | _BV(PORTB4) | _BV(PORTB5)))
		| ((cnt & 0x07) << 3);
	PORTC = (PORTC & ~(_BV(PORTC4) | _BV(PORTC5)))
		| level_bits;

	cnt++;
	level_l_bits >>= 1;
	level_r_bits >>= 1;
}
