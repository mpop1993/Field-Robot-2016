/*
 * TmrCfg.c
 *
 * Created: 5/19/2016 11:34:36 AM
 *  Author: Mihai
 */ 

// *************************************************************************************************************************************
// ----- Includes
#include "sam.h"
#include "TmrCfg.h"

// ----- Local variables
uint8_t iDelay = 0;
uint8_t iFlag = 0;

// ----- Function prototipes
void Configure_Timers(void);
void Configure_TC0(void);
void delay_ms(uint32_t iPrescription);

// *************************************************************************************************************************************

void Configure_Timers(){
	Configure_TC0();
}

void Configure_TC0()
{
	// Enable TC0 in PMC power management controller - ID 27
	PMC->PMC_PCER0 |= 1 << ID_TC0;
	
	// Disable the clock for TC0
	TC0->TC_CHANNEL->TC_CCR = TC_CCR_CLKDIS;
	
	// Disable interrupts
	TC0->TC_CHANNEL->TC_IDR = 0xFFFFFFFF;
	
	// Clear status register
	TC0->TC_CHANNEL->TC_SR;
	
	// Set Mode
	TC0->TC_CHANNEL->TC_CMR = TC_CMR_CPCTRG | TC_CMR_TCCLKS_TIMER_CLOCK5;
	
	// Set compare value register TC_CR
	TC0->TC_CHANNEL[0].TC_RC = 10000;
	
	// Configure and enable interrupt on RC compare
	NVIC_EnableIRQ((IRQn_Type) ID_TC0);
	TC0->TC_CHANNEL->TC_IER = TC_IER_CPCS;
	
	// Enable interrupts
	//TC0->TC_CHANNEL->TC_IER = 0xFFFFFFFF;
}

void TC0_Handler(void)
{
	// Clear status register
	TC0->TC_CHANNEL->TC_SR;
	
	// Disable timer
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
	
	iDelay = 1;
}

void delay_ms(uint32_t iPrescription)
{
	// Set compare value for TC0
	TC0->TC_CHANNEL[0].TC_RC = iPrescription*29;
	
	// Reset and enable timer
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN|TC_CCR_SWTRG;
	
	while(!iDelay);
	
	iDelay = 0;
}