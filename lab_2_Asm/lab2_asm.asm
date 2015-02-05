.include "m16def.inc"
.def temp = r16
.def count = r17
.def ms = r18
.def i = r10
.equ delay_time1 = 255
.equ delay_time2 = 255
.equ max_delay = 30
.equ min_delay = 5
.equ def_delay = 15

;____ начало кода ____
    .org 0
;____ инициализаци€ стека ____
Init: 	ldi     temp, low(RAMEND)
		out     SPL, temp        ;вершина стека конечный адрес ќ«”
   		ldi     temp, high(RAMEND)
   		out     SPH, temp
;____ инициализаци€ портов ____
ldi temp, 0b00001111 ; загрузить константу
out DDRD, temp ; запись в порт
ldi temp, 0b11111100 
out DDRC, temp		; PC(0-1) на ввод
ldi temp, 0b00000011
out PORTC, temp		;подключение подт€гивающего резистора
ldi ms, def_delay

Loop: 
		ldi count, 0x04				
		ldi temp, 0b11111110
		
	Four_cycle:
		mov i, ms			
		All_delay:
;-----------------------------------------
			cpi ms, max_delay
			breq Q1
				sbic PINC,0
				rjmp Q1
				rcall Delay				
				sbic PINC,0
				rjmp Q1
				inc ms
				rjmp Q2
;-----------------------------------------
		Q1:		
			cpi ms, min_delay
			breq Q2
				sbic PINC,1
				rjmp Q2
				rcall Delay
				sbic PINC,1
				rjmp Q2
				dec ms
				rjmp Q2
						
		Q2:
			out PORTD, temp
			rcall Delay
			
		Q3: 			
			dec i
			brne All_delay
			sec
			rol temp			
			dec count
breq Loop
	rjmp Four_cycle			
rjmp Loop

;максимальна€ задержка составл€ет 195848 тактов,
;что при частоте 8ћгц составл€ет примерно 24.5мс

Delay:
         ldi r21, delay_time1
 delay1: ldi r20, delay_time2
 delay2: dec r20
         brne delay2
         dec r21
         brne delay1
 ret
		

