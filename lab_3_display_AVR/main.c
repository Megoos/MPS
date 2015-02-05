#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include "LCD.h"

#define ATmega

//led
#define		LCD_PORT	(PORTA)
#define		LCD_DDR		(DDRA)

// PA0 - R/s
// PA1 - R/W
// PA2 - E
// PA4:PA7 - DB4:DB7
/*
MEMO:
	real 1s is 125;
	proteus 1s is about 9; Cause it is slow;
	to see minutes in proteus - 1; BECAUSE IT CRASHES ALL THE TIME
*/
#define ARBITRARY_SECOND 125

//button
#define BUT_DDR (DDRD)
#define BUT_PORT (PORTD)
#define BUT_PIN (PIND)


volatile int H[3] = {0,0,0};
volatile int M[3] = {0,0,0};
volatile int S[3] = {0,0,0};
//вывод времени в i-ую линию
void Output(int i)
{
	char str[8];
	sprintf(str, "%02d:%02d:%02d",H[i],M[i],S[i]);
	LCD_GotoXY(8,i);
	LCD_SendString(str,8);
}

//update clock with regard to hours, minutes and seconds
void UpdateClock()
{
	S[0]++;
	if (S[0]==60)
	{
		S[0]=0;
		M[0]++;
		if (M[0]==60)
		{
			M[0]=0;
			H[0]++;
			if (M[0]==60)
			{
				H[0]=0;
			}
		}
	}
	Output(0);
}

volatile int count;
volatile int safety;
//таймер
ISR(TIMER1_COMPA_vect)
{

	if (safety!=0)
	{
		safety--;
	}

	count++;
	if(count==ARBITRARY_SECOND)
	{
		UpdateClock();
		count=0;
	}
}

//button1
ISR(INT0_vect)
{
	if (safety==0)
	{
		H[1]=H[0];
		M[1]=M[0];
		S[1]=S[0];
		Output(1);
		safety = ARBITRARY_SECOND;
	}
}
//button1
ISR(INT1_vect)
{
	if (safety==0)
	{
		H[2]=H[0];
		M[2]=M[0];
		S[2]=S[0];
		Output(2);
		safety = ARBITRARY_SECOND;
	}
}


int main()
{	
	LCD_PORT = 0xFF;			//Порт А как выход. Исходное состояние 0хFF
	LCD_DDR = 0xFF;
	
	S[0]=0;	M[0]=0;
	S[1]=0;	M[1]=0;
	S[2]=0;	M[2]=0;
	
	//initializing timer 1 to compare to 250 and set to 0 afterwards;
	TCCR1B=(1<<WGM12)|(1<<CS12);
	OCR1A=250;
	TIMSK|=(1<<OCIE1A);
	TCNT1=0;
	count=0;
	//initializing INT0 and INT1 to fire interrupts on rising edge;
	GICR = (1<<INT0) | (1<<INT1);
	MCUCR = (1<<ISC01) | (1<<ISC00) | (1<<ISC11) | (1<<ISC10);
	safety = ARBITRARY_SECOND;	
	//setting up DDR and PORT for buttons;
	BUT_PORT = 0b00001100;
	BUT_DDR = 0b00000000;
	//initializing screen;
	//LCD_Init();
	LCD_Init(&LCD_PORT,&LCD_DDR);
	LCD_GotoXY(0,0);
	LCD_SendString("Time:    N/A    ",16);
	LCD_GotoXY(0,1);
	LCD_SendString("Lap1:    N/A    ",16);
	LCD_GotoXY(0,2);
	LCD_SendString("Lap2:    N/A    ",16);
	LCD_GotoXY(0,3);
	LCD_SendString("Гуськов М.Е.:   ",16);
	LCD_NewItem();
	LCD_GotoXY(14,3);
	LCD_SendChar(0x01);
	//start
	sei();
	while (1);	return 0;
}
