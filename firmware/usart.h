#ifndef	__USART_H__
#define	__USART_H__

#define	WRITEBUFFERSIZE		6
#define	READBUFFERSIZE		6

#define	BAUD_MACRO

unsigned char USART_available();

#ifdef BAUD_MACRO
#define	USART_getBaud(baud)	(F_CPU / 8 / (baud) - 1)
#else
unsigned short USART_getBaud(unsigned long baud);
#endif

#define	USART_5N1		0x00
#define	USART_6N1		0x02
#define	USART_7N1		0x04
#define	USART_8N1		0x06
#define	USART_5N2		0x08
#define	USART_6N2		0x0A
#define	USART_7N2		0x0C
#define	USART_8N2		0x0E
#define	USART_5E1		0x20
#define	USART_6E1		0x22
#define	USART_7E1		0x24
#define	USART_8E1		0x26
#define	USART_5E2		0x28
#define	USART_6E2		0x2A
#define	USART_7E2		0x2C
#define	USART_8E2		0x2E
#define	USART_5O1		0x30
#define	USART_6O1		0x32
#define	USART_7O1		0x34
#define	USART_8O1		0x36
#define	USART_5O2		0x38
#define	USART_6O2		0x3A
#define	USART_7O2		0x3C
#define	USART_8O2		0x3E

void USART_init(unsigned short baud, unsigned char config);

unsigned char USART_free(void);
unsigned char USART_receive(void);
unsigned char USART_canRead(void);
unsigned char USART_read(unsigned char* buf, unsigned char length);
unsigned char USART_readByte(unsigned char* b);
unsigned char USART_send(const unsigned char* data, unsigned char length);

#endif
