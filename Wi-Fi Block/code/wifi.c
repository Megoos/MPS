
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


	char serial[50];
    int id_socket; 
    char condition[11]; //sostoianie
    char ask_socket[13]; // opros 
    unsigned char action[10];   


void main(void)
{
	DDRB |= (1 << 2); // debug
	DDRB |= (1 << 3); // Led restart PC
	DDRB |= (1 << 4); // Led on PC
	DDRB |= (1 << 5); // Led good
    DDRB |= (1 << 6); // Led error
    DDRB |= (1 << 7); // Led socket
	
	PORTB |= (1 << 2);
	PORTB |= (1 << 3);
	PORTB |= (1 << 4);
	PORTB |= (1 << 5);
    PORTB |= (1 << 6);
    PORTB |= (1 << 7);
    _delay_ms(1000);
	LCD_PORT = 0xFF;
	LCD_DDR = 0xFF;
	LCD_Init(&LCD_PORT,&LCD_DDR);
	_delay_ms(1000);
	LCD_Clear();

					
	init_UART();					//	инициализация UART
	sei();
	_delay_ms(1000);				

    while(1)						
    {	
	    send_Uart_str("AT+"); // 
        read_uart(serial);
		PORTB |= (1 << 6);
		if (serial[1] == 'O' && serial[2] == 'K')
        {			
			send_Uart_str("AT+WJOIN"); // 
            read_uart(serial);
            if (serial[1] == 'O' && serial[2] == 'K')
            {					
				//AT+SKCT=[protocol],[cs],[host_timeout],<port><CR>
                send_Uart_str("AT+SKCT=0,1,0,5000"); // create socket 
                read_uart(serial);
                if (serial[1] == 'O' && serial[2] == 'K')
                {					
                    id_socket = serial[4]; // save id socket
                    sprintf(condition,"AT+SKSTT=%c", id_socket);
                    sprintf(ask_socket,"AT+SKRCV=%c,4", id_socket);
                    do
                    {
                        send_Uart_str(condition); // check
                        read_uart(serial);
                        if (serial[6] == '2') // если подключены к соккету
                        {	
									
                            PORTB |= (1 << 7);
                            PORTB &= ~(1 << 5);
                            send_Uart_str(ask_socket);
                            read_uart(serial);

                            if (serial[1] == 'O' && serial[2] == 'K' && serial[4] != '0')
                            {	
								PORTB &= ~(1 << 7);
                                read_socket(action,1);
                                if (action[0] == '1'){																	
									PORTB &= ~(1 << 3);
									_delay_ms(1000);
                                    PORTB |= (1 << 3);
								}									
                                if (action[0] == '2'){
									PORTB &= ~(1 << 2);
									PORTB &= ~(1 << 4);
									_delay_ms(1000);
                                    PORTB |= (1 << 4);
								}
                            }
                        }
                        else // если не подключены к разьему
                        {
                            PORTB |= (1 << 5);
                            PORTB &= ~(1 << 7);                            
                        }
                    } while (serial[6] != '0');
                }
                else
                    PORTB &= ~(1 << 6);
            }
            else
                PORTB &= ~(1 << 6);
        }
        else
            PORTB &= ~(1 << 6);
    }
		  
}
