/*
---------------------------------------------------------------------
   Временные задержки
---------------------------------------------------------------------
*/


#include <htc.h>


// задержка t микросекунд X10 (Freq - 4MHz)
extern void delay4_10us(unsigned char t);

// задержка t милисекунд (Freq - 4MHz)
extern void delay4_ms(unsigned int t);

// задержка t микросекунд X5 (Freq - 20MHz)
extern void delay20_5us(unsigned char t);

// задержка t микросекунд X10 (Freq - 20MHz)
extern void delay20_10us(unsigned char t);

// задержка t милисекунд (Freq - 20MHz)
extern void delay20_ms(unsigned int t);


// END of FILE
