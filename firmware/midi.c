#include <string.h>

#include "midi.h"
#include "timer.h"
#include "cv.h"

static unsigned char cmd;
static unsigned char chan;
static unsigned char seq;
static unsigned char args[2];
static unsigned char in_sysex;

static void note_off(unsigned char chan, unsigned char note, unsigned char velocity)
{
	/* empty */
}

static void note_on(unsigned char chan, unsigned char note, unsigned char velocity)
{
	/* empty */
}

static void poly_pressure(unsigned char chan, unsigned char note, unsigned char pressure)
{
	/* empty */
}

static void control_change(unsigned char chan, unsigned char controller, unsigned char value)
{
	if(chan == CHANNEL) {
		switch(controller) {
			case 32:
				CV_set(CV_CH0_VOLUME, value << 1);
				break;
			case 33:
				CV_set(CV_CH1_VOLUME, value << 1);
				break;
			case 34:
				CV_set(CV_CH2_VOLUME, value << 1);
				break;
			case 35:
				CV_set(CV_CH3_VOLUME, value << 1);
				break;
			case 36:
				CV_set(CV_CH0_PAN, value << 1);
				break;
			case 37:
				CV_set(CV_CH1_PAN, value << 1);
				break;
			case 38:
				CV_set(CV_CH2_PAN, value << 1);
				break;
			case 39:
				CV_set(CV_CH3_PAN, value << 1);
				break;
			case 40:
				CV_set(CV_CH0_VOLUME_L, value << 1);
				break;
			case 41:
				CV_set(CV_CH0_VOLUME_R, value << 1);
				break;
			case 42:
				CV_set(CV_CH1_VOLUME_L, value << 1);
				break;
			case 43:
				CV_set(CV_CH1_VOLUME_R, value << 1);
				break;
			case 44:
				CV_set(CV_CH2_VOLUME_L, value << 1);
				break;
			case 45:
				CV_set(CV_CH2_VOLUME_R, value << 1);
				break;
			case 46:
				CV_set(CV_CH3_VOLUME_L, value << 1);
				break;
			case 47:
				CV_set(CV_CH3_VOLUME_R, value << 1);
				break;
		}
	}
}

static void program_change(unsigned char chan, unsigned char program)
{
	if(chan == CHANNEL) {
		switch(program) {
			case 0: /* four mono channels, maximum volume */
				CV_set(CV_CH0_VOLUME_L, 255);
				CV_set(CV_CH0_VOLUME_R, 255);
				CV_set(CV_CH1_VOLUME_L, 255);
				CV_set(CV_CH1_VOLUME_R, 255);
				CV_set(CV_CH2_VOLUME_L, 255);
				CV_set(CV_CH2_VOLUME_R, 255);
				CV_set(CV_CH3_VOLUME_L, 255);
				CV_set(CV_CH3_VOLUME_R, 255);
				break;
			case 1: /* four mono channels, center panned */
				CV_set(CV_CH0_PAN, 128);
				CV_set(CV_CH0_VOLUME, 255);
				CV_set(CV_CH1_PAN, 128);
				CV_set(CV_CH1_VOLUME, 255);
				CV_set(CV_CH2_PAN, 128);
				CV_set(CV_CH2_VOLUME, 255);
				CV_set(CV_CH3_PAN, 128);
				CV_set(CV_CH3_VOLUME, 255);
				break;
			case 2: /* two stereo channels */
				CV_set(CV_CH0_PAN, 0);
				CV_set(CV_CH0_VOLUME, 255);
				CV_set(CV_CH1_PAN, 255);
				CV_set(CV_CH1_VOLUME, 255);
				CV_set(CV_CH2_PAN, 0);
				CV_set(CV_CH2_VOLUME, 255);
				CV_set(CV_CH3_PAN, 255);
				CV_set(CV_CH3_VOLUME, 255);
				break;
		}
	}
}

static void channel_pressure(unsigned char chan, unsigned char program)
{
	/* empty */
}

static void pitch_bend(unsigned char chan, unsigned char lsb, unsigned char msb)
{
	/* empty */
}

void MIDI_init(void)
{
	cmd = 0;
	in_sysex = 0;
}

void MIDI_process(unsigned char c)
{
	if(c == 0xF0) {
		in_sysex = 1;
		return;
	} else if(c == 0xF7) {
		in_sysex = 0;
		return;
		return;
	} else if(c >= 0xF8 && c <= 0xFF) {
		/* ignore RealTime messages */
		return;
	}

	/* ignore sysex messages for now */
	if(in_sysex) {
		return;
	}

	if(c >= 0x80) {
		cmd = c & 0xF0;
		chan = c & 0x0F;
		seq = 0;
	} else {
		switch(cmd) {
			case NOTE_OFF:
				if(seq < 2)
					args[seq++] = c;
				if(seq == 2) {
					note_off(chan, args[0], args[1]);
					seq = 0;
				}
				break;
			case NOTE_ON:
				if(seq < 2)
					args[seq++] = c;
				if(seq == 2) {
					if(args[1] == 0) {
						/* VELOCITY=0 means NOTE_OFF */
						note_off(chan, args[0], 0);
					} else {
						note_on(chan, args[0], args[1]);
					}
					seq = 0;
				}
				break;
			case POLY_PRESSURE:
				if(seq < 2)
					args[seq++] = c;
				if(seq == 2) {
					poly_pressure(chan, args[0], args[1]);
					seq = 0;
				}
				break;
			case CTRL_CHANGE:
				if(seq < 2)
					args[seq++] = c;
				if(seq == 2) {
					control_change(chan, args[0], args[1]);
					seq = 0;
				}
				break;
			case PROG_CHANGE:
				program_change(chan, c);
				seq = 0;
				break;
			case CHAN_PRESSURE:
				channel_pressure(chan, c);
				seq = 0;
				break;
			case PITCH_BEND:
				if(seq < 2)
					args[seq++] = c;
				if(seq == 2) {
					pitch_bend(chan, args[0], args[1]);
					seq = 0;
				}
				break;
		}
	}
}
