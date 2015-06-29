/*
---------------------------------------------------------------------
   SPI �������� (�������)
   ����������� ��������
---------------------------------------------------------------------
*/


#include "spi_soft.h"

// ������������� SPI
void _spi_init(void)
{
   // �������������� ��������� ����� SPI
   SPI_SS       = 1;
   TRIS_SPI_SS  = 0; // ����� - Chip Select
   SPI_SCK      = 0;
   TRIS_SPI_SCK = 0; // ����� - SCLK
   SPI_SDI      = 0;
   TRIS_SPI_SDI = 0; // ����� - MOSI
   SPI_SDO      = 1;
   TRIS_SPI_SDO = 1; // ����  - MISO
}


// ����� ������������������
void _spi_start(void)
{
   SPI_SCK = 0; // SPI_MODE = 0
   SPI_SS  = 0; // Chip Select - Enable
}


// ���� ������������������
void _spi_stop(void)
{
   SPI_SS  = 1; // Chip Select - Disable
   SPI_SCK = 0; // SPI_MODE = 0
}


// ������� ����
void _spi_sendbyte(unsigned char d)
{
   unsigned char i;

   // ��������� 1 ����
   for(i=0; i<8; i++)
   {
      CLRWDT();

      // ��������� ����� ��� = 1
      if (d & MLsmask)
      {
         SPI_SDI = 1; // �������� 1
      }
      else
      {
         SPI_SDI = 0; // �������� 0
      }

      SPI_SCK = 1; // �������������
      NOP();
      //NOP();
      //NOP();
      SPI_SCK = 0; // �������������

      // ����� ��� �������� ���� ����
      #if (MLs)
         d <<= 1;
      #else
         d >>= 1;
      #endif    
   } // for(i=0; i<8; i++)
}


// ��������� ����
unsigned char _spi_readbyte(void)
{
   unsigned char i, spiReadData=0;

   for(i=0; i<8; i++)
   {
      CLRWDT();

      // ����� ��� �������� ���� ����
      #if (MLr)
         spiReadData <<= 1;
      #else
         spiReadData >>= 1;
      #endif

      SPI_SCK = 1; // �������������
      NOP();
      if (SPI_SDO == 1) { spiReadData |= MLrmask; } // ������ ���
      //NOP();
      //NOP();
      SPI_SCK = 0; // �������������

   } // for(i=0; i<8; i++)

   return spiReadData;
}


// END of FILE
