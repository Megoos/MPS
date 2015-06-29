#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

//// LCD ////
#define pLCD_PORT PORTA
#define pLCD_DDR DDRA

#define pSET_RS() do{pLCD_PORT |= (1 << 0);} while(0)
#define pCLR_RS() do{pLCD_PORT &= ~(1 << 0);}while(0)
#define pSET_RW() do{pLCD_PORT |= (1 << 1);} while(0)
#define pCLR_RW() do{pLCD_PORT &= ~(1 << 1);}while(0)
#define pSET_E() do{ pLCD_PORT |= (1 << 2);} while(0)
#define pCLR_E() do{ pLCD_PORT &= ~(1 << 2);}while(0)

void pLCD_SendNibble(uint8_t nibble) {
	pSET_E();
	_delay_us(8);
	pLCD_PORT &= 0x0F;
	pLCD_PORT |= (nibble & 0xF0);
	pCLR_E();
	_delay_us(40);
}

void pLCD_SendCommand(uint8_t data) {
	pCLR_RS();
	pLCD_SendNibble(data);
	pLCD_SendNibble(data << 4);
}

void pLCD_SendData(uint8_t data) {
	pSET_RS();
	pLCD_SendNibble(data);
	pLCD_SendNibble(data << 4);
}

uint8_t pLCD_Rus(const uint8_t data) {
	char ans = (char)data;

	switch (ans) {
		case 'А': ans = 'A'; break;
		case 'Б': ans = 0xA0; break;
		case 'В': ans = 'B'; break;
		case 'Г': ans = 0xA1; break;
		case 'Д': ans = 0xE0; break;
		case 'Е': ans = 'E'; break;
		case 'Ё': ans = 0xA2; break;
		case 'Ж': ans = 0xA3; break;
		case 'З': ans = 0xA4; break;
		case 'И': ans = 0xA5; break;
		case 'Й': ans = 0xA6; break;
		case 'К': ans = 'K'; break;
		case 'Л': ans = 0xA7; break;
		case 'М': ans = 'M'; break;
		case 'Н': ans = 'H'; break;
		case 'О': ans = 'O'; break;
		case 'П': ans = 0xA8; break;
		case 'Р': ans = 'P'; break;
		case 'С': ans = 'C'; break;
		case 'Т': ans = 'T'; break;
		case 'У': ans = 0xA9; break;
		case 'Ф': ans = 0xAA; break;
		case 'Х': ans = 'X'; break;
		case 'Ц': ans = 0xE1; break;
		case 'Ч': ans = 0xAB; break;
		case 'Ш': ans = 0xAC; break;
		case 'Щ': ans = 0xE2; break;
		case 'Ъ': ans = 0xAD; break;
		case 'Ы': ans = 0xAE; break;
		case 'Ь': ans = 0x08; break;
		case 'Э': ans = 0xAF; break;
		case 'Ю': ans = 0xB0; break;
		case 'Я': ans = 0xB1; break;
		//
		case 'а': ans = 0x61; break;
		case 'б': ans = 0xB2; break;
		case 'в': ans = 0xB3; break;
		case 'г': ans = 0xB4; break;
		case 'д': ans = 0xE3; break;
		case 'е': ans = 'e'; break;
		case 'ё': ans = 0xB5; break;
		case 'ж': ans = 0xB6; break;
		case 'з': ans = 0xB7; break;
		case 'и': ans = 0xB8; break;
		case 'й': ans = 0xB9; break;
		case 'к': ans = 0xBA; break;
		case 'л': ans = 0xBB; break;
		case 'м': ans = 0xBC; break;
		case 'н': ans = 0xBD; break;
		case 'о': ans = 'o'; break;
		case 'п': ans = 0xBE; break;
		case 'р': ans = 'p'; break;
		case 'с': ans = 'c'; break;
		case 'т': ans = 0xBF; break;
		case 'у': ans = 'y'; break;
		case 'ф': ans = 0xE4; break;
		case 'х': ans = 'x'; break;
		case 'ц': ans = 0xE5; break;
		case 'ч': ans = 0xC0; break;
		case 'ш': ans = 0xC1; break;
		case 'щ': ans = 0xE6; break;
		case 'ъ': ans = 0xC2; break;
		case 'ы': ans = 0xC3; break;
		case 'ь': ans = 0xC4; break;
		case 'э': ans = 0xC5; break;
		case 'ю': ans = 0xC6; break;
		case 'я': ans = 0xC7; break;
	}

	return (uint8_t)ans;
}

