#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

#if !defined(USART0_TX_vect) && defined(USART_TX_vect)
#define	USART0_TX_vect		USART_TX_vect
#endif
#if !defined(USART0_RX_vect) && defined(USART_RX_vect)
#define	USART0_RX_vect		USART_RX_vect
#endif
#if !defined(USART0_UDRE_vect) && defined(USART_UDRE_vect)
#define	USART0_UDRE_vect	USART_UDRE_vect
#endif

#define	RX_BUFFER_SIZE		(1 << READBUFFERSIZE)
#define	TX_BUFFER_SIZE		(1 << WRITEBUFFERSIZE)

#define	RX_BUFFER_MASK		(RX_BUFFER_SIZE - 1)
#define	TX_BUFFER_MASK		(TX_BUFFER_SIZE - 1)

static unsigned char tx_read;
static unsigned char tx_write;
static unsigned char rx_read;
static unsigned char rx_write;

static unsigned char txbuffer[TX_BUFFER_SIZE];
static unsigned char rxbuffer[RX_BUFFER_SIZE];

static unsigned char paused;

#ifndef BAUD_MACRO
unsigned short USART_getBaud(unsigned long baud)
{
	return F_CPU / 8 / baud - 1;
}
#endif

void USART_init(unsigned short baud, unsigned char config)
{
	UBRR0H = (unsigned char) (baud >> 8);
	UBRR0L = (unsigned char) baud;
	UCSR0A |= (1 << U2X0);
	UCSR0B |= ((1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0));
	UCSR0B &= ~((1 << UDRIE0) | (1 << TXCIE0) | (1 << UCSZ02));
	UCSR0A &= ~(1 << MPCM0); // SingleMaster
	UCSR0C = config;
	tx_read = tx_write = 0;
	rx_read = rx_write = 0;
	paused = 0;
}

unsigned char USART_available()
{
	return (RX_BUFFER_SIZE + rx_write - rx_read) & RX_BUFFER_MASK;
}

unsigned char USART_receive(void)
{
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

unsigned char USART_free(void)
{
	return rx_write >= rx_read ? RX_BUFFER_MASK - rx_write + rx_read - 1 :
		rx_read - rx_write - 1;
}

unsigned char USART_canRead(void)
{
	return rx_read != rx_write;
}

unsigned char USART_read(unsigned char* buf, unsigned char length)
{
	unsigned char i;
	for(i = 0; i < length; i++) {
		if(rx_read == rx_write)
			break;
		*buf++ = rxbuffer[rx_read++];
		rx_read &= RX_BUFFER_MASK;
	}
	return i;
}

unsigned char USART_readByte(unsigned char* b)
{
	if(rx_read == rx_write)
		return 0;
	*b = rxbuffer[rx_read++];
	rx_read &= RX_BUFFER_MASK;
	return 1;
}

unsigned char USART_send(const unsigned char* data, unsigned char length)
{
	unsigned char i;
	unsigned char sreg;
	sreg = SREG;
	cli();
	unsigned char freesize = tx_write >= tx_read ? TX_BUFFER_SIZE -
			(int)tx_write + (int)tx_read - 1 : tx_read -
			tx_write - 1;
	if(length > freesize) { // not enough free space
		SREG = sreg;
		return 0;
	}
	for(i = 0; i < length; i++) { // data -> transmit buffer
		txbuffer[tx_write++] = *data++;
		tx_write &= TX_BUFFER_MASK;
	}
	UCSR0B |= (1 << UDRIE0);
	SREG = sreg;
	return 1;
}

ISR(USART0_UDRE_vect)
{
	// buffer empty
	if(tx_read == tx_write) {
		UCSR0B &= ~(1 << UDRIE0);
	} else {
		UDR0 = txbuffer[tx_read++];
		tx_read &= TX_BUFFER_MASK;
	}
}

ISR(USART0_RX_vect)
{
	rxbuffer[rx_write++] = UDR0;
	rx_write &= RX_BUFFER_MASK;
}
