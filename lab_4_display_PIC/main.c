#include <main.h>
#include "LCD_driver.c"
//#include <string.h>
//#include <stdio.h>

/*#define LCD_ENABLE_PIN PIN_B2
#define LCD_RS_PIN PIN_B0
#define LCD_RW_PIN PIN_B1
#define LCD_DATA4 PIN_B4
#define LCD_DATA5 PIN_B5
#define LCD_DATA6 PIN_B6
#define LCD_DATA7 PIN_B7*/
volatile int H[3] = {0,0,0};
volatile int M[3] = {0,0,0};
volatile int S[3] = {0,0,0};

#define ARBITRARY_SECOND 25 //3

void Output(int i)
{
   char str[8];
   sprintf(str, "%02d:%02d:%02d",H[i],M[i],S[i]);
   LCD_GotoXY(8,i);
   LCD_SendString(str,8);
}

void UpdateClock()
{
   S[0]++;
   if (S[0]==60)
   {
      S[0]=0;
      M[0]++;
      if (M[0]==60)
      {
         M[0]=0;
         H[0]++;
         if (M[0]==60)
         {
            H[0]=0;
         }
      }
   }
   Output(0);
}

volatile int count;
volatile int safety;
//таймер
#INT_TIMER1
void TimerOverflow() 
{
    if (safety!=0)
   {
      safety--;
   }

   count++;
   if(count==ARBITRARY_SECOND)
   {
      UpdateClock();
      count=0;
   }
}

#int_rb
void button_isr() {
   //delay_ms (20);  //debounce
   if( input(PIN_B4)==0)
      {
         if (safety==0)
               {
                  H[1]=H[0];
                  M[1]=M[0];
                  S[1]=S[0];
                  Output(1);
                  safety = ARBITRARY_SECOND;
               }
      }
      else{
      if( input(PIN_B5)==0)
         {
            if (safety==0)
               {
                  H[2]=H[0];
                  M[2]=M[0];
                  S[2]=S[0];
                  Output(2);
                  safety = ARBITRARY_SECOND;
               }
         }
      }
 
}

void main()
{
   enable_interrupts(global);
   enable_interrupts(int_rb);   
   PORT_B_PULLUPS(0b00110000);
   
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_TIMER1);
   

/*set_tris_b((0b11111000));
//TRISB=0b11111000;            // RB0, RB1 и RB2 на выход, остальные на вход
output_b((0b00000000));
//PORTB=0b00000000;           // Устанавливаем на выходах нули
INTCON=0b10001000;*/


   LCD_Init();
   LCD_GotoXY(0,0);
   char str1[] = " Time:    N/A    ";
   LCD_SendString(str1,16);
   LCD_GotoXY(0,1);
   char str2[] = "Lap1:    N/A    ";
   LCD_SendString(str2,16);
   LCD_GotoXY(0,2);
   char str3[] = "Lap2:    N/A    ";
   LCD_SendString(str3,16);
   LCD_GotoXY(0,3);
   char str4[] = "Cтрока          ";
   LCD_SendString(str4,16);
   LCD_NewItem();
   LCD_GotoXY(14,3);
   LCD_SendChar(0x01);
  
   while(TRUE)
   { 
      //TODO: User Code
   }

}
