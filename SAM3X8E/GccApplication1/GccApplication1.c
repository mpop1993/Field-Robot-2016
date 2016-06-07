/*
 * GccApplication1.c
 *
 * Created: 5/14/2016 11:51:56 AM
 *  Author: Mihai
 */ 

// *************************************************************************************************************************************
// ----- Includes
#include "sam.h"
#include "InitCfg.h"
#include "MotorCtrl.h"
#include "TmrCfg.h"
#include "UART.h"
#include "global_variables.h"
#include "Encoders.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ----- Defines
#define F_CPU 84000000L

// ----- Prototipes
void selfTest(void);

// ----- Local variables
uint8_t c = 4;

// *************************************************************************************************************************************

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	/* Initialize pins */
	Pin_Configuration();
	/* Initialize PWM generator */
	InitPWMController_MCLK(); 
	/* Initialize timers */
	Configure_Timers();
	/* Configre UART */
	configure_uart();
	
	
	/* Disable watchdog */
	WDT->WDT_MR |=  WDT_MR_WDDIS;
	
	/* Run initialization sequence for motor drivers */
	Init_Motors();

	//selfTest();
	
	iEncoder_DR = 0;
	iEncoder_ST = 0;
	
	int delta = 0;
	char decizion = 0;
	
	iSpeed_DR = 70;
	iSpeed_ST = 70;
	char buffer[500];
	
    while (1) 
    {
		delta = iEncoder_DR-iEncoder_ST;
		if(delta<0){
			// viram dreapta
			decizion = 'd';
		}else if(delta>0){
			//viram stanga
			decizion = 's';
		}else{
			//mergem in fata
			decizion = 'o';
		}
		
		uart_putchar(decizion);
		uart_putchar(10);
		
		//memset(buffer,0,sizeof(buffer));
		//printInt(delta,buffer);
		//buffer[9]=10;
		//sendString(buffer,10);
		
		if(iEncoder_DR > 250){
			iSpeed_DR = 0;
			iEncoder_DR = 0;
		}
		if(iEncoder_ST > 250){
			iSpeed_ST = 0;
			iEncoder_ST = 0;
		}
		WriteMotors(iSpeed_ST,iSpeed_DR);
	}
}


void selfTest(void){
	
	// BUZZER
		// Enable output
		PIOD->PIO_SODR = PIO_PD7; // Arduino Due Pin 25
		delay_ms(1000);
		// Disable output
		PIOD->PIO_CODR = PIO_PD7; // Arduino Due Pin 25
		delay_ms(1000);
	
	// PUMP
		// Enable output
		PIOC->PIO_SODR = PIO_PC24; // Arduino Due Pin 25
		delay_ms(1000);
		// Disable output
		PIOC->PIO_CODR = PIO_PC24; // Arduino Due Pin 25
		delay_ms(1000);
		
	// MOTORS
		WriteMotors(100,100);
		delay_ms(1000);
		WriteMotors(-100,-100);
		delay_ms(1000);
		WriteMotors(0,0);
}


