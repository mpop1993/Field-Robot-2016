/*
 * event.c
 *
 * Created: 26.04.2016 20:57:10
 *  Author: Win7
 */ 
#include "event.h"
#include "stdio.h"
#include "pin_control.h"
#include "hm55b_compass.h"
#include <math.h>
#include <util/delay.h>
#define FALSE  0;
#define TRUE  1;
int adjust_angle(int initial_angle, int angle_presc)
{
	int target_angle;
	if(initial_angle < 0 && angle_presc < 0)
	target_angle = initial_angle - angle_presc;
	else
	target_angle = initial_angle + angle_presc;
	
	if( target_angle > 180 )
	target_angle = (target_angle -180) - 180;
	else if( target_angle < -180)
	target_angle = (target_angle + 180) + 180;
	
	return target_angle;
	
}
void event_handler_rcv(int8_t angle_presc)
{
    int8_t initial_angle = 0;
    uint8_t is_init_angle_read = FALSE;
    uint8_t angle_found = FALSE;
	int target_angle=0;
	// pin 12 arduino uno -- when is high -- angle was found
	pin_mode(DDRB,PORTB4, OUTPUT);
	
	/*init compass*/
	HM55B_PinInit();
	HM55B_Reset();
	pin_mode(DDRB, CLK_PIN,OUTPUT);
	
	/*read data from compass until angle will be found*/
	while(!angle_found)
	{
		
		HM55B_StartMeasurementCommand(); // necessary!!
		_delay_ms(40); // the data is 40ms later ready
	
		HM55B_ReadCommand(); // read data and print Status

	
		int X_Data = ShiftIn(11); // Field strength in X
		int Y_Data = ShiftIn(11); // and Y direction
	
	
		set_pin(PORTB,EN_PIN); // ok deselect chip
		int actual_angle = 180 * (atan2(-1 * Y_Data , X_Data) / M_PI); // angle is atan( -y/x) !!!
	
		if(!is_init_angle_read)
		{
			initial_angle = actual_angle;
			target_angle = adjust_angle(initial_angle, angle_presc);
				
			is_init_angle_read = TRUE;
		}
		
		if(is_init_angle_read)
		{
			if( (actual_angle >= target_angle) && (angle_presc <=0) && (target_angle >=0) )
			{
				angle_found = TRUE;
			}
			else if( (actual_angle >= target_angle) && (angle_presc < 0) && (target_angle<0) )
			{
				angle_found = TRUE;
			}
			else if( (actual_angle <= target_angle) && (angle_presc>=0) && (target_angle<0) )
			{
				angle_found = TRUE;
			}
			else if( (actual_angle <= target_angle) && (angle_presc>=0) && (target_angle>=0) )
			{
				angle_found = TRUE;
			}	
			
			if(angle_found) /*if angle was found, set pin to high*/
				set_pin(PORTB,PORTB4);
		} 
		
		
	}
	
	

}