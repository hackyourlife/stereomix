#ifndef __LED_H__
#define __LED_H__

#define	LED_STATUS_CH0	_BV(0)
#define	LED_STATUS_CH1	_BV(1)
#define	LED_STATUS_CH2	_BV(2)
#define	LED_STATUS_CH3	_BV(3)

void	LED_init(void);
void	LED_on(void);
void	LED_reset(void);

void	LED_setLevelL(u8 level);
void	LED_setLevelR(u8 level);

u8	LED_getChannelStatus(void);
void	LED_setChannelStatus(u8 status);

void	LED_update(void);

#endif
