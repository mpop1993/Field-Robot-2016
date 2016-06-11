/*
 * MotorCtrl.c
 *
 * Created: 5/15/2016 2:28:06 PM
 *  Author: Mihai
 */ 

// *************************************************************************************************************************************
// ----- Includes
#include "sam.h"
#include "TmrCfg.h"
#include "MotorCtrl.h"
#include "UART.h"
#include "global_variables.h"

// ----- Defines
#define F_CPU 84000000L
#define _BV(bit) (0x1u << bit)
#define CHAN_ST 4
#define CHAN_DR 5
volatile uint8_t flag12;

// ----- Function prototipes
void InitPWMController_MCLK(void);
static void SetPeriod(uint32_t chan, int period);
static void SetDuty(uint32_t chan, int duty);
void WriteMotors(int percent_ST, int percent_DR);
void InitMotors();
void ForwardDrive(void);
void ControlledDrive(uint32_t percentage_ST, uint32_t percentage_DR);
void signum(int x);

// *************************************************************************************************************************************

void InitPWMController_MCLK()
{
	//Enable the PWM clock (36) 
	PMC->PMC_PCER1 |= _BV((ID_PWM - 32)); // PMC_PCER1 = Peripheral Clock Enable Register 1
	
	//Channel Prescaler - Use ClockA as configured in PWM_CLK
	//Preliminary frequency = 84000000 / 1024 = 82031 Hz
	PWM->PWM_CH_NUM[CHAN_ST].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024; // PWM_CHR = PWM Channel Mode Register
	PWM->PWM_CH_NUM[CHAN_DR].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024; // PWM_CHR = PWM Channel Mode Register
	
	//Final frequency = 82031 / 1600 = 51 Hz
	SetPeriod(CHAN_ST, 1640);
	//Duty cycle = (200 / 400) * 100 = 50%
	SetDuty(CHAN_ST, 100);
	
	//Final frequency = 82031 / 1600 = 51Hz
	SetPeriod(CHAN_DR, 1640);
	//Duty cycle = (x / 800) * 100 = y%
	SetDuty(CHAN_DR, 100);
	
	PWM->PWM_ENA = _BV(CHAN_DR); // PWM_ENA = PWM Enable Register
	PWM->PWM_ENA = _BV(CHAN_ST); // PWM_ENA = PWM Enable Register
}

static void SetPeriod(uint32_t chan, int period)
{
	//If the channel is disabled then we can write directly to the register
	//	else if enabled we write to the update register which acts as a double buffer
	if ((PWM->PWM_SR & _BV(chan)) == 0)
		PWM->PWM_CH_NUM[chan].PWM_CPRD = period; // PWM_CPRD = PWM Channel Period Register
	else
		PWM->PWM_CH_NUM[chan].PWM_CPRDUPD = period; // PWM_CPRDUPD = PWM Channel Period Update Register
}

static void SetDuty(uint32_t chan, int duty)
{
	//If the channel is disabled then we can write directly to the register
	//	else if enabled we write to the update register which acts as a double buffer
	if ((PWM->PWM_SR & _BV(chan)) == 0)
		PWM->PWM_CH_NUM[chan].PWM_CDTY = duty;
	else
		PWM->PWM_CH_NUM[chan].PWM_CDTYUPD = duty;
}

void WriteMotors(int percent_ST, int percent_DR)
{
	SetDuty(CHAN_ST,123+(percent_ST/2.43)); // PWM motor stanga
	SetDuty(CHAN_DR,123-(percent_DR/2.43)); // PWM motor dreapta
}

