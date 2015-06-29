/*
---------------------------------------------------------------------
   Работа с EEPROM и FLASH памятью контроллера
---------------------------------------------------------------------
*/


#include <htc.h>


// Запись нескольких байта в EEPROM память
// adr - адрес начальной ячейки
// *pbuff - указатель на буфер с данными
// num_vals - кол-во байт для записи
extern void eeprom_writebuff(unsigned char adr, unsigned char *buff, unsigned char num_vals);


// Чтение нескольких байта из EEPROM памяти
// adr - адрес начальной ячейки
// *pbuff - указатель на буфер с данными
// num_vals - кол-во байт для чтения
extern void eeprom_readbuff(unsigned char adr, unsigned char *buff, unsigned char num_vals);


// Запись байта в EEPROM память
// adr - адрес ячейки
// d - данные
extern void eeprom_writebyte(unsigned char adr, unsigned char d);


// Чтение байта из EEPROM памяти
// adr - адрес ячейки
extern unsigned char eeprom_readbyte(unsigned char adr);



// Чтение 2 байт из FLASH памяти
// adrh, adrl - адрес ячейки
// *pdh, *pdl - указатель для сохранения данных
void flash_readbyte(unsigned char adrh, unsigned char adrl, unsigned char *dh, unsigned char *dl);


// Чтение 2 байт из FLASH памяти
// adr - адрес ячейки
int flash_readint(int adr);


// Запись 2 байт в FLASH память
// adrh, adrl - адрес ячейки
// dh, dl - данные
void flash_writebyte(unsigned char adrh, unsigned char adrl, unsigned char dh, unsigned char dl);


// Запись 2 байт в FLASH память
// adr - адрес ячейки
// d - данные
void flash_writeint(int adr, int d);


// END of FILE
