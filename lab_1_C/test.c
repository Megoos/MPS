#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 8000000UL // 8MHz

#define DDR_LED DDRD
#define PORT_LED PORTD

#define DDR_BUT DDRC
#define PORT_BUT PORTC
#define PIN_BUT PINC

#define DEF_DELAY 2
#define MAX_DELAY 10
#define MIN_DELAY 1

int main(void) {
	DDR_LED = 0x0F;
	PORT_LED = 0x0F;
	DDR_BUT |= ~((1<<0) | (1<<1));
	PORT_BUT |= (1<<0) | (1<<1);

  	int ms = DEF_DELAY;
	int st0 = 0;
	int st1 = 0;

		while(1){
			for(int i=0; i<=3; i++){ 
				for(int j = ms; j>=0; j--){
				
					if(st0 == 0){
					if(ms < MAX_DELAY){
						if((PIN_BUT & (1<<0))==0){
							_delay_ms(50);
							if((PIN_BUT & (1<<0))==0){
								ms = ms+1;
								st0 = 1;
							}					
						}
					}
				}else{
					if((PIN_BUT & (1<<0))==1){
						st0 = 0;
					}
				}
				
				
				if (st1 == 0){
					if (ms > MIN_DELAY){
						if((PIN_BUT & (1<<1))==0){
							_delay_ms(50);
							if((PIN_BUT & (1<<1))==0){
								ms = ms-1;
								st1 = 1;
							}
						}
					}
				}else{
					if((PIN_BUT & (1<<1))==2){
							st1 = 0;
						}
				}
				
				PORT_LED = ~(1<<i);
				_delay_ms(50);
			
				}
				
			}
		}	
}
