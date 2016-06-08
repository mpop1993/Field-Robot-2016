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

	void configure_uart(void);
	extern int uart_getchar(uint8_t *c);
	extern int uart_putchar(const uint8_t c);
	extern void sendString(const char* c, uint16_t length);
	void printInt(int value, char* buffer);
	uint8_t getNewSpeed();
#endif