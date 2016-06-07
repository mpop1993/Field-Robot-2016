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

// ----- Defines
#define F_CPU 84000000L
#define _BV(bit) (0x1u << bit)
#define CHAN_ST 4
#define CHAN_DR 5

// ----- Function prototipes
void InitPWMController_MCLK(void);
static void SetPeriod(uint32_t chan, int period);
static void SetDuty(uint32_t chan, int duty);
void WriteMotors(int percent_ST, int percent_DR);
void Init_Motors();

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

void Init_Motors()
{
	WriteMotors(0,0);
	// Enable output
	PIOC->PIO_SODR = PIO_PC23; // Arduino Due Pin 25
	delay_ms(1000);
	
	WriteMotors(100,100);//Stop Motors; Percent_RIGHT|Percent_LEFT
	// Disable output
	PIOC->PIO_CODR = PIO_PC23; // Arduino Due Pin 25
	delay_ms(1000);
	
	WriteMotors(-100,-100);
	// Enable output
	PIOC->PIO_SODR = PIO_PC23; // Arduino Due Pin 25
	delay_ms(1000);
	
	WriteMotors(0,0);//Stop Motors; Percent_RIGHT|Percent_LEFT
	// Disable output
	PIOC->PIO_CODR = PIO_PC23; // Arduino Due Pin 25
	delay_ms(3000);
}  

void goForward(){
	
	int delta = 0;
	char decizion = 0;
	
	delta = iEncoder_DR-iEncoder_ST;
	
	if(delta<0){
		// viram stanga
		decizion = 's';
		iSpeed_DR =
		
		}else if(delta>0){
		//viram dreapta
		decizion = 'd';
		}else{
		//mergem in fata
		decizion = 'o';
	}
	
	uart_putchar(decizion);
}