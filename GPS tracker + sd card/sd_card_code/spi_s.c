#include <htc.h>

__CONFIG (0x3F72);


#include "memory.h"      // ������ � EEPROM
#include "delay.h"       // ��������� ��������
#include "spi_soft.h"    // SPI ��������


// PIC16F876A
// ������� +5V, VSS
// ������� ����� - 16 ���


// ������ - AT45DB081D
// ������� +3.3V (6), VSS (7)
// (5) WP# - +3.3V
// (3) RESET# - +3.3V
// (2) SCK - SPI_SCK - (22) RB1
// (8) SO - SPI_SDO - (24) RB3
// (1) SI - SPI_SDI - (23) RB2
// (4) CS# - SPI_SS - (16) RC5
// SPI_SS (�������� � ������� � 10�)


// �������
void main(void);            // �������� ���������
unsigned char Memory_Read_Status(void);// (AT45DB081D) ������ Status ��������


// �������� ���������
void main(void)
{
   // ����������
   unsigned char MEM_status;
   unsigned char MEMbuff[5];

/*
   // �������������� ������������� ��������� �����������
   STATUS = 0b00000000;  // ��������� �����������
   OPTION = 0b00000111;  // TMR0/WDT,RB0/INT,RportB
   INTCON = 0b00000000;  // ����������
   PIE1   = 0b00000000;  // ������������ ���������� (����������)
   PIR1   = 0b00000000;  // ������������ ���������� (������)
   PIE2   = 0b00000000;
   PIR2   = 0b00000000;
   PCON   = 0b00000011;  // �������� ������ �����������
   // 3.0 EEPROM
   EEADR  = 0b00000000;
   EEADRH = 0b00000000;
   EEDATA = 0b00000000;
   EEDATH = 0b00000000;
   EECON1 = 0b00000000;
   // 4.0 I/O PORTS
   PORTA  = 0b00000000;
   PORTB  = 0b00000000;
   PORTC  = 0b00000000;
   TRISA  = 0b11111111;  // 1-Input 0-Output
   TRISB  = 0b11111111;
   TRISC  = 0b11111111;
   // 5.0 TIMER0 MODULE
   TMR0   = 0;
   // 6.0 TIMER1 MODULE
   TMR1H  = 0b00000000;
   TMR1L  = 0b00000000;
   T1CON  = 0b00000000;
   // 7.0 TIMER2 MODULE
   TMR2   = 0;
   PR2    = 0xFF;
   T2CON  = 0b00000011;
   // 8.0 CCP
   CCPR1H = 0b00000000;
   CCPR1L = 0b00000000;
   CCPR2H = 0b00000000;
   CCPR2L = 0b00000000;
   CCP1CON= 0b00000000;
   CCP2CON= 0b00000000;
   // 9.0 MSSP
   SSPSTAT= 0b00000000;
   SSPCON = 0b00000000;
   SSPCON2= 0b00000000;
   SSPADD = 0b00000000;
   SSPBUF = 0b00000000;
   // 10.0 USART
   SPBRG  = 103; // 9600, brgh=1
   TXSTA  = 0b00000110; // ��������� �����������
   RCSTA  = 0b00000000; // ��������� ���������
   // 11.0 ADC
   ADCON0 = 0b10000000; // 32Tosc 
   ADCON1 = 0b00000000; // all AN input, left
   ADRESH = 0b00000000;
   ADRESL = 0b00000000;
   // 12.0 COMPARATOR MODULE
   CMCON  = 0b00000111;
   // 13.0 COMPARATOR VOLTAGE REFERENCE MODULE
   CVRCON = 0b00000000;
*/

   //GIE = 1;


   MEM_status = 0x00;
   MEMbuff[0] = 0x00;
   MEMbuff[1] = 0x00;
   MEMbuff[2] = 0x00;
   MEMbuff[3] = 0x00;
   MEMbuff[4] = 0x00;


   // ���������
   RC2    = 0;
   TRISC2 = 0;

   // ������������� SPI
   _spi_init();


   // ������������ �������� ����� �������
   // �������� ������ � ������
   delay20_ms(20);


// 1. �������� ���������.
//    ��������� ������� ������.
//    ����� ��������� ������� ����� ���������.

   // ----------------------------------------------------
   RC2    = 1;

   // ������� ������ AT45DB081D   
   // AT45DB081D - Chip Erase
   // �������� ���� ������
   // (C7H 94H 80H 9AH)
   _spi_start();
   _spi_sendbyte(0xC7); // Command
   _spi_sendbyte(0x94); // Command
   _spi_sendbyte(0x80); // Command
   _spi_sendbyte(0x9A); // Command
   _spi_stop();
   // Delay Tce = 7-22S  (Check Status.RDY)
   do
   {
      MEM_status = Memory_Read_Status();
   } while (!(MEM_status & 0x80));  


   RC2    = 0;
   // ----------------------------------------------------

   
// 2. ���������� 5 ���� � ������.

   // ��������� �������� ������ � �����
   // AT45DB081D - Main Memory Page to Buffer Transfer
   // (53H for buffer 1 or 55H for buffer 2)
   _spi_start();
   _spi_sendbyte(0x55); // Command - Buffer 2
   _spi_sendbyte(0x0A); // Address - x x x P P P P P
   _spi_sendbyte(0x74); // Address - P P P P P P P x 
   _spi_sendbyte(0x00); // Address - x x x x x x x x 
   _spi_stop();
   // Delay Txfr = 200uS  (Check Status.RDY)
   do
   {
      MEM_status = Memory_Read_Status();
   } while (!(MEM_status & 0x80));


   // ������ � �����
   // AT45DB081D - Buffer Write
   // (84H for buffer 1 or 87H for buffer 2)
   _spi_start();
   _spi_sendbyte(0x87); // Command - Write Buffer 2
   _spi_sendbyte(0x00); // Address - x x x x x x x x
   _spi_sendbyte(0x00); // Address - x x x x x x x B
   _spi_sendbyte(0x0E); // Address - B B B B B B B B
   _spi_sendbyte(0x00); // ??? 1 Dummy Byte ???
   _spi_sendbyte(0x12); // ������ ����
   _spi_sendbyte(0x23); // ������ ����
   _spi_sendbyte(0x34); // ������ ����
   _spi_sendbyte(0x45); // ��������� ����
   _spi_sendbyte(0x56); // ����� ����
   _spi_stop();


   // ��������� ����� � �������� ������ � ���������� ���������
   // AT45DB081D - Buffer to Main Memory Page Program with Built-in Erase
   // (83H for buffer 1 or 86H for buffer 2)
   _spi_start();
   _spi_sendbyte(0x86); // Command - Buffer 2
   _spi_sendbyte(0x0A); // Address - x x x P P P P P
   _spi_sendbyte(0x74); // Address - P P P P P P P x 
   _spi_sendbyte(0x00); // Address - x x x x x x x x 
   _spi_stop();
   // Delay Tep = 14-35mS  (Check Status.RDY)
   do
   {
      MEM_status = Memory_Read_Status();
   } while (!(MEM_status & 0x80));
   // ----------------------------------------------------


// 3. ������ 5 ���� �� ������.

   // ��������� �������� ������ � �����
   // AT45DB081D - Main Memory Page to Buffer Transfer
   // (53H for buffer 1 or 55H for buffer 2)
   _spi_start();
   _spi_sendbyte(0x53); // Command - Buffer 1
   _spi_sendbyte(0x0A); // Address - x x x P P P P P
   _spi_sendbyte(0x74); // Address - P P P P P P P x 
   _spi_sendbyte(0x00); // Address - x x x x x x x x 
   _spi_stop();
   // Delay Txfr = 200uS  (Check Status.RDY)
   do
   {
      MEM_status = Memory_Read_Status();
   } while (!(MEM_status & 0x80));


   // ������ �� ������
   // AT45DB081D - Buffer Read
   // (D1H for buffer 1 or D2H for buffer 2)
   _spi_start();
   _spi_sendbyte(0xD1); // Command - Read Buffer 1
   _spi_sendbyte(0x00); // Address - x x x x x x x x
   _spi_sendbyte(0x00); // Address - x x x x x x x B 
   _spi_sendbyte(0x0E); // Address - B B B B B B B B 
   _spi_sendbyte(0x00); // 1 Dummy Byte
   MEMbuff[0] = _spi_readbyte(); // ������ ����
   MEMbuff[1] = _spi_readbyte(); // ������ ����
   MEMbuff[2] = _spi_readbyte(); // ������ ����
   MEMbuff[3] = _spi_readbyte(); // ��������� ����
   MEMbuff[4] = _spi_readbyte(); // ����� ����
   _spi_stop();

   // ----------------------------------------------------


// 4. ��������� ������ ������������ � EEPROM
//    ������ �����������.
//    ��������� �������� ������.

   eeprom_writebyte(0x00, MEMbuff[0]);
   eeprom_writebyte(0x01, MEMbuff[1]);
   eeprom_writebyte(0x02, MEMbuff[2]);
   eeprom_writebyte(0x03, MEMbuff[3]);
   eeprom_writebyte(0x04, MEMbuff[4]);


   // ������ ����
   while(1)
   {
      // �������
      RC2 = 1;
      delay20_ms(250);
      RC2 = 0;
      delay20_ms(250);
   }
}


// (AT45DB081D) ������ Status ��������
unsigned char Memory_Read_Status(void)
{
   unsigned char temp;

   _spi_start();
   _spi_sendbyte(0xD7);    // Command - Status Register Read
   temp = _spi_readbyte(); // ������ Status ��������
   _spi_stop();

   return temp;
}
