#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LCD.h"

#define ATmega

//led
#define		LCD_PORT	(PORTA)
#define		LCD_DDR		(DDRA)

#define DDR_CON (DDRC)
#define PORT_CON (PORTC)

//int
#define DATA_PORT PORTD
#define DATA_DDR DDRD
#define DATA_PIN PIND

#define CONTROL_PORT PORTB
#define CONTROL_DDR DDRB
#define CONTROL_PIN PINB
#define LINE PIN3
#define STROB PIN2



void Init() {
	DATA_DDR = 0x00;
	DATA_PORT = 0xFF;
	//CONTROL_DDR	&= ((1 << LINE) | ~(1 << STROB));

	CONTROL_DDR = 0b11111011;

	
	CONTROL_PORT = 0xFF; // 11   //01 = 04
	
}


char ex[16];
uint8_t k = 0;
uint8_t fl = 0;
char* help= "                ";
			 
ISR(INT2_vect) {
	
	_delay_ms(2);
	CONTROL_PORT = 0b00000100;
 			
		if(k == 0) {
			LCD_GotoXY(0, 2);
			LCD_SendString("                ", 16);

			LCD_GotoXY(0, 3);
			LCD_SendString("                ", 16);

			_delay_ms(5);
		} else {_delay_ms(9);}

 		help[k] = '*';
		ex[k] = DATA_PIN;
		++k;
				
		LCD_GotoXY(0, 1);
		LCD_SendString(help, 16);

		CONTROL_PORT = 0xFF;
		if (k == 16) { 
			while (fl<16){ help[fl] = ' '; fl++;}
			fl = 0;
			k = 0;
			LCD_GotoXY(0, 2);
			LCD_SendString(ex, 16);

			LCD_GotoXY(0, 3);
			LCD_SendString("Получил!        ", 16);
		}	
		
}

int main() {

	LCD_PORT = 0xFF;	//Порт А как выход. Исходное состояние 0хFF
	LCD_DDR = 0xFF;
	Init();
	LCD_Init(&LCD_PORT,&LCD_DDR);
	
	GICR = (1 << INT2);//	GICR = (1 << INT2);
	MCUCSR = (0<<ISC2); //MCUCR=0x00;
//	MCUCSR=0x40;
//	GIFR=0x20;


	GICR = (1<<INT2);
	MCUCR = (0<<ISC2);
	char* ready = "Готов!          ";
	LCD_GotoXY(0, 0);
	LCD_SendString(ready, 16);
		
	sei();
	while (1);	return 0;
}

