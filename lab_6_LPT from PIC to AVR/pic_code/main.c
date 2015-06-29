#include <16F877A.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#use delay(crystal=20000000)

//// PARALLEL ////
#define DATA_PORT output_d
#define DATA_TRIS set_tris_d

#define CONTROL_TRIS set_tris_c
#define LINE PIN_B1
#define STROB PIN_C7

#define DELAY_TIME (2)

void Init() {
   DATA_TRIS(0x00);
   CONTROL_TRIS(0xFF & ~(1 << STROB));
 
   output_high(STROB);
   
   
}

void SendByte(int8 send) {
   DATA_PORT(send);
   delay_ms(2);  
}

char* text="42+42-42*42/42=?";
int8 l = 16;


void Send() {     
for (int8 i = 0; i < l; ++i) {  
        while(input(LINE)==0){ }
        output_low(STROB);
        delay_ms(DELAY_TIME);
        SendByte(text[i]);
        while(input(LINE)==1){ }  
        output_high(STROB);
        delay_ms(10);
   }   
}

////END PARALLEL ////

#INT_EXT //SCLK
void ext_isr(void) {
   Send();
}

void main() {
   
   disable_interrupts(GLOBAL);
   Init();
   // buttons //
   set_tris_b(0xff);
   ext_int_edge(L_TO_H); 
   enable_interrupts(INT_RB);
   port_b_pullups(1);
   
   
   clear_interrupt(INT_RB);
   enable_interrupts(INT_EXT);
   enable_interrupts(global);
   
   while(TRUE) {
      //TODO: User Code
   }

}


