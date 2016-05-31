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
	//Configure_UART();
	
	
	/* Disable watchdog */
	WDT->WDT_MR |=  WDT_MR_WDDIS;
	
	/* Run initialization sequence for motor drivers */
	Init_Motors();

	//selfTest();
	
	iEncoder_DR = 0;
	iEncoder_ST = 0;
	WriteMotors(30,30);
	
    while (1) 
    {
		
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


