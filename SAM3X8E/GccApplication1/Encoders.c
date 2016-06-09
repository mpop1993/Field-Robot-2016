/*
 * Encoders.c
 *
 * Created: 5/19/2016 5:33:11 PM
 *  Author: Mihai
 */ 

// *************************************************************************************************************************************
// ----- Includes
#include "sam.h"
#include "Encoders.h"
#include "global_variables.h"
#include "TmrCfg.h"

// ----- Local variables


// ----- Function prototipes


// *************************************************************************************************************************************

void PIOB_Handler()
{
	//Since the interrupt could be any pin on PORTB we need to
	//  check if PB27 is the one triggering the interrupt.
	if((PIOB->PIO_ISR & PIO_PB27)==PIO_PB27){
		iEncoder_ST++;
		iEncoder_ST_current++;
	}
}

void PIOD_Handler()
{
	//Since the interrupt could be any pin on PORTD we need to
	//  check if PD8 is the one triggering the interrupt.
	if((PIOD->PIO_ISR & PIO_PD8)==PIO_PD8){
		iEncoder_DR++;
		iEncoder_DR_current++;
	}
}


