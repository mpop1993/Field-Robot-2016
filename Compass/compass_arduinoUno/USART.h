#include <avr/io.h>
#include "avr/interrupt.h"
#include "pin_control.h"
#include "event.h"
#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void UsartWrite( char *data,  char length)
{
	unsigned char i = 0;
	
	while (length > 0)
	{
		if (UCSR0A & (1<<UDRE0))
		{
			UDR0 = data[i++];
			length--;
		}
	}
}
void USART_init(void)
{
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B|= (1<<RXEN0);//enable Rx
	UCSR0B|=(1<<TXEN0);//enable  TX
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable reception and RC complete interrupt
	UCSR0B |= (1<<RXEN0)|(1<<RXCIE0);
	//  Enable global interrupts
	sei();
}

unsigned char USART_receive(void)
{
	
	while(!(UCSR0A & (1<<RXC0)));//wait while rx enable
	return UDR0;
	
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));//wait while buffer is empty
	UDR0 = data;
	
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
	
}
ISR(USART_RX_vect)
{
	int8_t data = UDR0;
	event_handler_rcv(data);
	//set_pin(PORTB,PORTB4);
	 	
}
