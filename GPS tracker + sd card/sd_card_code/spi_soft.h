/*
---------------------------------------------------------------------
   SPI протокол (ВЕДУЩИЙ)
   Программная эмуляция
---------------------------------------------------------------------
*/


#include <htc.h>


// назначение выводов порта
#define SPI_SCK RB1 // выход - SCLK
#define SPI_SDI RB2 // выход - MOSI
#define SPI_SDO RB3 // вход  - MISO
#define SPI_SS  RC5 // выход - Chip Select

// настройка выводов порта
#define TRIS_SPI_SCK TRISB1 // выход - SCLK
#define TRIS_SPI_SDI TRISB2 // выход - MOSI
#define TRIS_SPI_SDO TRISB3 // вход  - MISO
#define TRIS_SPI_SS  TRISC5 // выход - Chip Select

// выводы портов должны быть сконфигурированы на вход/выход
// в соответствии с направлением данных (выход=0, вход=1)

// некоторые выводы контроллера не могут сформировать "1"
// поэтому м необходима подтяжка внешним резистором к питанию

// режим SPI -  SPI_MODE = 0


// Старший/Младший бит ПЕРЕДАЕТСЯ первым
// 1 - первым передается старший бит - MSb
// 0 - первым передается младший бит - LSb
#define MLs 1

// маска для отправки MSb|LSb
#if (MLs == 1)
   #define MLsmask 0x80 // отправка
#else
   #define MLsmask 0x01
#endif


// Старший/Младший бит ПРИНИМАЕТСЯ первым
// 1 - первым принимается старший бит - MSb
// 0 - первым принимается младший бит - LSb
#define MLr 1

// маска для приема MSb|LSb
#if (MLr == 1)
   #define MLrmask 0x01 // прием
#else
   #define MLrmask 0x80
#endif


// инициализация SPI
extern void _spi_init(void);

// СТАРТ последовательность
extern void _spi_start(void);

// СТОП последовательность
extern void _spi_stop(void);

// ПОСЛАТЬ байт
extern void _spi_sendbyte(unsigned char d);

// ПРОЧИТАТЬ байт
extern unsigned char _spi_readbyte(void);


// END of FILE
