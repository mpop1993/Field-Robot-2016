/*
 * adc.c
 *
 * Created: 19.04.2016 19:05:09
 *  Author: Win7
 */ 
#include <avr/io.h>
#include "avr/interrupt.h"
#include "adc.h"
void adc_init()
{
	
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
	ADMUX |= (1<<REFS0);//AVcc with external capacitor at Aref pin
	ADCSRA |= (1<<ADEN);//enable ADC
	ADCSRA |= (1<<ADSC);//ADC start conversion
}

uint8_t  read_adc(uint8_t channel){
	ADMUX &= 0xF0;//set input A0 to A5
	ADMUX |= channel;//select chanel A0 to A5
	ADCSRA |= (1<<ADSC);//start conversion
	while(ADCSRA & (1<<ADSC));//wait wile adc conversion are not updated
	return (uint8_t)ADCW;//read and return voltage
}
