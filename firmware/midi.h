#ifndef __MIDI_H__
#define __MIDI_H__

#define	NOTE_OFF	0x80
#define	NOTE_ON		0x90
#define	POLY_PRESSURE	0xA0
#define	CTRL_CHANGE	0xB0
#define	PROG_CHANGE	0xC0
#define	CHAN_PRESSURE	0xD0
#define	PITCH_BEND	0xE0

/* CC names */
#define	CC_MODULATION	1
#define	CC_BREATH	2
#define	CC_FOOT		4
#define	CC_PORTAMENTO	5
#define	CC_VOLUME	7
#define	CC_BALANCE	8
#define	CC_PAN		10
#define	CC_EXPRESSION	11
#define	CC_DAMPER	64
#define	CC_PORT_EN	65
#define	CC_SOSTENUTO	66
#define	CC_SOFT		67
#define	CC_LEGATO	68

#define	CC_ALL_SND_OFF	120
#define	CC_RESET_CTRL	121
#define	CC_LOCAL	122
#define	CC_ALL_NOTE_OFF	123
#define	CC_OMNI_OFF	124
#define	CC_OMNI_ON	125
#define	CC_MONO_MODE	126
#define	CC_POLY_MODE	127

#define	CHANNEL		15

void MIDI_init(void);
void MIDI_process(unsigned char c);

#endif
