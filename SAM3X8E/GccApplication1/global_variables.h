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

volatile uint32_t iEncoder_DR;
volatile uint32_t iEncoder_ST;
volatile uint32_t iEncoder_DR_current;
volatile uint32_t iEncoder_ST_current;
volatile uint16_t iSpeed_DR;
volatile uint16_t iSpeed_ST;
extern volatile uint8_t percentage_ST;
extern volatile uint8_t percentage_DR;
extern volatile uint8_t newSpeed;