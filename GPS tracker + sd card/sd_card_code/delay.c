/*
---------------------------------------------------------------------
   ��������� ��������
---------------------------------------------------------------------
*/


#include "delay.h"


// �������� t ����������� X10 (Freq - 4MHz)
extern void delay4_10us(unsigned char t)
{
   do
   {
      CLRWDT();
      NOP();
      NOP();
      NOP();
      NOP();
      --t;
   } while (t > 0);
}


// �������� t ���������� (Freq - 4MHz)
extern void delay4_ms(unsigned int t)
{
   do {
      delay4_10us(99);
   } while(--t);
}


// �������� t ����������� X5 (Freq - 20MHz)
extern void delay20_5us(unsigned char t)
{
   unsigned char n;
   do
   {
      for (n=0; n<3; ++n) { CLRWDT(); }
      --t;
   } while (t > 0);
}


// �������� t ����������� X10 (Freq - 20MHz)
extern void delay20_10us(unsigned char t)
{
   unsigned char n;
   do
   {
      for (n=0; n<7; ++n) { CLRWDT(); }
      --t;
   } while (t > 0);
}


// �������� t ���������� (Freq - 20MHz)
extern void delay20_ms(unsigned int t)
{
   do {
      delay20_10us(99);
   } while(--t);
}


// END of FILE
