#include <16F877A.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)

//// SPI ////

#define SPI_tris set_tris_d
#define SPI_port output_d
#define SS PIN_D3
#define MISO PIN_D1
#define MOSI PIN_D2
#define SCLK PIN_D0

#define High() output_high(MISO);
#define Low() output_low(MISO); 

void SPI_SlaveInit() {
   SPI_tris(0xFF & ~(1 << MISO));
}

int8 SPI_recieve() {
   
   Low();
   while (input(SS)) {}
  
   while (!input(SS) ) {
       while(!input(SCLK)){}
         Low();        
       while(input(SCLK)){}
   }
   return 1;
}

int8 SPI_Send(int8 send) {
    
  if (send & 0x80) {
         High(); 
   }
   else { 
         Low(); 
   }
   //send <<= 1;  
   while (input(SS)) {}
      while (!input(SS) ) {     
         while(!input(SCLK)){}         
             if (send & 0x80) { 
               High(); 
               }
            else { 
               Low(); 
               }
                 send <<= 1; 
         while(input(SCLK)){}     
      }
     return 1;
}
/*
int8 SPI_Send(int8 send) {
  
   if (send & 0x80) { High(); }
         else { Low(); }
  // send <<= 1;
 
   while (input(SS)) {}
   int8 state = input(SCLK);
   while (!input(SS) ) {
      int8 state1 = input(SCLK);
       
      if (state == 1 && (state1 == 0)) {
      send <<= 1;
         if (send & 0x80) { High(); }
         else { Low(); }
         //ans = (input(MOSI) << 1);
         //ans <<= 1;
         //send <<= 1;
         //++k;
      }
      state = state1;
   }
   return 1;
}
*/


#INT_EXT //SCLK
void ext_isr(void) {
}

int status = 0;
//#byte PortB=6
#INT_RB 
void int_rb_isr() {
   if(!input(PIN_B4)) {
      // //Button1
      status = 0;
      output_low(PIN_B1);
      output_c(0);
      Low();      
   }
   if(!input(PIN_B5)) {
      // //Button2
      status = 1;
      output_high(PIN_B1);
      High();      
   }
   clear_interrupt(INT_RB);
}

void main() {
   disable_interrupts(GLOBAL);
   delay_ms(100);

   SPI_SlaveInit();
   
   // buttons //
   set_tris_b(0xff);
   ext_int_edge(L_TO_H); 
   enable_interrupts(INT_RB);
   port_b_pullups(1);
   clear_interrupt(INT_RB);
   enable_interrupts(INT_EXT);
   //ens buttons //
  
   enable_interrupts(global);
   
   //char text0[11] = { 0xF0, 0x0F, 0xF0, 0x0F, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
   
   char* text2 = " Солнышкина ";
   char* text1 = "Звездочкина ";
   
   output_c(0x00);
   int8  flag= 0;
   while(1) {
     //flag = SPI_Send(' ');
     flag = SPI_recieve();
      if (flag != 0) {
         if (status == 0) {
            for (int8 i = 0; i < 11; ++i) {
                 SPI_Send(text1[i]);                 
            }           
         } else {
            for (int8 i = 0; i < 11; ++i) {
                 SPI_Send(text2[i]);                 
            }         
         }
         flag = 0;
      }       
   }
}

