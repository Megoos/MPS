#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>


#define		pLCD_PORT		(PORTA)
#define		pLCD_DDR		(DDRA)
// PA0 - R/s
// PA1 - R/W
// PA2 - E
// PA4:PA7 - DB4:DB7

#define sbi(x)  (1<<x)
#define cbi(x)  (~(1<<x))

#define 	pSET_E()	do{pLCD_PORT |= (1 << 2); }while(0)
#define 	pCLR_E()	do{pLCD_PORT &= ~(1 << 2); }while(0)
#define 	pSET_RS()	do{pLCD_PORT |= (1 << 0); }while(0)
#define 	pCLR_RS()	do{pLCD_PORT &= ~(1 << 0); }while(0)
#define 	pSET_RW()	do{pLCD_PORT |= (1 << 1); }while(0)
#define 	pCLR_RW()	do{pLCD_PORT &= ~(1 << 1); }while(0)


#define Display_Port PORTD
#define Display_DDR DDRD
#define Display_Pin PIND

#define Cathode_Port PORTB
#define Cathode_DDR DDRB
#define Cathode_Pin PINB

#define Button_Port PORTB
#define Button_DDR DDRB
#define Button_Pin PINB

#define Button1 6
#define Button2 7

char time[16];


unsigned char hour, minute, second;
unsigned char count = 2;

unsigned char digit0;
unsigned char digit1;
unsigned char digit2;
unsigned char digit3;


void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);

unsigned char i2c_write_byte(unsigned char data);
unsigned char i2c_read_byte(unsigned char *data, unsigned char ack);
unsigned char ds1307_read(unsigned char address, unsigned char *data);
unsigned char ds1307_write(unsigned char address, unsigned char data);


void LCD_Init(void);
void LCD_SendChar(uint8_t data);
void LCD_SendString(char * data, uint8_t Length);
void LCD_GotoXY(uint8_t X, uint8_t Y);
void LCD_Clear(void);
void LCD_NewItem(void);
void pLCD_SendCommand(uint8_t);
void LCD_SetClock(void);
void LCD_ReadTime(void);
void LCD_ShowTime(void);
void Segment_ShowTime(void);


void LCD_ReadTime(void)
{
	unsigned char temp;

	ds1307_read(0x00, &temp);
	second = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);

	ds1307_read(0x01, &temp);
	minute = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);

	ds1307_read(0x02, &temp);
	hour = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
}

void LCD_ShowTime(void)
{
	LCD_GotoXY(0,0);
	LCD_SendString(" It's showtime! ", 16);
	LCD_GotoXY(0,1);
	sprintf (time, "    %02d:%02d:%02d    ", hour, minute, second);
	LCD_SendString(time, 16);
}

void Segment_ShowTime(void)
{
	
    
	//Cathode_Port = 0b11000011;
}



void LCD_SetClock(void)
{
	
	if((Button_Pin & (1 << Button1))==0) // Если нажата кнопка
	{
		while((Button_Pin & (1 << Button1))==0){} // Ждем отпускания кнопки
		hour++; // Увеличиваем часы на 1
		if(hour > 23) hour = 0;
		// Преобразуем из двоичной системы в BCD и записываем в DS1307
		unsigned char temp;
		temp = ((hour/10) << 4)|(hour%10);
		ds1307_write(0x02, temp);
		_delay_ms(100);
	}

	if((Button_Pin & (1 << Button2))==0) // Если нажата кнопка
	{
		while((Button_Pin & (1 << Button2))==0){} // Ждем отпускания кнопки
		minute++; // Увеличиваем минуты на 1
		if(minute > 59) minute = 0;
		// Преобразуем из двоичной системы в BCD и записываем в DS1307
		unsigned char temp;
		temp = ((minute/10) << 4)|(minute%10);
		ds1307_write(0x01, temp);
		_delay_ms(100);
	}
}


void pLCD_SendNibble(uint8_t Nibble)
{
	pSET_E();
	_delay_us(8);	// 8us
	pLCD_PORT &= 0x0F;
	pLCD_PORT |= (Nibble & 0xF0);
	pCLR_E();
	_delay_us(40);	// 40 us
}

void pLCD_SendCommand(uint8_t Data)
{
	pCLR_RS();
	pLCD_SendNibble(Data);
	pLCD_SendNibble(Data << 4);
}


void LCD_Init(void)
{
	pLCD_DDR = 0b11111111;
	pLCD_PORT = 0;
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

void LCD_SendChar(uint8_t data)
{
	pSET_RS();
	pLCD_SendNibble(data);
	pLCD_SendNibble(data << 4);
}

void LCD_SendString(char * data, uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length; i++)
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
	LCD_SendChar(0b00111111);//0
	LCD_SendChar(0b00000000);//1
	LCD_SendChar(0b00000000);//2
	LCD_SendChar(0b00000000);//3
	LCD_SendChar(0b00000000);//3
	LCD_SendChar(0b00000000);//4
	LCD_SendChar(0b00000000);//5
	LCD_SendChar(0b00000000);//6
	
}


void i2c_init(void)
{
	
	TWBR = 2;
	TWSR = (1 << TWPS1)|(1 << TWPS0); 
	TWCR |= (1 << TWEN); 
}

void i2c_start(void)
{
	
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTA);
	//while(!(TWCR & (1 << TWINT)));
	_delay_ms(2);
}

void i2c_stop(void)
{
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO); 
	//while(TWCR & (1 << TWSTO)); 
	_delay_ms(2);
}


