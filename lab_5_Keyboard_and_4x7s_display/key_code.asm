;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;											;
;	PIC Microcontroller for                 ;
;   Interfacing a Keypad                    ;
;   and 4x7Segments Display                 ;
;											;
;	By Mikhail Guskov 2015					;
;	meguskov@gmail.com.com					;
;	Skype: meguskov							;
;											;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	LIST p=16f877a 				;	tell assembler what chip we are using
	include "P16f877a.inc"		;	include library
   	__config h'3f32'			;	setting fuse
   
	PC equ h'02'				;	reg PC, count command

	cblock h'20'				;	varible with 20h adress
	what_button					;	identification button
	digit1						;	digit 1
	digit2						;	digit 2
	digit3						;	digit 3
	digit4						;	digit 4
	digit
	J
	K
	endc						

	org h'0000'					;	adress begin programm

	bsf STATUS, RP0 			;	select bank 1 (позволяет выбрать Input / Output состояние на нашем порте )
	movlw b'00000000' 			;	set PORTС all outputs '0'-output,'1'-input.
	movwf TRISC					;	set bits
	movlw b'11111000' 			;	set PORTD pins 0, 1, 2 and 3 outputs. pins 4, 5, 6 and 7 inputs
	movwf TRISD 				;	set bits 
	movlw b'00000000'
	movwf TRISB
	bcf STATUS, RP0 			;	select bank 0
	goto setup					;	skip setup.


digit_data						;	procedure selection numbers
	addwf PC					;	add number to the PC, which jump need number
	retlw b'00111111'			;	number 0
	retlw b'00000110'			;	number 1	
	retlw b'01011011'			;	number 2
	retlw b'01001111'			;	number 3
	retlw b'01100110'			;	number 4
	retlw b'01101101'			;	number 5
	retlw b'01111101'			;	number 6
	retlw b'00000111'			;	number 7
	retlw b'01111111'			;	number 8
	retlw b'01101111'			;	number 9	
	retlw b'01000000'			;	*
	

swap 							; procedure for swap varible
		movf digit3, w
		movwf digit4
		movf digit2, w
		movwf digit3
		movf digit1, w
		movwf digit2
	return

setup							;	sets ports and registrs
		movlw 0xff				
		movwf what_button
		movlw d'10'				;	output on display first symbol
		movwf digit1
		movwf digit2
		movwf digit3
		movwf digit4
		clrf PORTD				;	make PORTD all zero's
		movlw b'11111111'		
		movwf PORTB	

begin							;	main program
	bsf PORTB, 5
	call display_digit			;	call procedure display_digit
	movf digit1, w
	call check_keypad			;	call procedure check_keypad
	;call delay

	clrf PORTD
	movwf what_button
	xorlw 0xff
	btfsc STATUS, Z
	goto begin
	call swap
	movf what_button, w
	movwf digit1
	
keyoff							; 	unpressed button?
	bcf PORTB, 5
	call display_digit			;	call procedure display_digit
	call check_keypad
	;call delay

	clrf PORTD
	movwf what_button
	xorlw 0xff
	btfss STATUS, Z
	goto keyoff
	
	goto begin					;	while(1)
	


check_keypad					;	procedure scan and check button
		
		;movf what_button, w	 ;	last pressed button
		
		bsf PORTD, 0			;	lets scan the first column of keys		
		btfsc PORTD, 4			;	has the 1 key been pressed? if yes then
		retlw d'01'				;	copy decimal number 01 into w.
		btfsc PORTD, 5			;	has the 4 key been pressed? if yes then
		retlw d'04'				;	copy decimal number 04 into w.
		btfsc PORTD, 6			;	has the 7 key been pressed? if yes then
		retlw d'07'				;	copy decimal number 07 into w.
		btfsc PORTD, 7			;	has the * key been pressed? if yes then
		goto reset_all			; 	call procedure reset variable digit
		bcf PORTD, 0			;	now we have finished scanning the first column of keys
	
		bsf PORTD, 1			;	lets scan the middle column of keys
		btfsc PORTD, 4			;	has the 2 key been pressed? if yes then
		retlw d'02'				;	copy decimal number 02 into w. 
		btfsc PORTD, 5			;	has the 5 key been pressed? if yes then
		retlw d'05'				;	copy decimal number 05 into w.
		btfsc PORTD, 6			;	has the 8 key been pressed? if yes then
		retlw d'08'				;	copy decimal number 08 into w.
		btfsc PORTD, 7			;	has the 0 key been pressed? if yes then
		retlw d'00'				;	copy decimal number 00 into w.
		bcf PORTD, 1			;	now we have finished scanning the middle column of keys
	
		bsf PORTD, 2			;	lets scan the last column of keys
		btfsc PORTD, 4			;	has the 3 key been pressed? if yes then
		retlw d'03'				;	copy decimal number 03 into w.
		btfsc PORTD, 5			;	has the 6 key been pressed? if yes then
		retlw d'06'				;	copy decimal number 06 into w.
		btfsc PORTD, 6			;	has the 9 key been pressed? if yes then
		retlw d'09'				;	copy decimal number 09 into w.
		btfsc PORTD, 7			;	has the # key been pressed? if yes then
		goto test				;	call test procedure, input on display "1234".
		bcf PORTD, 2			;	now we have finished scanning the last column of keys
		retlw 0xff

reset_all
		movlw d'10'				
		movwf digit2
		movwf digit3
		movwf digit4
		retlw d'10'				

test 
		movlw d'03'			
		movwf digit2
		movlw d'02'
		movwf digit3
		movlw d'01'
		movwf digit4
		retlw d'04'
	return	

delay       movlw       .4
            movwf       J
JLoop       movlw       .255
            movwf       K
KLoop       decfsz      K,F
            goto        KLoop
            decfsz      J,F
            goto        JLoop
            return

;delay
;	nop
;	nop
;	nop
;	return				

display_digit					;	procedure output numbers on display
     
	movf digit1, w
	call digit_data				;	receiving 8 bits code digit
	bcf PORTB, 0
	movwf PORTC					;	and move this 8-bits of data to PORTC for display on the display
	call delay
	bsf PORTB, 0

	movf digit2, w
	call digit_data				;	receiving 8 bits code digit
	bcf PORTB, 1
	movwf PORTC					;	and move this 8-bits of data to PORTC for display on the display
	call delay
	bsf PORTB, 1

	movf digit3, w
	call digit_data				;	receiving 8 bits code digit
	bcf PORTB, 2
	movwf PORTC					;	and move this 8-bits of data to PORTC for display on the display
	call delay
	bsf PORTB, 2

	movf digit4, w
	call digit_data				;	пreceiving 8 bits code digit
	bcf PORTB, 3
	movwf PORTC					;	and move this 8-bits of data to PORTC for display on the display
	call delay
	bsf PORTB, 3
	
	return						;	return to main program
	
	end					