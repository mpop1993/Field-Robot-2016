/*
 * global_variables.h
 *
 * Created: 5/19/2016 5:40:17 PM
 *  Author: Mihai
 */ 

// ----- Encoder related variables


#define ENCODER_DR 250
#define ENCODER_ST 250
#define BASE_SPEED 70
#define KP_ENC 5
#define KD_ENC 0

uint32_t iEncoder_DR;
uint32_t iEncoder_ST;
uint32_t iEncoder_DR_current;
uint32_t iEncoder_ST_current;
uint16_t iSpeed_DR;
uint16_t iSpeed_ST;