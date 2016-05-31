/*
 * UART.h
 *
 * Created: 5/19/2016 5:52:49 PM
 *  Author: Mihai
 */ 

#define SUPERVISED_MODE

#if defined(MANUAL_MODE)


#elif defined(AUTO_MODE)

	
#elif defined(SUPERVISED_MODE)

	void Configure_UART(void);
	int uart_getchar(uint8_t *c);
	int uart_putchar(const uint8_t c);

#endif