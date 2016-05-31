/*
 * Timers.c
 *
 * Created: 5/14/2016 1:28:26 PM
 *  Author: Mihai
 */ 

#include "sam.h"
#include "Timers.h"

uint8_t iDelay = 0;

static void Configure_TC0(uint16_t iPrescription)
{
	// Enable TC0 (27 is TC0)
	PMC->PMC_PCER0 = 1 << 27;
	
	// Disable TC clock
	TC0->TC_CHANNEL->TC_CCR = TC_CCR_CLKDIS;

	// Disable interrupts
	TC0->TC_CHANNEL->TC_IDR = 0xFFFFFFFF;
	
	// Clear status register
	TC0->TC_CHANNEL->TC_SR;
	
	// Set Mode
	TC0->TC_CHANNEL->TC_CMR = TC_CMR_CPCTRG | TC_CMR_TCCLKS_TIMER_CLOCK5;
	
	// Compare Value
	TC0->TC_CHANNEL[0].TC_RC = iPrescription * 28; // 24 = 1 ms; 59 = 2 ms
	
	// Configure and enable interrupt on RC compare
	NVIC_EnableIRQ((IRQn_Type) ID_TC0);
	TC0->TC_CHANNEL->TC_IER = TC_IER_CPCS;
	
	// Reset counter (SWTRG) and enable counter clock (CLKEN)
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void TC0_Handler(void)
{
	volatile uint32_t ul_status;
	ul_status = TC0->TC_CHANNEL->TC_SR;

	iDelay = 1;
}

void delay(uint16_t iPrescription){

	Configure_TC0(iPrescription);
	
	while(iDelay == 0){
		
		// DO NOTHING - Wait for the event to happen
	}
	iDelay = 0;
	
	// Disable TC clock
	TC0->TC_CHANNEL->TC_CCR = TC_CCR_CLKDIS;
	
	// Disable interrupts
	TC0->TC_CHANNEL->TC_IDR = 0xFFFFFFFF;
	
	// Clear status register
	TC0->TC_CHANNEL->TC_SR;
}