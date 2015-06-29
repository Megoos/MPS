#define ATmega

#ifdef ATmega
	#include <avr/io.h>
	#include <util/delay.h>
	#include <avr/pgmspace.h>
	
#else
	#include <16F877A.h>
	#use delay(clock=1000000)		
	#define _delay_us delay_us
	#define _delay_ms delay_ms
#endif
/////////////

#include "LCD.h"


//crossplatform
#define 	pSET_E()		do{set_port(get_port()|0b00000100);}while(0)
#define 	pCLR_E()		do{set_port(get_port()&0b11111011);}while(0)
#define 	pSET_RS()		do{set_port(get_port()|0b00000001);}while(0)
#define 	pCLR_RS()		do{set_port(get_port()&0b11111110);}while(0)
#define 	pSET_RW()		do{set_port(get_port()|0b00000010);}while(0)
#define 	pCLR_RW()		do{set_port(get_port()&0b11111101);}while(0)


// PA0 - R/s
// PA1 - R/W
// PA2 - E
// PA4:PA7 - DB4:DB7

struct lcd
{
	volatile uint8_t * PORT;
	volatile uint8_t * DDR;
};
struct lcd LCD;

void set_port(uint8_t in_PORT)
{  
	#ifdef ATmega
      *LCD.PORT=in_PORT;
	#else
      output_d(in_PORT);
	#endif
}

void set_ddr(uint8_t in_DDR)
{  
	#ifdef ATmega
      *LCD.DDR=in_DDR;
	#else
	 set_tris_d((~in_DDR));
	#endif
}

uint8_t get_port()
{  
   #ifdef ATmega
      return *LCD.PORT;
   	#else
      uint8_t result=0;
      result |= (input_state(PIN_D0)<<0);
      result |= (input_state(PIN_D1)<<1);
      result |= (input_state(PIN_D2)<<2);
      result |= (input_state(PIN_D3)<<3);
      result |= (input_state(PIN_D4)<<4);
      result |= (input_state(PIN_D5)<<5);
      result |= (input_state(PIN_D6)<<6);
      result |= (input_state(PIN_D7)<<7);
      return  result;
	#endif 
}



static uint8_t lcd_rus(uint8_t);

//таблица перекодировки в русские символы.
static const unsigned char convert_HD44780[64] =
{
	0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,
	0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
	0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,
	0xAC,0xE2,0xAD,0xAE,0xAD,0xAF,0xB0,0xB1,
	0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,
	0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
	0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,
	0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7
};

//CP
void pLCD_SendNibble(uint8_t Nibble)
{
	pSET_E();
	_delay_us(8);	
	set_port(get_port()&0b00001111);
	set_port(get_port()|(Nibble&0b11110000));
	pCLR_E();
	_delay_us(40);	
}

//CP
void pLCD_SendCommand(uint8_t Data)
{
	pCLR_RS();
	pLCD_SendNibble(Data);
	pLCD_SendNibble(Data << 4);
}

//CP
void LCD_Init(uint8_t * in_PORT, uint8_t * in_DDR)
{

	LCD.PORT = in_PORT;
	LCD.DDR = in_DDR;


	set_port(0x00);
	set_ddr(0xFF);

	
	pLCD_SendNibble(0b00110000);
	_delay_ms(5);
	pLCD_SendNibble(0b00110000);
	_delay_us(160);
	pLCD_SendNibble(0b00110000);
	_delay_us(160);
	pLCD_SendNibble(0b00100000);
	_delay_us(160);
	pLCD_SendCommand(0b00101000);
	_delay_us(160);
	pLCD_SendCommand(0b00001100);
	_delay_us(160);
	pLCD_SendCommand(0b00000100);
	_delay_us(160);
	pLCD_SendCommand(0b00000001);
	_delay_ms(2);
}

//CP
static uint8_t lcd_rus(uint8_t c)
{

	if  (c > 191)
	{
		c -=192;
		c= (convert_HD44780[c]);
	}

	return c;
}

//CP
void LCD_SendChar(uint8_t data)
{
	pSET_RS();
	pLCD_SendNibble(lcd_rus(data));
	pLCD_SendNibble(lcd_rus(data) << 4);
}

void LCD_SendString(char * data, uint8_t Length)
{
	for(uint8_t i = 0; i < Length; i++)
		LCD_SendChar(data[i]);
}

void LCD_GotoXY(uint8_t X, uint8_t Y)
{
	uint8_t Address = 0;
	switch(Y)
	{
		case 0:	Address = 0;break;
		case 1: Address = 0x40;	break;
		case 2: Address = 0x10; break;
		case 3: Address = 0x50; break;
	}
	Address += (X & 0x0F);

	pLCD_SendCommand(0x80 | Address);
}

void LCD_Clear(void)
{
	pLCD_SendCommand(0b00000001);
	_delay_ms(2);
}

void LCD_NewItem(void)
{
   pLCD_SendCommand(0x48);
   LCD_SendChar(0b00100111);//0
   LCD_SendChar(0b00100011);//1
   LCD_SendChar(0b00100101);//2
   LCD_SendChar(0b00100100);//3
   LCD_SendChar(0b00100100);//3
   LCD_SendChar(0b00110100);//4
   LCD_SendChar(0b00111000);//5
   LCD_SendChar(0b00111100);//6
   
}
