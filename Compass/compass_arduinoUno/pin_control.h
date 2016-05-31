/*
 * pin_control.h
 *
 * Created: 22.05.2016 12:50:17
 *  Author: Win7
 */ 
#define INPUT 0
#define OUTPUT 1

#define set_bit(val, bit_no) ( val|= (1 << bit_no ) )
#define clear_bit(val, bit_no) ( val&= ( ~(1 << bit_no) ) )
#define set_pin(PORT,PIN) set_bit(PORT,PIN)
#define clear_pin(PORT,PIN) clear_bit(PORT,PIN)
/*
* set direction PIN from PORT
* MODE values: INPUT or OUTPUT
*/
#define pin_mode(PORT, PIN, MODE) do{\
	if(MODE) set_bit(PORT,PIN);\
	else  clear_bit(PORT,PIN);\
}while(0)