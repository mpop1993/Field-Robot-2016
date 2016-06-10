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
	void InitMotors(void);
	void WriteMotors(int percent_ST, int percent_DR);
	void ForwardDrive(void);
	void ControlledDrive(uint32_t percentage_ST, uint32_t percentage_DR);
	
#endif