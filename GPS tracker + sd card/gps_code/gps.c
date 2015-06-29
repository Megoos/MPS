
#define F_CPU 8000000UL	//	рабочая частота

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "uart.h"


#define		LCD_PORT	(PORTA)
#define		LCD_DDR		(DDRA)


#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<PE)
#define DATA_OVERRUN (1<<DOR)

// USART Receiver buffer
#define RX_BUFFER_SIZE 16
char rx_buffer[RX_BUFFER_SIZE];
 
unsigned char rx_wr_index,rx_rd_index,rx_counter;
 
 
// This flag is set on USART Receiver buffer overflow

//char read_enable = 0;
//volatile char lcd_out = 0;


/*
ISR(USART_RXC_vect )
{
	char status,data;
	status=UCSRA;
	data=UDR;
	if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	{
		if (data =='+')
		{
			rx_wr_index=1;
			read_enable = 1;
 
		}
 
		if((data == 0x0d)&&(read_enable == 1))
		{
				read_enable = 0;
				lcd_out = 1;				
		}
 
		if (read_enable == 1)
		{
			rx_buffer[rx_wr_index++]=data;
			if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
 
			if (++rx_counter == RX_BUFFER_SIZE)
			{
				rx_counter=0;
			
			}
 
		}
 
	}
}*/


/*int it_ok (unsigned char *s, unsigned char *c, int d){

for(int i = 0; i<=d; i++){
	if(s[i+1] == c[i]){

	}else{
		return 0;}

	}
	return 1;
}
*/



void main(void)
{
	DDRB |= (1 << 2); // debug
	
	PORTB &= ~(1 << 2);
	
	_delay_ms(1000);
	LCD_PORT = 0xFF;
	LCD_DDR = 0xFF;
	LCD_Init(&LCD_PORT,&LCD_DDR);
	_delay_ms(1000);
	LCD_Clear();
	char* coord1 = "          'N    ";
	char* coord2 = "          'E    ";
	 char* ready = "  Coordinates:  ";
	LCD_GotoXY(0, 0);
	LCD_SendString(ready, 16);

	LCD_GotoXY(0, 1);
	LCD_SendString(coord1, 16);

	LCD_GotoXY(0, 2);
	LCD_SendString(coord2, 16);


	_delay_ms(1000);
					
	init_UART();					//	инициализация UART
	sei();
	_delay_ms(1000);				

	
	//send_int_Uart(2013);			//	отправка числа
	//	send_Uart(13);				//	перенос строки
	PORTB |= (1 << 2);
    while(1)						
    {		        
        read_coord(coord1,10);
		LCD_GotoXY(0, 1);
		LCD_SendString(coord1, 16);
		read_coord(coord2,10);
		LCD_GotoXY(0, 2);
		LCD_SendString(coord2, 16);
		
    }
		  
}
