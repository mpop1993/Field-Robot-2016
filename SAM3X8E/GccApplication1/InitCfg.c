/*
 * InitCfg.c
 *
 * Created: 5/19/2016 2:50:32 PM
 *  Author: Mihai
 */ 

// *************************************************************************************************************************************
// ----- Includes
#include "sam.h"
#include "InitCfg.h"

// ----- Defines
#define _BV(bit) (0x1u << bit)

// *************************************************************************************************************************************
void Pin_Configuration()
{
	// Enable clock for line A
		PMC->PMC_PCER0 |= _BV(ID_PIOA);
	// Enable clock for line C
		PMC->PMC_PCER0 |= _BV(ID_PIOC);
	// Enable clock for line B
		PMC->PMC_PCER0 |= _BV(ID_PIOB);
	// Enable clock for line D
		PMC->PMC_PCER0 |= _BV(ID_PIOD);

	// Set pin 13[B27] as input for encoder data - left[ST]
		PIOB -> PIO_PER |= PIO_PB27;
		PIOB -> PIO_ODR |= PIO_PB27;
		PIOB -> PIO_PUDR |= PIO_PB27;
		//Additional Interrupt Modes Enable Register
		PIOB->PIO_AIMER |= PIO_PB27;
		//Edge Select Register
		PIOB->PIO_ESR |= PIO_PB27;
		//Rising Edge/Low Level Select Register
		PIOB->PIO_REHLSR |= PIO_PB27;
		//Finally enable interrupts on PORTB.PB27
		PIOB->PIO_IER |= PIO_PB27;
		NVIC_EnableIRQ(PIOB_IRQn);
	
	// Set pin 12[D8] as input for encoder data - right[DR]
		PIOD -> PIO_PER |= PIO_PD8;
		PIOD -> PIO_ODR |= PIO_PD8;
		PIOD -> PIO_PUDR |= PIO_PD8;
		//Additional Interrupt Modes Enable Register
		PIOD -> PIO_AIMER |= PIO_PD8;
		//Edge Select Register
		PIOD -> PIO_ESR |= PIO_PD8;
		//Rising Edge/Low Level Select Register
		PIOD -> PIO_REHLSR |= PIO_PD8;
		//Finally enable interrupts on PORTD.PD8
		PIOD -> PIO_IER |= PIO_PD8;
		NVIC_EnableIRQ(PIOD_IRQn);
		
	// Set pin 11[D7] as output for Buzzer - PCB2 pin U
		PIOD -> PIO_PER |= PIO_PD7; // PIO_PER = PIO enable register
		PIOD -> PIO_OER |= PIO_PD7; // PIO_OER = Output Enable Register
		PIOD -> PIO_PUDR |= PIO_PD7; // Disable pull-up on pin D7
	
	// Set pin 9[C21] as PWM output for motors - left[ST]
		PIOC->PIO_PDR |= PIO_PC22; // PIO_PDR = PIO Disable register
		PIOC->PIO_ABSR |= PIO_PC22; // PIO_ABSR = Peripheral AB Select Register
		PIOC->PIO_OER |= PIO_PC22; // Enable output on pin C22
		PIOC->PIO_PUDR |= PIO_PC22; // Disable pull-up on pin C22
	
	// Set pin 8[C22] as PWM output for motors - right[DR]
		PIOC->PIO_PDR |= PIO_PC21; // Disable PIO Control on PC21 and set up for Peripheral B
		PIOC->PIO_ABSR |= PIO_PC21;
		PIOC->PIO_OER |= PIO_PC21; // Enable output on pin C21
		PIOC->PIO_PUDR |= PIO_PC21; // Enable pull-up on pin C21
	
	// Set pin 7[C23] as output for LED - PCB2 pinS
		PIOC -> PIO_PER |= PIO_PC23;
		PIOC -> PIO_OER |= PIO_PC23;
		PIOC -> PIO_PUDR |= PIO_PC23;
		
	// Set pin 6[C24] as output for PUMP - PCB2 pinH
		PIOC -> PIO_PER |= PIO_PC24;
		PIOC -> PIO_OER |= PIO_PC24;
		PIOC -> PIO_PUDR |= PIO_PC24;
		
	// Set pin 5[C25] as input for start/stop from camera
		PIOC -> PIO_PER |= PIO_PC25;
		PIOC -> PIO_ODR |= PIO_PC25;
		PIOC -> PIO_PUDR |= PIO_PC25;
		//Additional Interrupt Modes Enable Register
		PIOC->PIO_AIMER |= PIO_PC25;
		//Edge Select Register
		PIOC->PIO_ESR |= PIO_PC25;
		//Rising Edge/Low Level Select Register
		PIOC->PIO_REHLSR |= PIO_PC25;
		//Finally enable interrupts on PORTB.PB27
		PIOC->PIO_IER |= PIO_PC25;
		NVIC_EnableIRQ(PIOC_IRQn);
		
	// Set pin 3[C28] as input for motor init
		PIOC -> PIO_PER |= PIO_PC28;
		PIOC -> PIO_ODR |= PIO_PC28;
		PIOC -> PIO_PUDR |= PIO_PC28;
		//Additional Interrupt Modes Enable Register
		PIOC->PIO_AIMER |= PIO_PC28;
		//Edge Select Register
		PIOC->PIO_ESR |= PIO_PC28;
		//Rising Edge/Low Level Select Register
		PIOC->PIO_REHLSR |= PIO_PC28;
		//Finally enable interrupts on PORTC.PC28
		PIOC->PIO_IER |= PIO_PC28;
		NVIC_EnableIRQ(PIOC_IRQn);
		
	//// Set pin 5[C25] as input for start/stop from camera
		//PIOA -> PIO_PER |= PIO_PA14;
		//PIOA -> PIO_ODR |= PIO_PA14;
		//PIOA -> PIO_PUDR |= PIO_PA14;
		////Additional Interrupt Modes Enable Register
		//PIOA->PIO_AIMER |= PIO_PA14;
		////Edge Select Register
		//PIOA->PIO_ESR |= PIO_PA14;
		////Rising Edge/Low Level Select Register
		//PIOA->PIO_REHLSR |= PIO_PA14;
		////Finally enable interrupts on PORTB.PB27
		//PIOA->PIO_IER |= PIO_PA14;
		//NVIC_EnableIRQ(PIOA_IRQn);
		
		// ----- TASK_1 Interrupt priority
		#if defined(TASK_1)
			
		// ----- TASK_2 Interrupt priority
		#elif defined(TASK_2)
		
		// ----- TASK_3 Interrupt priority
		#elif defined(TASK_3)
			NVIC_SetPriority(PIOC_IRQn,1);
		// ----- TASK_4 Interrupt priority
		#elif defined(TASK_4)
	
		#endif
}


