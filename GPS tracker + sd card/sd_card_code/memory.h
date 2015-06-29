/*
---------------------------------------------------------------------
   ������ � EEPROM � FLASH ������� �����������
---------------------------------------------------------------------
*/


#include <htc.h>


// ������ ���������� ����� � EEPROM ������
// adr - ����� ��������� ������
// *pbuff - ��������� �� ����� � �������
// num_vals - ���-�� ���� ��� ������
extern void eeprom_writebuff(unsigned char adr, unsigned char *buff, unsigned char num_vals);


// ������ ���������� ����� �� EEPROM ������
// adr - ����� ��������� ������
// *pbuff - ��������� �� ����� � �������
// num_vals - ���-�� ���� ��� ������
extern void eeprom_readbuff(unsigned char adr, unsigned char *buff, unsigned char num_vals);


// ������ ����� � EEPROM ������
// adr - ����� ������
// d - ������
extern void eeprom_writebyte(unsigned char adr, unsigned char d);


// ������ ����� �� EEPROM ������
// adr - ����� ������
extern unsigned char eeprom_readbyte(unsigned char adr);



// ������ 2 ���� �� FLASH ������
// adrh, adrl - ����� ������
// *pdh, *pdl - ��������� ��� ���������� ������
void flash_readbyte(unsigned char adrh, unsigned char adrl, unsigned char *dh, unsigned char *dl);


// ������ 2 ���� �� FLASH ������
// adr - ����� ������
int flash_readint(int adr);


// ������ 2 ���� � FLASH ������
// adrh, adrl - ����� ������
// dh, dl - ������
void flash_writebyte(unsigned char adrh, unsigned char adrl, unsigned char dh, unsigned char dl);


// ������ 2 ���� � FLASH ������
// adr - ����� ������
// d - ������
void flash_writeint(int adr, int d);


// END of FILE
