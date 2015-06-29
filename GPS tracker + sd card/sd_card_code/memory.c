/*
---------------------------------------------------------------------
   –абота с EEPROM и FLASH пам€тью контроллера
---------------------------------------------------------------------
*/


#include "memory.h"


// «апись нескольких байт в EEPROM пам€ть
// adr - адрес начальной €чейки
// *pbuff - указатель на буфер с данными
// num_vals - кол-во байт дл€ записи
void eeprom_writebuff(unsigned char adr, unsigned char *pbuff, unsigned char num_vals)
{
   unsigned char i;
   for(i=0; i<num_vals; i++, adr++)
   {
      eeprom_writebyte(adr, pbuff[i]);
   }
}


// „тение нескольких байт из EEPROM пам€ти
// adr - адрес начальной €чейки
// *pbuff - указатель на буфер с данными
// num_vals - кол-во байт дл€ чтени€
void eeprom_readbuff(unsigned char adr, unsigned char *pbuff, unsigned char num_vals)
{
   unsigned char i;
   for(i=0; i<num_vals; i++, adr++)
   {
      pbuff[i] = eeprom_readbyte(adr);
   }
}


// «апись байта в EEPROM пам€ть
// adr - адрес €чейки
// d - данные
void eeprom_writebyte(unsigned char adr, unsigned char d)
{

   // проверка окончани€ предыдущей записи
   while (WR) { };

   EEADR  = adr;  // адрес €чейки
   EEDATA = d;    // данные

   EEPGD  = 0;    // запись в eeprom
   WREN   = 1;    // разрешить запись в eeprom

   GIE = 0;       // запрет прерываний

   EEIF   = 0;    //

   // об€зательна€ последовательность
   EECON2 = 0x55;
   EECON2 = 0xAA;
   WR = 1; // запись

   WREN = 0;	  // запретить запись в eeprom
   GIE  = 1;      // разрешить прерывани€
}

// „тение байта из EEPROM пам€ти
// adr - адрес €чейки
unsigned char eeprom_readbyte(unsigned char adr)
{

   // проверка окончани€ предыдущей записи
   while (WR) { };

   EEADR = adr;   // адрес €чейки
   EEPGD = 0;     // чтение из eeprom
   RD = 1;	  // чтение
   return(EEDATA);
}



// „тение 2 байт из FLASH пам€ти
// adrh, adrl - адрес €чейки
// *pdh, *pdl - указатель дл€ сохранени€ данных
void flash_readbyte(unsigned char adrh, unsigned char adrl, unsigned char *pdh, unsigned char *pdl)
{

   // проверка окончани€ предыдущей записи
   while (WR) { };

   EEADR = adrl;   // адрес €чейки L
   EEADRH= adrh;   // адрес €чейки H
   EEPGD = 1;      // чтение из flash
   RD = 1;	   // чтение

   // об€зательна€ последовательность
   NOP();
   NOP();

   *pdl = EEDATA;  // данные L
   *pdh = EEDATH;  // данные H
}


// „тение 2 байт из FLASH пам€ти
// adr - адрес €чейки
int flash_readint(int adr)
{
   int temp;

   // проверка окончани€ предыдущей записи
   while (WR) { };

   EEADR = adr;        // адрес €чейки L
   EEADRH= adr >> 8;   // адрес €чейки H
   EEPGD = 1;          // чтение из flash
   RD = 1;	       // чтение

   // об€зательна€ последовательность
   NOP();
   NOP();

   temp = EEDATH << 8;    // данные L
   temp = temp | EEDATA;  // данные H

   return(temp);
}


// «апись 2 байт в FLASH пам€ть
// adrh, adrl - адрес €чейки
// dh, dl - данные
void flash_writebyte(unsigned char adrh, unsigned char adrl, unsigned char dh, unsigned char dl)
{

   // проверка окончани€ предыдущей записи
   while (WR) { };

   EEADR  = adrl;   // адрес €чейки L
   EEADRH = adrh;   // адрес €чейки H
   EEDATA = dl;     // данные L
   EEDATH = dh;     // данные H

   GIE = 0;       // запрет прерываний

   EEPGD  = 1;    // запись в flash
   WREN   = 1;    // разрешить запись в flash
   EEIF   = 0;    //

   // об€зательна€ последовательность
   EECON2 = 0x55;
   EECON2 = 0xAA;
   WR = 1; // запись

   NOP();
   NOP();

   WREN = 0;	  // запретить запись в flash
   GIE  = 1;      // разрешить прерывани€
}


// «апись 2 байт в FLASH пам€ть
// adr - адрес €чейки
// d - данные
void flash_writeint(int adr, int d)
{

   // проверка окончани€ предыдущей записи
   while (WR) { };

   EEADR  = adr;      // адрес €чейки L
   EEADRH = adr >> 8; // адрес €чейки H
   EEDATA = d;        // данные L
   EEDATH = d >> 8;   // данные H

   GIE = 0;       // запрет прерываний

   EEPGD  = 1;    // запись в flash
   WREN   = 1;    // разрешить запись в flash
   EEIF   = 0;    //

   // об€зательна€ последовательность
   EECON2 = 0x55;
   EECON2 = 0xaa;
   WR = 1; // запись

   NOP();
   NOP();

   WREN = 0;	  // запретить запись в flash
   GIE  = 1;      // разрешить прерывани€
}


// END of FILE