unsigned char i2c_write_byte(unsigned char data)
{
	TWDR = data; 
	TWCR = (1 << TWEN)|(1 << TWINT); 
	//while(!(TWCR & (1 << TWINT)));
	_delay_ms(2);
	if((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x28 || (TWSR & 0xF8) == 0x40)
	{
		return 1;
	}
	else
	return 0; 
}


unsigned char i2c_read_byte(unsigned char *data, unsigned char ack)
{
	if(ack)
	{
		
		TWCR |= (1 << TWEA);
	}
	else
	{
		
		TWCR &= ~(1 << TWEA);
	}

	TWCR |= (1 << TWINT);
	//while(!(TWCR & (1 << TWINT))); 
	_delay_ms(2);

	if((TWSR & 0xF8) == 0x58 || (TWSR & 0xF8) == 0x50)
	{
		
		
		*data = TWDR; 
		return 1;
	}
	else
	return 0; 
}

unsigned char ds1307_read(unsigned char address, unsigned char *data)
{
	unsigned char res; 
	i2c_start(); 
	res = i2c_write_byte(0b11010000);
	if(!res)	return 0; 

	res = i2c_write_byte(address);
	if(!res)	return 0; 
	i2c_start(); 
	res = i2c_write_byte(0b11010001);	
	if(!res)	return 0; 
	res = i2c_read_byte(data,0);
	if(!res)	return 0; 
	i2c_stop(); 
	return 1;
}

unsigned char ds1307_write(unsigned char address, unsigned char data)
{
	unsigned char res; 
	i2c_start();	
	res = i2c_write_byte(0b11010000);	
	if(!res)	return 0; 

	res = i2c_write_byte(address);
	if(!res)	return 0; 
	res = i2c_write_byte(data); 
	if(!res)	return 0; 
	i2c_stop(); 
	return 1;
}



ISR(TIMER0_COMP_vect) 
{
	Cathode_Port |= sbi(2)|sbi(3)|sbi(4)|sbi(5); 
	TCNT0 = 0; //timer count
	unsigned char number[] =
	{
		0b00111111, //0
		0b00000110, //1
		0b01011011, //2
		0b01001111, //3
		0b01100110, //4
		0b01101101, //5
		0b01111101, //6
		0b00000111, //7
		0b01111111, //8
		0b01101111  //9
	};

	count++;
	if (count == 6) {count = 0;}
	Cathode_Port &= cbi(count);
	
	
	digit0 = hour/10;
	digit1 = hour%10;
	digit2 = minute/10;
	digit3 = minute%10;
	
	
	if (count == 2) {
		Display_Port &= 0b10000000;
		Display_Port |= number[digit0]; }
	if (count == 3) {
		Display_Port &= 0b10000000;
		Display_Port |= number[digit1];}
	if (count == 4) {
		Display_Port &= 0b10000000;
		Display_Port |= number[digit2];}
	if (count == 5) {
		Display_Port &= 0b10000000;
		Display_Port |= number[digit3];}
}

ISR(TIMER2_COMP_vect)
	{
		TCNT2 = 0;
		char buf[4];
		static uint8_t temp = 0;
		static uint8_t flag = 0;
		if ((hour == 7)&&(minute<5))
		{
			temp++;
			if (temp == 50)
			{
				temp = 0;
				if (flag == 1)
				{
					Display_DDR |= sbi(7); //ножка, которой
					Display_Port &= cbi(7);//будем дергать
				}
				if (flag == 0)
				{
					Display_DDR |= sbi(7); //ножка, которой
					Display_Port |= sbi(7);//будем дергать
				}
				flag ^= (1<<0); //инвертируем флаг
			}
		}
	}


int main(void)
{
	_delay_ms(100);

	DDRA = 0x00;
	PORTA = 0xFF;

	LCD_Init();
	i2c_init();
	
	unsigned char temp;
	
	ds1307_read(0x00, &temp);
	temp &= ~(1 << 7); 
	ds1307_write(0x00,temp);
	

	Cathode_DDR |= sbi(2) | sbi(3) | sbi(4) | sbi(5); //общие катоды на выход
	
	
	Button_DDR &= cbi(6)|cbi(7);
	Button_Port |= sbi(6) | sbi(7); //подтяженька на кнопоньках 

	//LCD_NewItem();
	
	OCR0 = 25; // high value of TCNT0
	OCR2 = 50; // для дерганной ножки
	//CS - prescaler = 1024; WGM - timer working mode (reset when == OCR0)
	TCCR0 |= (0 << FOC0) | (1 << WGM01) | (0 << WGM00) | (1 << CS02) | (0 << CS01) | (1 << CS00);
	TCCR2 |= (0 << FOC0) | (1 << WGM01) | (0 << WGM00) | (1 << CS22) | (1 << CS21) | (0 << CS20); //prescaler = 256;
	TIMSK |= (1 << OCIE0) | (1<<OCIE2); // enable timer0, timer2 interrupts
	
	//WDTCR |= (1<<WDE) | (1<<WDP2) | (1<<WDP1)| (1<<WDP0); //уотч доже тимер еври 2.2с
	LCD_ReadTime();
	Display_DDR = 0xFF;
	
	sei();
	while(1)
	{
		LCD_ReadTime();
	//	LCD_ShowTime();
		LCD_SetClock();//Установка часов, если нажаты кнопки установки
	}
}