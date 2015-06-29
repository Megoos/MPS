/*
---------------------------------------------------------------------
   SPI протокол (ВЕДУЩИЙ)
   Программная эмуляция
---------------------------------------------------------------------
*/


#include "spi_soft.h"

// инициализация SPI
void _spi_init(void)
{
   // первоначаотное положение линий SPI
   SPI_SS       = 1;
   TRIS_SPI_SS  = 0; // выход - Chip Select
   SPI_SCK      = 0;
   TRIS_SPI_SCK = 0; // выход - SCLK
   SPI_SDI      = 0;
   TRIS_SPI_SDI = 0; // выход - MOSI
   SPI_SDO      = 1;
   TRIS_SPI_SDO = 1; // вход  - MISO
}


// СТАРТ последовательность
void _spi_start(void)
{
   SPI_SCK = 0; // SPI_MODE = 0
   SPI_SS  = 0; // Chip Select - Enable
}


// СТОП последовательность
void _spi_stop(void)
{
   SPI_SS  = 1; // Chip Select - Disable
   SPI_SCK = 0; // SPI_MODE = 0
}


// ПОСЛАТЬ байт
void _spi_sendbyte(unsigned char d)
{
   unsigned char i;

   // отправить 1 байт
   for(i=0; i<8; i++)
   {
      CLRWDT();

      // проверить старш бит = 1
      if (d & MLsmask)
      {
         SPI_SDI = 1; // передать 1
      }
      else
      {
         SPI_SDI = 0; // передать 0
      }

      SPI_SCK = 1; // синхроимпульс
      NOP();
      //NOP();
      //NOP();
      SPI_SCK = 0; // синхроимпульс

      // сдвиг для передачи след бита
      #if (MLs)
         d <<= 1;
      #else
         d >>= 1;
      #endif    
   } // for(i=0; i<8; i++)
}


// ПРОЧИТАТЬ байт
unsigned char _spi_readbyte(void)
{
   unsigned char i, spiReadData=0;

   for(i=0; i<8; i++)
   {
      CLRWDT();

      // сдвиг для передачи след бита
      #if (MLr)
         spiReadData <<= 1;
      #else
         spiReadData >>= 1;
      #endif

      SPI_SCK = 1; // синхроимпульс
      NOP();
      if (SPI_SDO == 1) { spiReadData |= MLrmask; } // читаем бит
      //NOP();
      //NOP();
      SPI_SCK = 0; // синхроимпульс

   } // for(i=0; i<8; i++)

   return spiReadData;
}


// END of FILE
