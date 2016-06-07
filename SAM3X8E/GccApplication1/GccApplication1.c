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
	WriteMotors(50,50);
	uint8_t speed = 50;
	
    while (1) 
    {
		if(iEncoder_DR > 250){
			WriteMotors(0,0);
			iEncoder_DR = 0;
			delay_ms(1000);
			speed = speed + 5;
			WriteMotors(speed,speed);
		}
		uart_putchar(48);
		
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


