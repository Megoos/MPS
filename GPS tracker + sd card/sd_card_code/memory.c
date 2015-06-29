/*
---------------------------------------------------------------------
   Работа с EEPROM и FLASH памятью контроллера
---------------------------------------------------------------------
*/


#include "memory.h"


// Запись нескольких байт в EEPROM память
// adr - адрес начальной ячейки
// *pbuff - указатель на буфер с данными
// num_vals - кол-во байт для записи
void eeprom_writebuff(unsigned char adr, unsigned char *pbuff, unsigned char num_vals)
{
   unsigned char i;
   for(i=0; i<num_vals; i++, adr++)
   {
      eeprom_writebyte(adr, pbuff[i]);
   }
}


// Чтение нескольких байт из EEPROM памяти
// adr - адрес начальной ячейки
// *pbuff - указатель на буфер с данными
// num_vals - кол-во байт для чтения
void eeprom_readbuff(unsigned char adr, unsigned char *pbuff, unsigned char num_vals)
{
   unsigned char i;
   for(i=0; i<num_vals; i++, adr++)
   {
      pbuff[i] = eeprom_readbyte(adr);
   }
}


// Запись байта в EEPROM память
// adr - адрес ячейки
// d - данные
void eeprom_writebyte(unsigned char adr, unsigned char d)
{

   // проверка окончания предыдущей записи
   while (WR) { };

   EEADR  = adr;  // адрес ячейки
   EEDATA = d;    // данные

   EEPGD  = 0;    // запись в eeprom
   WREN   = 1;    // разрешить запись в eeprom

   GIE = 0;       // запрет прерываний

   EEIF   = 0;    //

   // обязательная последовательность
   EECON2 = 0x55;
   EECON2 = 0xAA;
   WR = 1; // запись

   WREN = 0;	  // запретить запись в eeprom
   GIE  = 1;      // разрешить прерывания
}

// Чтение байта из EEPROM памяти
// adr - адрес ячейки
unsigned char eeprom_readbyte(unsigned char adr)
{

   // проверка окончания предыдущей записи
   while (WR) { };

   EEADR = adr;   // адрес ячейки
   EEPGD = 0;     // чтение из eeprom
   RD = 1;	  // чтение
   return(EEDATA);
}



// Чтение 2 байт из FLASH памяти
// adrh, adrl - адрес ячейки
// *pdh, *pdl - указатель для сохранения данных
void flash_readbyte(unsigned char adrh, unsigned char adrl, unsigned char *pdh, unsigned char *pdl)
{

   // проверка окончания предыдущей записи
   while (WR) { };

   EEADR = adrl;   // адрес ячейки L
   EEADRH= adrh;   // адрес ячейки H
   EEPGD = 1;      // чтение из flash
   RD = 1;	   // чтение

   // обязательная последовательность
   NOP();
   NOP();

   *pdl = EEDATA;  // данные L
   *pdh = EEDATH;  // данные H
}


// Чтение 2 байт из FLASH памяти
// adr - адрес ячейки
int flash_readint(int adr)
{
   int temp;

   // проверка окончания предыдущей записи
   while (WR) { };

   EEADR = adr;        // адрес ячейки L
   EEADRH= adr >> 8;   // адрес ячейки H
   EEPGD = 1;          // чтение из flash
   RD = 1;	       // чтение

   // обязательная последовательность
   NOP();
   NOP();

   temp = EEDATH << 8;    // данные L
   temp = temp | EEDATA;  // данные H

   return(temp);
}


// Запись 2 байт в FLASH память
// adrh, adrl - адрес ячейки
// dh, dl - данные
void flash_writebyte(unsigned char adrh, unsigned char adrl, unsigned char dh, unsigned char dl)
{

   // проверка окончания предыдущей записи
   while (WR) { };

   EEADR  = adrl;   // адрес ячейки L
   EEADRH = adrh;   // адрес ячейки H
   EEDATA = dl;     // данные L
   EEDATH = dh;     // данные H

   GIE = 0;       // запрет прерываний

   EEPGD  = 1;    // запись в flash
   WREN   = 1;    // разрешить запись в flash
   EEIF   = 0;    //

   // обязательная последовательность
   EECON2 = 0x55;
   EECON2 = 0xAA;
   WR = 1; // запись

   NOP();
   NOP();

   WREN = 0;	  // запретить запись в flash
   GIE  = 1;      // разрешить прерывания
}


// Запись 2 байт в FLASH память
// adr - адрес ячейки
// d - данные
void flash_writeint(int adr, int d)
{

   // проверка окончания предыдущей записи
   while (WR) { };

   EEADR  = adr;      // адрес ячейки L
   EEADRH = adr >> 8; // адрес ячейки H
   EEDATA = d;        // данные L
   EEDATH = d >> 8;   // данные H

   GIE = 0;       // запрет прерываний

   EEPGD  = 1;    // запись в flash
   WREN   = 1;    // разрешить запись в flash
   EEIF   = 0;    //

   // обязательная последовательность
   EECON2 = 0x55;
   EECON2 = 0xaa;
   WR = 1; // запись

   NOP();
   NOP();

   WREN = 0;	  // запретить запись в flash
   GIE  = 1;      // разрешить прерывания
}


// END of FILE
