/*
 * motor_control.h
 *
 * Created: 5/15/2016 2:28:20 PM
 *  Author: Mihai
 */ 
#ifndef MOTOR_CONTROL
#define MOTOR_CONTROL

	void InitPWMController_CLKA(void);
	void InitPWMController_MCLK(void);
	void Init_Motors(void);
	void WriteMotors(int percent_ST, int percent_DR);
	
#endif