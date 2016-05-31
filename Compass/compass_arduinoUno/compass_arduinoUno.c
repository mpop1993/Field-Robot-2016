/*
 * compass_arduinoUno.c
 *
 * Created: 19.04.2016 19:04:27
 *  Author: Win7
 */ 

#define F_CPU 16000000UL 
#include <avr/io.h>
#include "avr/interrupt.h"
#include <stdio.h>
#include <math.h>
#include <util/delay.h>
#include "USART.h"
#include "event.h"
#include "hm55b_compass.h"
#include "pin_control.h"
int volatile rcv_cmd_flag;
#define SEND_COMPASS_DATA
int main(void)
{
	
	int X_Data = 0;
	int Y_Data = 0;
	int angle;
	char outputString[12];
	USART_init();
	pin_mode(DDRB,PORTB4, OUTPUT);
	
	HM55B_PinInit();
	HM55B_Reset();
	pin_mode(DDRB, CLK_PIN,OUTPUT);
	
	
	int i=0;
	//char read_char = USART_receive();
	//set_pin(PORTB,PORTB4);
	
	while(1)
	{
		//set_pin(PORTB,PORTB4);
#ifdef SEND_COMPASS_DATA		
		HM55B_StartMeasurementCommand(); // necessary!!
		_delay_ms(40); // the data is 40ms later ready

		 HM55B_ReadCommand(); // read data and print Status
		

		X_Data = ShiftIn(11); // Field strength in X
		Y_Data = ShiftIn(11); // and Y direction

		
		set_pin(PORTB,EN_PIN); // ok deselect chip
	    angle = 180 * (atan2(-1 * Y_Data , X_Data) / M_PI); // angle is atan( -y/x) !!!
		
		dtostrf((float)X_Data,6,0,outputString);
		USART_putstring(outputString);
		
		USART_send(' ');
		dtostrf((float)Y_Data,6,0,outputString);
		USART_putstring(outputString);
		
		USART_send(' ');
		dtostrf((float)angle,6,0,outputString);
		USART_putstring(outputString);
		
		USART_send('\n');
		
#endif		


	}
    
}