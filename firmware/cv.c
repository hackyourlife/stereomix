#include <avr/io.h>
#include <avr/pgmspace.h>
#include "types.h"
#include "cv.h"
#include "tables.h"
#include "led.h"

static u8 volume_ch0;
static u8 volume_ch1;
static u8 volume_ch2;
static u8 volume_ch3;
static u8 pan_ch0;
static u8 pan_ch1;
static u8 pan_ch2;
static u8 pan_ch3;

static u8 volume_ch0_l;
static u8 volume_ch0_r;
static u8 volume_ch1_l;
static u8 volume_ch1_r;
static u8 volume_ch2_l;
static u8 volume_ch2_r;
static u8 volume_ch3_l;
static u8 volume_ch3_r;

static u8 sin4(u8 x)
{
	return (u8) pgm_read_byte(sin4table + x);
}

void CV_init(void)
{
	DDRC |= _BV(DDC0) | _BV(DDC1) | _BV(DDC2) \
		| _BV(DDC3);

	pan_ch0 = 128;
	pan_ch1 = 128;
	pan_ch2 = 128;
	pan_ch3 = 128;

	volume_ch0 = 255;
	volume_ch1 = 255;
	volume_ch2 = 255;
	volume_ch3 = 255;

	volume_ch0_l = 255;
	volume_ch0_r = 255;
	volume_ch1_l = 255;
	volume_ch1_r = 255;
	volume_ch2_l = 255;
	volume_ch2_r = 255;
	volume_ch3_l = 255;
	volume_ch3_r = 255;

	LED_setChannelStatus(0);
}

static u8 cv_step = 0;

/* Principles of Operation:
 *
 * step 0: disconnect MUX, load PWM with channel 0
 * step 1: connect MUX, since voltage is now stable
 * step 2: disconnect MUX, load PWM with channel 1
 * step 3: ...
 */
void CV_process(void)
{
	u8 mux = (cv_step >> 1) & 3;

	if(cv_step & 1) {
		/* connect */
		PORTC &= ~_BV(PORTC3);
	} else {
		/* disconnect */
		PORTC |= _BV(PORTC3);
	}

	PORTC = (PORTC & 0xF8) | mux;

	switch(mux) {
		case 0:
			OCR0B = volume_ch0_l;
			OCR0A = volume_ch0_r;
			break;
		case 1:
			OCR0B = volume_ch1_l;
			OCR0A = volume_ch1_r;
			break;
		case 2:
			OCR0B = volume_ch2_l;
			OCR0A = volume_ch2_r;
			break;
		case 3:
			OCR0B = volume_ch3_l;
			OCR0A = volume_ch3_r;
			break;
	}

	cv_step++;
}

static inline u8 get_pan_left(u8 volume, u8 pan)
{
	return ((u16) volume * (u16) sin4(~pan)) >> 8;
}

static inline u8 get_pan_right(u8 volume, u8 pan)
{
	return ((u16) volume * (u16) sin4(pan)) >> 8;
}

static inline void set_mute_status(u8 ch, u8 volume_l, u8 volume_r)
{
	u8 status = LED_getChannelStatus();
	u8 bit = 0;

	switch(ch) {
		case 0:
			bit = LED_STATUS_CH0;
			break;
		case 1:
			bit = LED_STATUS_CH1;
			break;
		case 2:
			bit = LED_STATUS_CH2;
			break;
		case 3:
			bit = LED_STATUS_CH3;
			break;
	}

	if(!volume_l && !volume_r) {
		status |= bit;
	} else {
		status &= ~bit;
	}

	LED_setChannelStatus(status);
}

void CV_set(unsigned char cv, unsigned char value)
{
	switch(cv) {
		case CV_CH0_VOLUME:
			volume_ch0 = value;
			volume_ch0_l = get_pan_left(volume_ch0, pan_ch0);
			volume_ch0_r = get_pan_right(volume_ch0, pan_ch0);
			set_mute_status(0, volume_ch0_l, volume_ch0_r);
			break;
		case CV_CH0_PAN:
			pan_ch0 = value;
			volume_ch0_l = get_pan_left(volume_ch0, pan_ch0);
			volume_ch0_r = get_pan_right(volume_ch0, pan_ch0);
			set_mute_status(0, volume_ch0_l, volume_ch0_r);
			break;
		case CV_CH1_VOLUME:
			volume_ch1 = value;
			volume_ch1_l = get_pan_left(volume_ch1, pan_ch1);
			volume_ch1_r = get_pan_right(volume_ch1, pan_ch1);
			set_mute_status(1, volume_ch1_l, volume_ch1_r);
			break;
		case CV_CH1_PAN:
			pan_ch1 = value;
			volume_ch1_l = get_pan_left(volume_ch1, pan_ch1);
			volume_ch1_r = get_pan_right(volume_ch1, pan_ch1);
			set_mute_status(1, volume_ch1_l, volume_ch1_r);
			break;
		case CV_CH2_VOLUME:
			volume_ch2 = value;
			volume_ch2_l = get_pan_left(volume_ch2, pan_ch2);
			volume_ch2_r = get_pan_right(volume_ch2, pan_ch2);
			set_mute_status(2, volume_ch2_l, volume_ch2_r);
			break;
		case CV_CH2_PAN:
			pan_ch2 = value;
			volume_ch2_l = get_pan_left(volume_ch2, pan_ch2);
			volume_ch2_r = get_pan_right(volume_ch2, pan_ch2);
			set_mute_status(2, volume_ch2_l, volume_ch2_r);
			break;
		case CV_CH3_VOLUME:
			volume_ch3 = value;
			volume_ch3_l = get_pan_left(volume_ch3, pan_ch3);
			volume_ch3_r = get_pan_right(volume_ch3, pan_ch3);
			set_mute_status(3, volume_ch3_l, volume_ch3_r);
			break;
		case CV_CH3_PAN:
			pan_ch3 = value;
			volume_ch3_l = get_pan_left(volume_ch3, pan_ch3);
			volume_ch3_r = get_pan_right(volume_ch3, pan_ch3);
			set_mute_status(3, volume_ch3_l, volume_ch3_r);
			break;
		case CV_CH0_VOLUME_L:
			volume_ch0_l = value;
			set_mute_status(0, volume_ch0_l, volume_ch0_r);
			break;
		case CV_CH0_VOLUME_R:
			volume_ch0_r = value;
			set_mute_status(0, volume_ch0_l, volume_ch0_r);
			break;
		case CV_CH1_VOLUME_L:
			volume_ch1_l = value;
			set_mute_status(1, volume_ch1_l, volume_ch1_r);
			break;
		case CV_CH1_VOLUME_R:
			volume_ch1_r = value;
			set_mute_status(1, volume_ch1_l, volume_ch1_r);
			break;
		case CV_CH2_VOLUME_L:
			volume_ch2_l = value;
			set_mute_status(2, volume_ch2_l, volume_ch2_r);
			break;
		case CV_CH2_VOLUME_R:
			volume_ch2_r = value;
			set_mute_status(2, volume_ch2_l, volume_ch2_r);
			break;
		case CV_CH3_VOLUME_L:
			volume_ch3_l = value;
			set_mute_status(3, volume_ch3_l, volume_ch3_r);
			break;
		case CV_CH3_VOLUME_R:
			volume_ch3_r = value;
			set_mute_status(3, volume_ch3_l, volume_ch3_r);
			break;
	}
}
