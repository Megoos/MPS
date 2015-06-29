
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

/*тактовая частота мк*/
#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define sbi(x)  (1<<x)
#define cbi(x)  (~(1<<x))


/*___________________________Настройки_____________________________________*/

/*прескалер таймера 1L, 8L, 64L, 256L, 1024L*/
#define SUART_PRESCALER 8L

/*скорость обмена*/
#define SUART_BAUD_RATE 9600L

/*вместимость приемного буфера*/
#define SUART_BUF_SIZE 16

/*пин приемника*/
#define RX_PINX PINB
#define RX_PORTX PORTB
#define RX_DDRX DDRB
#define RX_PIN 3

/*пин передатчика*/
#define TX_PORTX PORTB
#define TX_DDRX DDRB
#define TX_PIN 2

/*________________________Пользовательские функции___________________________*/

void SUART_Init(void);
char SUART_GetChar(void);
void SUART_PutChar(char ch);
void SUART_PutStr(char *str);

/*___________________________________________________________________________*/

#define TRUE 1
#define FALSE 0

#define RX_NUM_OF_BITS  8
#define TX_NUM_OF_BITS  10
#define NUM_OF_CYCLES   3

static volatile uint8_t inbuf[SUART_BUF_SIZE];
static volatile uint8_t	qin = 0;
static volatile uint8_t	qout = 0;

static volatile uint8_t 	flag_rx_ready;
static uint8_t 	                flag_rx_waiting_for_stop_bit;
static uint8_t 	                flag_rx_off;
static uint8_t 	                rx_mask;
static uint8_t 	                timer_rx_ctr;
static uint8_t 	                bits_left_in_rx;

static volatile uint8_t 	flag_tx_busy;
static volatile uint8_t 	timer_tx_ctr;
static volatile uint8_t 	bits_left_in_tx;
static volatile uint16_t	internal_tx_buffer;

//______________________________________________________
#define get_rx_pin_status() RX_PINX & (1<<RX_PIN)
#define set_tx_pin_high()   TX_PORTX |= (1<<TX_PIN)
#define set_tx_pin_low()    TX_PORTX &= ~(1<<TX_PIN)


#define TIME_VALUE  (F_CPU/(SUART_BAUD_RATE*NUM_OF_CYCLES*SUART_PRESCALER))
#define TIMER_TCNT_VALUE (0xff - (TIME_VALUE - 1))
#define ERROR_VALUE ((F_CPU*100)/(TIME_VALUE*SUART_BAUD_RATE*NUM_OF_CYCLES*SUART_PRESCALER))

#if ((TIME_VALUE > 256)||(TIME_VALUE == 0))
#error "TIMER_TCNT_VALUE is not correct. Change PRESCALER, BAUD_RATE or F_CPU"
#endif

#if ((ERROR_VALUE > 100)||(ERROR_VALUE < 100))
#warning "Baud rate error > 1%"
#endif

#if SUART_PRESCALER==1L
#define CSXX (0<<CS02)|(0<<CS01)|(1<<CS00)
#elif SUART_PRESCALER==8L
#define CSXX (0<<CS02)|(1<<CS01)|(0<<CS00)
#elif SUART_PRESCALER==64L
#define CSXX (0<<CS02)|(1<<CS01)|(1<<CS00)
#elif SUART_PRESCALER==256L
#define CSXX (1<<CS02)|(0<<CS01)|(0<<CS00)
#elif SUART_PRESCALER==1024L
#define CSXX (1<<CS02)|(1<<CS01)|(1<<CS00)
#else
#error "prescaller not correct"
#endif

#ifdef __CODEVISIONAVR__
#define TIM0_OVF_VECTOR   TIM0_OVF
#else
#define TIM0_OVF_VECTOR   TIMER0_OVF_vect
#endif

ISR(TIM0_OVF_VECTOR)
{
	uint8_t   start_bit, flag_in;
	uint8_t rx_data;
	static uint16_t  internal_rx_buffer;
	
	TCNT0 += TIMER_TCNT_VALUE;
	
	/* Раздел передатчика */
	if (flag_tx_busy){
		--timer_tx_ctr;
		if (!timer_tx_ctr){
			if (internal_tx_buffer & 0x01){
				set_tx_pin_high();
			}
			else{
				set_tx_pin_low();
			}
			internal_tx_buffer >>= 1;
			timer_tx_ctr = NUM_OF_CYCLES;
			--bits_left_in_tx;
			if (!bits_left_in_tx){
				flag_tx_busy = FALSE;
				set_tx_pin_high();
			}
		}
	}
	/* Раздел приемника */
	if (flag_rx_off == FALSE){
		if (flag_rx_waiting_for_stop_bit){
			if ( --timer_rx_ctr == 0 ){
				flag_rx_waiting_for_stop_bit = FALSE;
				flag_rx_ready = FALSE;
				rx_data = (uint8_t)internal_rx_buffer;
				if ( rx_data != 0xC2 ){
					inbuf[qin] = rx_data;
					if ( ++qin >= SUART_BUF_SIZE ){
						qin = 0;
					}
				}
			}
		}
		else{   // rx_test_busy
			if ( flag_rx_ready == FALSE ){
				start_bit = get_rx_pin_status();
				if ( start_bit == 0 ){
					flag_rx_ready = TRUE;
					internal_rx_buffer = 0;
					timer_rx_ctr = NUM_OF_CYCLES + 1;
					bits_left_in_rx = RX_NUM_OF_BITS;
					rx_mask = 1;
				}
			}
			else{	// rx_busy
				if ( --timer_rx_ctr == 0 ){
					timer_rx_ctr = NUM_OF_CYCLES;
					flag_in = get_rx_pin_status();
					if ( flag_in ){
						internal_rx_buffer |= rx_mask;
					}
					rx_mask <<= 1;
					if ( --bits_left_in_rx == 0 ){
						flag_rx_waiting_for_stop_bit = TRUE;
					}
				}
			}
		}
	}	
}

