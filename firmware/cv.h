#ifndef __CV_H__
#define __CV_H__

#define	CV_CH0_VOLUME	0
#define	CV_CH1_VOLUME	1
#define	CV_CH2_VOLUME	2
#define	CV_CH3_VOLUME	3
#define	CV_CH0_PAN	4
#define	CV_CH1_PAN	5
#define	CV_CH2_PAN	6
#define	CV_CH3_PAN	7
#define	CV_CH0_VOLUME_L	8
#define	CV_CH0_VOLUME_R	9
#define	CV_CH1_VOLUME_L	10
#define	CV_CH1_VOLUME_R	11
#define	CV_CH2_VOLUME_L	12
#define	CV_CH2_VOLUME_R	13
#define	CV_CH3_VOLUME_L	14
#define	CV_CH3_VOLUME_R	15

void CV_init(void);
void CV_process(void);

void CV_set(unsigned char cv, unsigned char value);

#endif
