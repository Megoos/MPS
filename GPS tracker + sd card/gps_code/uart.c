#include "uart.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

void init_UART(void)
{
	//	��������� �������� 9600
	UBRRH=0;	//	UBRR=f/(16*band)-1 f=8000000�� band=9600, 
	UBRRL=51;	//	���������� ����������� ��������������� ����� ������
	
//			RXC			-	���������� �����
//			|TXC		-	���������� ��������
//			||UDRE 		-	���������� ������ ��� ��������
//			|||FE		-	������ �����
//			||||DOR		-	������ ������������ ������
//			|||||PE		-	������ ��������
//			||||||U2X	-	������� ��������
//			|||||||MPCM	-	����������������� �����
//			76543210
	UCSRA=0b00000000;

//			RXCIE		-	���������� ��� ����� ������
//			|TXCIE		-	���������� ��� ���������� ��������
//			||UDRIE		-	���������� ���������� ������ ��� ��������
//			|||RXEN		-	���������� �����
//			||||TXEN	-	���������� ��������
//			|||||UCSZ2	-	UCSZ0:2 ������ ����� ������
//			||||||RXB8	-	9 ��� �������� ������
//			|||||||TXB8	-	9 ��� ���������� ������
//			76543210
	UCSRB=0b00011000;	//	�������� ���� � �������� �� UART

//			URSEL		-	������ 1
//			|UMSEL		-	�����:1-���������� 0-�����������
//			||UPM1		-	UPM0:1 ��������
//			|||UPM0		-	UPM0:1 ��������
//			||||USBS	-	��� ����: 0-1, 1-2
//			|||||UCSZ1	-	UCSZ0:2 ������ ����� ������
//			||||||UCSZ0	-	UCSZ0:2 ������ ����� ������
//			|||||||UCPOL-	� ���������� ������ - ������������
//			76543210
	UCSRC=0b10000110;	//	8-������� �������
}

void send_Uart(unsigned char c)//	�������� �����
{
	while(!(UCSRA & (1<<UDRE)))	//	���������������, ����� ������� ��������
	{}
	UDR = c;
}

void send_Uart_str(unsigned char *s)//	�������� ������
{
	while (*s != 0) 
	{
		send_Uart(*s++);
	}
	send_Uart('\r');
}

void send_int_Uart(unsigned int c)//	�������� ����� �� 0000 �� 9999
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


void read_coord(char *action, int k)//	��������� �����
{
	for(int i=1; i<(k+1); i++){
	while(!(UCSRA&(1<<RXC))){}	//	���������������, ����� ������� ��������
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
