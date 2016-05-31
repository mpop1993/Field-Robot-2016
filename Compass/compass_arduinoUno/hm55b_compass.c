/*
 * hm55b_compass.c
 *
 * Created: 15.05.2016 23:52:41
 *  Author: Win7
 */ 
#include "hm55b_compass.h"
#include <util/delay.h>
#include "pin_control.h"
#define DELAY_US 1
void HM55B_PinInit()
{
	pin_mode(DDRB,EN_PIN,OUTPUT);
	pin_mode(DDRB,CLK_PIN,OUTPUT);
	pin_mode(DDRB,DIO_PIN,INPUT);	
}
void ShiftOut(int Value, int BitsCount)
{
	for(int i = BitsCount; i >= 0; i--)
	{
		clear_pin(PORTB,CLK_PIN);
		if((Value & 1<< i ) == ( 1 << i))
		{
			set_pin(PORTB,DIO_PIN);
		}
		else
		{
		   clear_pin(PORTB,DIO_PIN);
		}
		set_pin(PORTB,CLK_PIN);
		_delay_us(DELAY_US);
	
	}
	
}
int ShiftIn(int BitsCount)
{
	int ShiftIn_result = 0;
	pin_mode(DDRB,DIO_PIN,INPUT);
	for( int i = BitsCount; i >= 0 ; i--)
	{
		set_pin(PORTB,CLK_PIN);
		_delay_us(DELAY_US);
		if(PINB & (1 <<DIO_PIN) )
		{
			ShiftIn_result = ( ShiftIn_result << 1) + 1;
		}
		else
		{
			ShiftIn_result = (ShiftIn_result << 1) + 0;
		}
		clear_pin(PORTB,CLK_PIN);
		_delay_us(DELAY_US);
	}
	// if bit 11 is set, the value is negative 
	if ((ShiftIn_result & 1 << 11) == 1 << 11)
    {
		//ShiftIn_result = (0b11111000 << 8) | ShiftIn_result; // add in the upper Byte of the integer
		ShiftIn_result = (0xF8 << 8) | ShiftIn_result; // add in the upper Byte of the integer
	}


	return ShiftIn_result; 
}
void HM55B_Reset()
{
	pin_mode(DDRB,DIO_PIN,OUTPUT);
	clear_pin(PORTB,EN_PIN);
	ShiftOut(0x00, 3);
	set_pin(PORTB,EN_PIN);
}
void HM55B_StartMeasurementCommand()
{
	pin_mode(DDRB,DIO_PIN,OUTPUT);
	clear_pin(PORTB,EN_PIN);
	//ShiftOut(0b1000, 3);
	ShiftOut(0x08, 3);
	set_pin(PORTB,EN_PIN);	
}
int HM55B_ReadCommand()
{
	int result = 0;
	pin_mode(DDRB, DIO_PIN,OUTPUT);
	clear_pin(PORTB,EN_PIN);
	//ShiftOut(0b1100, 3);
	ShiftOut(0x0C, 3);
	result = ShiftIn(3);
	return result;
	
}