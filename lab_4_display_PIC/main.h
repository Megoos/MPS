   
#define notATmega

#ifdef ATmega
   #include <avr/io.h>
   #include <util/delay.h>
   #include <avr/pgmspace.h>
   //#define F_CPU 8000000
#else
   #include <16F877A.h>
   #include <stdint.h>

   #device ADC=16

   #FUSES NOWDT                    //No Watch Dog Timer
   #FUSES NOBROWNOUT               //No brownout reset
   #FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

   #use delay(crystal=20000000)

#endif