void InitMotors()
{
	WriteMotors(0,0);
	// Enable output
	PIOC->PIO_SODR = PIO_PC23; 
	delay_ms(1000);
	
	WriteMotors(100,100);//Stop Motors; Percent_RIGHT|Percent_LEFT
	// Disable output
	PIOC->PIO_CODR = PIO_PC23;
	delay_ms(1000);
	
	WriteMotors(-100,-100);
	// Enable output
	PIOC->PIO_SODR = PIO_PC23;
	delay_ms(1000);
	
	WriteMotors(0,0);//Stop Motors; Percent_RIGHT|Percent_LEFT
	// Disable output
	PIOC->PIO_CODR = PIO_PC23;
	delay_ms(3000);
}  

void ForwardDrive(){
	WriteMotors(BASE_SPEED,BASE_SPEED);
	int delta = 0;
	char decizion = 0;
	
	delta = iEncoder_DR-iEncoder_ST;
	
		if(delta<0){
			// viram stanga
			decizion = 's';
			iSpeed_DR = BASE_SPEED + 15;
			iSpeed_ST = BASE_SPEED - 15;
		}
		else if(delta>0){
			//viram dreapta
			decizion = 'd';
			iSpeed_DR = BASE_SPEED - 15;
			iSpeed_ST = BASE_SPEED + 15;
		}
		else{
			//mergem in fata
			decizion = 'o';
			iSpeed_DR = BASE_SPEED;
			iSpeed_ST = BASE_SPEED;
	}
	
	uart_putchar(decizion);
	uart_putchar(10);
	
	WriteMotors(iSpeed_ST,iSpeed_DR);
}

// 255 impulsuri ~ 60 cm 
// 
void ControlledDrive(uint32_t percent_ST, uint32_t percent_DR){
	
	if(sign_ST==1){
		iSpeed_ST=-80;
	}else{
		iSpeed_ST=80;
	}
	
	if(sign_DR==1){
		iSpeed_DR=-80;
	}else{
		iSpeed_DR=80;
	}
	
	uint8_t st = 0;
	uint8_t dr = 0;
	
	iEncoder_ST_current = 0;
	iEncoder_DR_current = 0;

	int timeout = 0;
	
	while((!(st && dr)) && (timeout < 100000))
	{
		if(iEncoder_DR_current >= percent_DR){
			iSpeed_DR = 0;
			st=1;
		}
		if(iEncoder_ST_current >= percent_ST){
			iSpeed_ST = 0;
			dr=1;
		}
		WriteMotors(iSpeed_ST,iSpeed_DR);
		timeout++;
	}
	WriteMotors(40,40);	 
	sendString("\n\n---------- Exit from controlledDrive",38);	
}

void ControlledDrive_Speed(uint32_t percent_ST, uint32_t percent_DR){
	if(sign_ST==1){
		iSpeed_ST=-70;
		}else{
		iSpeed_ST=BASE_SPEED;
	}
	
	if(sign_DR==1){
		iSpeed_DR=-70;
		}else{
		iSpeed_DR=BASE_SPEED;
	}
	
	uint8_t st = 0;
	uint8_t dr = 0;
	
	uint32_t iEncoder_DR_previous = 0;
	uint32_t iEncoder_ST_previous = 0;
	
	uint16_t iDelta_ST = 0;
	uint16_t iDelta_DR = 0;
	
	iEncoder_ST_current = 0;
	iEncoder_DR_current = 0;
	
	int timeout = 0;
	
	while((!(st && dr)) && (timeout < 100000))
	{
		iDelta_ST = iEncoder_ST_current - iEncoder_ST_previous;
		iDelta_DR = iEncoder_DR_current - iEncoder_DR_previous;
		
		if(iEncoder_DR_current >= percent_DR){
			iSpeed_DR = 0;
			st=1;
		}
		if(iEncoder_ST_current >= percent_ST){
			iSpeed_ST = 0;
			dr=1;
		}
		
		
		iEncoder_ST_previous = iEncoder_ST_current;
		iEncoder_DR_previous = iEncoder_DR_current;
		
		
		WriteMotors(iSpeed_ST,iSpeed_DR);
		timeout++;
	}
	WriteMotors(0,0);
	sendString("\n\n---------- Exit from controlledDrive",38);
}

