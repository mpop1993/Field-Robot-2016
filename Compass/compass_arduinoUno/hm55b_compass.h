/*
 * hm55b_compass.h
 *
 * Created: 15.05.2016 23:53:11
 *  Author: Win7
 */ 
#ifndef HM55B_COMPASS_H
#include <avr/io.h>
#define CLK_PIN PORTB0  //pin 8 arduino uno
#define EN_PIN  PORTB1  //pin 9 arduino uno
#define DIO_PIN PORTB2  //pin 10 arduino uno

void ShiftOut(int Value, int BitsCount);
int ShiftIn(int BitsCount);
void HM55B_Reset();
void HM55B_StartMeasurementCommand();
int HM55B_ReadCommand();
void HM55B_PinInit();

#endif 