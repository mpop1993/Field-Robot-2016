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
volatile uint8_t flag12=0;

// ----- Defines
#define F_CPU 84000000L
#define TASK_1

// ----- Prototipes
void selfTest(void);

// ----- Local variables
uint8_t c = 4;
volatile uint8_t startStop_Camera;
volatile uint8_t initializeMotors;

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
	
	sendString("###ON \n", 7);

	/* Disable watchdog */
	WDT->WDT_MR |=  WDT_MR_WDDIS;

	//selfTest();
	
	// Set variables 
	initializeMotors = 0;
	
	// ----- TASK_1
	#if defined(TASK_1)
		while (1)
		{	

			if(initializeMotors)
			{
				sendString("###Initializing\n", 16);
				InitMotors();
				initializeMotors = 0;
			}
			
			if(getNewSpeed())
			{
				sendString("###New Speed\n", 14);
				newSpeed = 0;
				ControlledDrive(percentage_ST,percentage_DR);
				flag12 = 0;
			}
		
			//ForwardDrive();
		}
		
	// ----- TASK_2
	#elif defined(TASK_2)
	
		while (1)
		{
			
		}
	
	// ----- TASK_3
	#elif defined(TASK_3)
		
		startStop_Camera = 0;
		
		while (1)
		{
			if(initializeMotors)
			{
				sendString("###Initializing\n", 16);
				InitMotors();
				initializeMotors = 0;
			}

			if(startStop_Camera)
			{ // Stop motors and ignore lidar values 
				WriteMotors(0,0);
			}
			else
			{ // Start and go based on lidar values
				
				if(getNewSpeed())
				{
					sendString("###New Speed\n", 14);
					newSpeed = 0;
					ControlledDrive(percentage_ST,percentage_DR);
					flag12 = 0;
				}
				
				//ForwardDrive();
			}
		}
	
	// ----- TASK_4
	#elif defined(TASK_4)
	
		while (1)
		{
			
		}	
	
	#endif

}


void selfTest(void){
	
	// BUZZER
		// Enable output
		PIOD->PIO_SODR = PIO_PD7;
		delay_ms(1000);
		// Disable output
		PIOD->PIO_CODR = PIO_PD7;
		delay_ms(1000);
	
	// PUMP
		// Enable output
		PIOC->PIO_SODR = PIO_PC24; 
		delay_ms(1000);
		// Disable output
		PIOC->PIO_CODR = PIO_PC24;
		delay_ms(1000);
		
	// MOTORS
		WriteMotors(50,50);
		delay_ms(1000);
		WriteMotors(-50,-50);
		delay_ms(1000);
		WriteMotors(0,0);
}


