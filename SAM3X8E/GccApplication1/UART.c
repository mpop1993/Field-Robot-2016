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
#include "MotorCtrl.h"
#include "global_variables.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 
// ----- Local variables
static volatile char buffer[70];

volatile uint32_t percentage_ST;
volatile uint32_t percentage_DR;
volatile uint8_t sign_ST;
volatile uint8_t sign_DR;
volatile uint8_t newSpeed;
extern volatile uint8_t flag12;

// ----- Function prototipes
uint8_t parseSpeed(char* buffer, int size);
int signum(int x);

// *************************************************************************************************************************************

inline int uart_getchar(uint8_t *c)
{
	*c = 0; 
	// Check if the receiver is ready
	for(int j = 0; j < 100000; j++)
	{
		if(UART->UART_SR & UART_SR_RXRDY)
		{
			// Read the character
			*c = (uint8_t) UART->UART_RHR;
			return 0;
		}
	}
	return 1;
}

inline int uart_putchar(const uint8_t c)
{
	// Check if the transmitter is ready
	for(int j = 0; j < 100000; j++)
	{
		if(UART->UART_SR & UART_SR_TXRDY)
		{
			UART->UART_THR = c;
			return 0;
		}
	}

	return 1;
}

void UART_Handler(void)
{	
	if(UART->UART_IMR & UART_IMR_RXRDY)
	{	   
		static int i = 0;
		uint8_t c;
		
		if(uart_getchar(&c))
		{
			return;
		}

		if(i > sizeof(buffer)-1)
		{
			sendString("####Too much data received\n", 27);
			memset(buffer, 0, sizeof(buffer));
			i = 0;
			return;
		}
		
		buffer[i] = c;
		
		if(buffer[i] == '\n')
		{
			//sendString("#### Parsing Strings: ", 22);
			sendString(buffer, i); // make an echo of the whole buffer untill now
			uart_putchar('\n');
			parseSpeed(buffer, i);	
			memset(buffer, 0, sizeof(buffer));
			i = 0;
		}
		else
		{
			i++;	
		}
	}
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

uint8_t getNewSpeed()
{
	if(flag12)
	{
		sendString("Set:\n", 5);
		return 1;
	}
	else
	{
		//sendString("UnSet:\n", 7);
		return 0;
	}
}

uint8_t parseSpeed(char* buffer, int size)
{
	//if(getNewSpeed())
	//{
		//sendString("-----Prescription not finished",30);
		//return 0;
	//}
	
	sign_ST = 0;
	sign_DR = 0;
	char bufParsing[70]="";
	memcpy(bufParsing, buffer+1, size-1);
	char* token1;
	token1 = strtok(bufParsing, "#");
	if(token1 != NULL)
	{
		char* token2;
		token2 = strtok(NULL, "#");
		
		if(token2 != NULL)
		{
			char *end;
			percentage_ST = strtol((token1+1), &end, 10);
			percentage_DR = strtol((token2+1), &end, 10);
			
			sendString("Speed ST: ", 10);
			sendString(token1, strlen(token1));

			if (token1[0] == 0x2d) {
				// minus
				sign_ST = 1;
			}
			
			sendString(" Speed DR: ", 11);
			sendString(token2, strlen(token2));
			
			if (token2[0] == 0x2d) {
				 // minus
				 sign_DR = 1;
			}
			
			newSpeed = 1;
			flag12=1;
		}
	}
	
	return 1;
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

int signum(int x){
	if (x > 0) return 1;
	else return -1;
}

