/*
---------------------------------------------------------------------
   SPI �������� (�������)
   ����������� ��������
---------------------------------------------------------------------
*/


#include <htc.h>


// ���������� ������� �����
#define SPI_SCK RB1 // ����� - SCLK
#define SPI_SDI RB2 // ����� - MOSI
#define SPI_SDO RB3 // ����  - MISO
#define SPI_SS  RC5 // ����� - Chip Select

// ��������� ������� �����
#define TRIS_SPI_SCK TRISB1 // ����� - SCLK
#define TRIS_SPI_SDI TRISB2 // ����� - MOSI
#define TRIS_SPI_SDO TRISB3 // ����  - MISO
#define TRIS_SPI_SS  TRISC5 // ����� - Chip Select

// ������ ������ ������ ���� ���������������� �� ����/�����
// � ������������ � ������������ ������ (�����=0, ����=1)

// ��������� ������ ����������� �� ����� ������������ "1"
// ������� � ���������� �������� ������� ���������� � �������

// ����� SPI -  SPI_MODE = 0


// �������/������� ��� ���������� ������
// 1 - ������ ���������� ������� ��� - MSb
// 0 - ������ ���������� ������� ��� - LSb
#define MLs 1

// ����� ��� �������� MSb|LSb
#if (MLs == 1)
   #define MLsmask 0x80 // ��������
#else
   #define MLsmask 0x01
#endif


// �������/������� ��� ����������� ������
// 1 - ������ ����������� ������� ��� - MSb
// 0 - ������ ����������� ������� ��� - LSb
#define MLr 1

// ����� ��� ������ MSb|LSb
#if (MLr == 1)
   #define MLrmask 0x01 // �����
#else
   #define MLrmask 0x80
#endif


// ������������� SPI
extern void _spi_init(void);

// ����� ������������������
extern void _spi_start(void);

// ���� ������������������
extern void _spi_stop(void);

// ������� ����
extern void _spi_sendbyte(unsigned char d);

// ��������� ����
extern unsigned char _spi_readbyte(void);


// END of FILE
