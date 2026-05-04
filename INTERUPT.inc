 /*
 * IncFile2.inc
 *
 *  Created: 2024-02-22 09:53:53
 *   Author: ahmha095
 */ 
 INT_INIT:
	push r16
	ldi r16, (1<<WGM12)|(1<<CS12)
	sts TCCR1B,r16
	ldi r16,0
	sts OCR1AH,r16 // HIGH 15-8
	sts OCR1AL,r16 // LOW 7-0
	ldi r16,(1<<OCIE1A) //ENABLES THE INT, ME THINKS
	sts TIMSK1,r16
	pop r16
	ret

INT_DAMATRIX:
	push r16
	in r16,sreg
	push r16
	call GET_LINE
	pop r16
	out sreg,r16
	pop r16
	reti

GET_LINE:
	push r16
	push ZH
	push ZL

	ldi ZH,HIGH(VMEM)
	ldi ZL,LOW(VMEM)

	call RESET_PTR
	lds r16,LINE
	cpi r16,8
	brne LINE_LOOP
	ldi r16,0
	sts LINE,r16
LINE_LOOP:
	cpi r16,0
	breq LINE_DONE
	dec r16
	subi ZL,-4
	rjmp LINE_LOOP
LINE_DONE:
	call SPI_WRITE


	lds r16,LINE
	inc r16
	sts LINE,r16
	pop ZL
	pop ZH
	pop r16
	ret
