/*
 * UART.c
 *
 * Created: 5/19/2016 5:50:16 PM
 *  Author: Mihai
 */ 

// *************************************************************************************************************************************
// ----- Includes
#include "sam.h"
#include "UART.h"
#include "TmrCfg.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ----- Local variables

char buffer[20];
// ----- Function prototipes


// *************************************************************************************************************************************

inline int uart_getchar(uint8_t *c)
{
	// Check if the receiver is ready
	if((UART->UART_SR & UART_SR_RXRDY) == 0)
	return 1;
	
	// Read the character
	*c = (uint8_t) UART->UART_RHR;
	return 0;
}

inline int uart_putchar(const uint8_t c)
{
	// Check if the transmitter is ready
	if(!(UART->UART_SR & UART_SR_TXRDY))
	return 1;
	
	// Send the character
	UART->UART_THR = c;
	return 0;
}

void UART_Handler(void)
{
	uint8_t c = 0;
	int i = 0;
	memset(buffer, 0, sizeof(buffer));
	
	// Check if the interrupt source is receive ready
	if(UART->UART_IMR & UART_IMR_RXRDY)
	{
		while(1)
		{
			while(!uart_getchar(&c)){
				buffer[i++]=c;
			}
			if(c == '\n' || (i >sizeof(buffer)-3))
				break;
		}
		buffer[i++] = '\r';
		buffer[i++] = '\n';
		sendString("Received: ", 10);
		sendString(buffer, i);
		parseSpeed(buffer);
		
	}
}

void configure_uart(void)
{
	uint32_t ul_sr;
	
	// ==> Pin configuration
	// Disable interrupts on Rx and Tx
	PIOA->PIO_IDR |= PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// Disable the PIO of the Rx and Tx pins so that the peripheral controller can use them
	PIOA->PIO_PDR |= PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// Read current peripheral AB select register and set the Rx and Tx pins to 0 (Peripheral A function)
	ul_sr = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR &= ~(PIO_PA8A_URXD | PIO_PA9A_UTXD) & ul_sr;
	
	// Enable the pull up on the Rx and Tx pin
	PIOA->PIO_PUER = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// ==> Actual uart configuration
	// Enable the peripheral uart controller
	PMC->PMC_PCER0 |= 1 << ID_UART;
	
	// Reset and disable receiver & transmitter
	UART->UART_CR |= UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
	
	// Set the baudrate to 115200
	UART->UART_BRGR |= 45; // 84000000 / 16 * x = BaudRate (write x into UART_BRGR)
	
	// No Parity
	UART->UART_MR |= UART_MR_PAR_NO;
	
	// Disable PDC channel requests
	UART->UART_PTCR |= UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	
	// Disable / Enable interrupts on end of receive
	UART->UART_IDR = 0xFFFFFFFF;
	NVIC_EnableIRQ((IRQn_Type) ID_UART);
	UART->UART_IER = UART_IER_RXRDY;
	
	// Enable receiver and trasmitter
	UART->UART_CR |= UART_CR_RXEN | UART_CR_TXEN;
	
}


void sendString(const char* c, uint16_t length){
	for(int i = 0;i<length;i++){
		while(uart_putchar(*(c+i)));
	}
}

void printInt(int value, char* buffer)
{
	int i = 8;
	buffer[i] = '0';
	while(i>0)
	{
		buffer[i] = '0';
		buffer[i] = value%10 + '0';
		value /= 10;
		i--;
	}
}

void parseSpeed(char* buffer, int len)
{
	char* token1;
	token1 = strtok(buffer, "#");
	if(token1 != NULL)
	{
		char* token2;
		token2 = strtok(NULL, "#");
		if(token2 != NULL)
		{
			sendString("Speed1: ", 8);
			sendString(token1, strlen(token1));
			sendString(" Speed2: ", 8);
			sendString(token2, strlen(token2));
			
			char *end;
			int speed1 = strtol(token1, &end, 10);
			int speed2 = strtol(token2, &end, 10);
			WriteMotors(speed1, speed2);
			//char parsed[2];
			//parsed[0]=speed1;
			//parsed[1]=speed2;
			//sendString(parsed, 2);
		}
	}
}