void LCD_Init() {
	pLCD_DDR |= 0b11110111;
	pLCD_PORT &= 0b00001000;

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

void LCD_SendString(char* data, uint8_t length) {
	uint8_t i;
	for (i = 0; i < length; i++)
		pLCD_SendData(pLCD_Rus(data[i]));
}

void LCD_GoToXY(uint8_t X, uint8_t Y) {
	uint8_t Address = 0;
	switch (Y) {
		case 0:	Address = 0; break;
		case 1: Address = 0x40;	break;
		case 2: Address = 0x10; break;
		case 3: Address = 0x50; break;
	}
	Address += (X & 0x0F);
	pLCD_SendCommand(0x80 | Address);
}

void LCD_Clear(void) {
	pLCD_SendCommand(0b00000001);
	_delay_ms(2);
}

uint8_t help = 128;


//// END LCD ////

//// SPI ////
#define DDR_SPI DDRB
#define PORT_SPI PORTB

#define SPI_SS DDB4
#define SPI_MOSI DDB5
#define SPI_MISO DDB6
#define SPI_SCK DDB7

void SPI_MasterInit() {
	
	DDR_SPI |= ( 1 << SPI_MOSI) | (1 << SPI_SCK) | ( 1 << SPI_SS);
	DDR_SPI &= ~(1 << SPI_MISO);
	SPSR = (1 << SPIF); // interrupt enable
	SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (0 << CPOL) | (0 << SPI2X)| (1 << SPR1) | (1 << SPR0);
	// SPR1 SPR0 SCK_Freq
	//   0    0    /4
    //   0    1    /16
	//	 1    0    /64
	//	 1    1    /128

	//PORT_SPI |= ( 1 << SPI_SS ); 
	DDRD |= 0b0000001;
	PORTD |= 0b0000001;
}

void SPI_MasterTransmit(char data) {
	//PORT_SPI &= ~(1 << SPI_SS );
	PORTD &= 0b11111110;
	_delay_ms(1);
	SPDR = data;
	while( !(SPSR & (1<<SPIF)) ) {}
	//PORT_SPI |= ( 1 << SPI_SS ); 
	_delay_us(100);
	PORTD |= 0b00000001;
}

char SPI_MasterRecieve() {
	//PORT_SPI &= ~(1 << SPI_SS );
	PORTD &= 0b11111110;
	SPDR = 0x55;
	while( !(SPSR & (1<<SPIF)) ) {}
	//PORT_SPI |= ( 1 << SPI_SS );
	//_delay_us(100); //me
	PORTD |= 0b00000001;
	return SPDR;
}

//// ENS SPI //// 

char* buf = "                ";
ISR(INT0_vect) { //D2
	//here();
	SPI_MasterTransmit(0x72);
	
	_delay_ms(10);


	
	uint8_t i;
	for (i = 0; i < 12; ++i) {
		buf[i] = SPI_MasterRecieve();
		_delay_ms(10);
	}

	/*uint8_t l = 16;
	LCD_GoToXY(0, 1);
	buf[0] = ' ';
	LCD_SendString(buf, l);
	*/
	
}

uint8_t kol = 0;

ISR(SPI_STC_vect) {
	
	uint8_t l = 16;
	LCD_GoToXY(0, 1);
	buf[0] = ' ';
	LCD_SendString(buf, l);
	
}


int main() {	
	LCD_Init();

	SPI_MasterInit();	

	DDRD &= 0b11111011;
	PORTD |= 0b00000101;
	GICR = (1 << INT0); // Enable Int0 iterruptd
	MCUCR |= (1 << ISC01) | (1 << ISC00);
	

	char* text = "Последней была: ";
	uint8_t l = 16;
	LCD_GoToXY(0, 0);
	LCD_SendString(text, l);

	sei();


	while(1) {

	}
}
