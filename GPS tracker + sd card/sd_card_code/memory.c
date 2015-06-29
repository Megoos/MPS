/*
---------------------------------------------------------------------
   ������ � EEPROM � FLASH ������� �����������
---------------------------------------------------------------------
*/


#include "memory.h"


// ������ ���������� ���� � EEPROM ������
// adr - ����� ��������� ������
// *pbuff - ��������� �� ����� � �������
// num_vals - ���-�� ���� ��� ������
void eeprom_writebuff(unsigned char adr, unsigned char *pbuff, unsigned char num_vals)
{
   unsigned char i;
   for(i=0; i<num_vals; i++, adr++)
   {
      eeprom_writebyte(adr, pbuff[i]);
   }
}


// ������ ���������� ���� �� EEPROM ������
// adr - ����� ��������� ������
// *pbuff - ��������� �� ����� � �������
// num_vals - ���-�� ���� ��� ������
void eeprom_readbuff(unsigned char adr, unsigned char *pbuff, unsigned char num_vals)
{
   unsigned char i;
   for(i=0; i<num_vals; i++, adr++)
   {
      pbuff[i] = eeprom_readbyte(adr);
   }
}


// ������ ����� � EEPROM ������
// adr - ����� ������
// d - ������
void eeprom_writebyte(unsigned char adr, unsigned char d)
{

   // �������� ��������� ���������� ������
   while (WR) { };

   EEADR  = adr;  // ����� ������
   EEDATA = d;    // ������

   EEPGD  = 0;    // ������ � eeprom
   WREN   = 1;    // ��������� ������ � eeprom

   GIE = 0;       // ������ ����������

   EEIF   = 0;    //

   // ������������ ������������������
   EECON2 = 0x55;
   EECON2 = 0xAA;
   WR = 1; // ������

   WREN = 0;	  // ��������� ������ � eeprom
   GIE  = 1;      // ��������� ����������
}

// ������ ����� �� EEPROM ������
// adr - ����� ������
unsigned char eeprom_readbyte(unsigned char adr)
{

   // �������� ��������� ���������� ������
   while (WR) { };

   EEADR = adr;   // ����� ������
   EEPGD = 0;     // ������ �� eeprom
   RD = 1;	  // ������
   return(EEDATA);
}



// ������ 2 ���� �� FLASH ������
// adrh, adrl - ����� ������
// *pdh, *pdl - ��������� ��� ���������� ������
void flash_readbyte(unsigned char adrh, unsigned char adrl, unsigned char *pdh, unsigned char *pdl)
{

   // �������� ��������� ���������� ������
   while (WR) { };

   EEADR = adrl;   // ����� ������ L
   EEADRH= adrh;   // ����� ������ H
   EEPGD = 1;      // ������ �� flash
   RD = 1;	   // ������

   // ������������ ������������������
   NOP();
   NOP();

   *pdl = EEDATA;  // ������ L
   *pdh = EEDATH;  // ������ H
}


// ������ 2 ���� �� FLASH ������
// adr - ����� ������
int flash_readint(int adr)
{
   int temp;

   // �������� ��������� ���������� ������
   while (WR) { };

   EEADR = adr;        // ����� ������ L
   EEADRH= adr >> 8;   // ����� ������ H
   EEPGD = 1;          // ������ �� flash
   RD = 1;	       // ������

   // ������������ ������������������
   NOP();
   NOP();

   temp = EEDATH << 8;    // ������ L
   temp = temp | EEDATA;  // ������ H

   return(temp);
}


// ������ 2 ���� � FLASH ������
// adrh, adrl - ����� ������
// dh, dl - ������
void flash_writebyte(unsigned char adrh, unsigned char adrl, unsigned char dh, unsigned char dl)
{

   // �������� ��������� ���������� ������
   while (WR) { };

   EEADR  = adrl;   // ����� ������ L
   EEADRH = adrh;   // ����� ������ H
   EEDATA = dl;     // ������ L
   EEDATH = dh;     // ������ H

   GIE = 0;       // ������ ����������

   EEPGD  = 1;    // ������ � flash
   WREN   = 1;    // ��������� ������ � flash
   EEIF   = 0;    //

   // ������������ ������������������
   EECON2 = 0x55;
   EECON2 = 0xAA;
   WR = 1; // ������

   NOP();
   NOP();

   WREN = 0;	  // ��������� ������ � flash
   GIE  = 1;      // ��������� ����������
}


// ������ 2 ���� � FLASH ������
// adr - ����� ������
// d - ������
void flash_writeint(int adr, int d)
{

   // �������� ��������� ���������� ������
   while (WR) { };

   EEADR  = adr;      // ����� ������ L
   EEADRH = adr >> 8; // ����� ������ H
   EEDATA = d;        // ������ L
   EEDATH = d >> 8;   // ������ H

   GIE = 0;       // ������ ����������

   EEPGD  = 1;    // ������ � flash
   WREN   = 1;    // ��������� ������ � flash
   EEIF   = 0;    //

   // ������������ ������������������
   EECON2 = 0x55;
   EECON2 = 0xaa;
   WR = 1; // ������

   NOP();
   NOP();

   WREN = 0;	  // ��������� ������ � flash
   GIE  = 1;      // ��������� ����������
}


// END of FILE