void SUART_Init(void)
{
	/*инициализация флагов*/
	flag_tx_busy = FALSE;
	flag_rx_ready = FALSE;
	flag_rx_waiting_for_stop_bit = FALSE;
	flag_rx_off = FALSE;
	
	/*настройка выводв*/
	RX_DDRX &= ~(1<<RX_PIN);
	RX_PORTX |= (1<<RX_PIN);
	TX_DDRX |= (1<<TX_PIN);
	TX_PORTX |= (1<<TX_PIN);
	
	/*настройка таймера Т0*/
	TCCR0 = CSXX;
	TCNT0 = TIMER_TCNT_VALUE;
	TIMSK |= (1<<TOIE0);
}

char SUART_GetChar(void) // взять символ
{
	uint8_t tmp;
	char ch;
	
	do{
		tmp = qin;
		while (qout == tmp){
			tmp = qin;
		}
		ch = inbuf[qout];
		if ( ++qout >= SUART_BUF_SIZE ){
			qout = 0;
		}
	}while ((ch == 0x0A) || (ch == 0xC2));
	
	return ch;
}

void SUART_PutChar(char ch)
{
	while(flag_tx_busy){};
	timer_tx_ctr = NUM_OF_CYCLES;
	bits_left_in_tx = TX_NUM_OF_BITS;
	internal_tx_buffer = ((uint16_t)ch<<1) | 0x200;
	flag_tx_busy = TRUE;
}

void SUART_PutStr(char *str)
{
	char ch;
	while (*str){
		ch = *str++;
		SUART_PutChar(ch);
	}
}


void send_Uart(unsigned char c)
{
	while(!(UCSRA & (1 << UDRE))) {}
	UDR = c;
}

unsigned char getch_Uart(void)
{
	while(!(UCSRA & (1 << RXC))) {}
	return UDR;
}


ISR (USART_RXC_vect){ //прерывание по приему данных из ПК
	unsigned char rdbyte = UDR;
	SUART_PutChar(rdbyte); //отправить программно без изменений
	
}
/*
ISR (USART_TXC_vect){ //прерывание по завершению передачи
	
}

ISR (USART_UDRE_vect){//прерывание по освобождению регистра данных
	
}*/
int main(void)
{
	cli();
	_delay_ms(100);

//			RXC			-	Завершение приема
//			|TXC		-	Завершение передачи
//			||UDRE 		-	Отсутствие регистра данных 
//			|||FE		-	Ошибка кадрирования
//			||||DOR		-	Ошибка переполнения буфера
//			|||||PE		-	Ошибка контроля четности
//			||||||U2X	-	Удвоение скорости обмена
//			|||||||MPCM	-	Режим мультипроцессорного обмена
//			76543210
	UCSRA=0b00000000;
		

//			RXCIE		-	Прерывание при приеме данных
//			|TXCIE		-	Прерывание по завершение передачи
//			||UDRIE		-	Прерывание при очистке регистра данных UART
//			|||RXEN		-	Разрешение приема
//			||||TXEN	-	Разрешение передачи
//			|||||UCSZ2	-	UCSZ0:2 размер кадра данных
//			||||||RXB8	-	9 бит принятых данных
//			|||||||TXB8	-	9 бит переданных данных
//			76543210
	UCSRB=0b10011000;	//	разрешен прием,передача и прерывание при приеме по UART


//			URSEL		-	Выбор регистра
//			|UMSEL		-	Режим:1-синхронный 0-асинхронный
//			||UPM1		-	UPM0:1 четность
//			|||UPM0		-	UPM0:1 четность
//			||||USBS	-	Кол-во стоп битов: 0-1, 1-2
//			|||||UCSZ1	-	UCSZ0:2 размер кадра данных
//			||||||UCSZ0	-	UCSZ0:2 размер кадра данных
//			|||||||UCPOL-	Полярность тактового сигнала, 0 - асинхронный режим
//			76543210
	UCSRC=0b10000110;	//	8-битовая посылка
	
	//BAUD =8000000/16*(UBRR+1)  UBBR = 51 -> BAUD ~ 9616 - > погрешность < 0.25% -> норм
	UBRRH=0x00;
	UBRRL=0x33;
	
	SUART_Init();
	sei();
	SUART_PutStr("software uart ");
	
	char mystr[]="UART hardware";
	for (int i=0; i < 13; i++) {
		 send_Uart(mystr[i]);
	}
	_delay_ms(10);
	
	unsigned char rdbyte;
	unsigned char rxbyte;
	
	while(1){
		
		rdbyte = SUART_GetChar();
		if (rdbyte == 0x31) {send_Uart('4'); rdbyte = 0;} //если пришел символ '1' отправить 4
		else
		if (rdbyte == 0x32) {send_Uart('5'); rdbyte = 0;} //если пришел символ '2' отправить 5
		else 
		{
			send_Uart(rdbyte);
			rdbyte = 0;
		}
		
		_delay_ms(10);		
	}
}
