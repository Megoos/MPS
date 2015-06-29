#include "uart.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

void init_UART(void)
{
	//	Установка скорости 9600
	UBRRH=0;	//	UBRR=f/(16*band)-1 f=8000000Гц band=9600, 
	UBRRL=51;	//	нормальный асинхронный двунаправленный режим работы
	
//			RXC			-	завершение приёма
//			|TXC		-	завершение передачи
//			||UDRE 		-	отсутствие данных для отправки
//			|||FE		-	ошибка кадра
//			||||DOR		-	ошибка переполнение буфера
//			|||||PE		-	ошибка чётности
//			||||||U2X	-	Двойная скорость
//			|||||||MPCM	-	Многопроцессорный режим
//			76543210
	UCSRA=0b00000000;

//			RXCIE		-	прерывание при приёме данных
//			|TXCIE		-	прерывание при завершение передачи
//			||UDRIE		-	прерывание отсутствие данных для отправки
//			|||RXEN		-	разрешение приёма
//			||||TXEN	-	разрешение передачи
//			|||||UCSZ2	-	UCSZ0:2 размер кадра данных
//			||||||RXB8	-	9 бит принятых данных
//			|||||||TXB8	-	9 бит переданных данных
//			76543210
	UCSRB=0b00011000;	//	разрешен приём и передача по UART

//			URSEL		-	всегда 1
//			|UMSEL		-	режим:1-синхронный 0-асинхронный
//			||UPM1		-	UPM0:1 чётность
//			|||UPM0		-	UPM0:1 чётность
//			||||USBS	-	топ биты: 0-1, 1-2
//			|||||UCSZ1	-	UCSZ0:2 размер кадра данных
//			||||||UCSZ0	-	UCSZ0:2 размер кадра данных
//			|||||||UCPOL-	в синхронном режиме - тактирование
//			76543210
	UCSRC=0b10000110;	//	8-битовая посылка
}

void send_Uart(unsigned char c)//	Отправка байта
{
	while(!(UCSRA & (1<<UDRE)))	
	{}
	UDR = c;
}

void send_Uart_str(unsigned char *s)//	Отправка строки
{
	while (*s != 0) 
	{
		send_Uart(*s++);
	}
	send_Uart('\r');
}

void send_int_Uart(unsigned int c)//	Отправка числа от 0000 до 9999
{
	unsigned char temp;
	c=c%10000;
	temp=c/100;
	send_Uart(temp/10+'0');
	send_Uart(temp%10+'0');
	temp=c%100;
	send_Uart(temp/10+'0');
	send_Uart(temp%10+'0');
}


void read_socket(char *action, int k)//	Получение байта
{
	for(int i=0; i<k; i++){
	while(!(UCSRA&(1<<RXC))){}
	action[i] = UDR;
	}
}


void read_uart(char *serial)
{
    int cont, fin;
    for (cont=0, fin=0; fin<1; cont++)
    {
        while ((UCSRA & (1 << RXC)) == 0) {}
        serial[cont] = UDR;
        if (serial[cont] == 0x0d) //\n
            fin++;
    }
}
