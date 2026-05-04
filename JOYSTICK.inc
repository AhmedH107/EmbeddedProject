/*
 * IncFile4.inc
 *
 *  Created: 2024-03-04 14:18:07
 *   Author: ahmha095
 */ 
 JOY_INIT:
	ldi r16,0b11100000
	out DDRC,r16
	ret

P1_INPUT:
	push r16
	ldi r16,(0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(P1_X)
	call INPUT
	call CHECK_PX
	pop r16
	ret

P2_INPUT:
	push r16
	ldi r16,(0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(P2_X)
	call INPUT
	call CHECK_PX
	pop r16
	ret

INPUT:
	sts ADMUX,r16
	ldi r16,(1<<ADSC)|(1<<ADEN)|(7<<ADPS0)
	sts ADCSRA,r16
JOY_WAIT:
	lds r16,ADCSRA
	andi r16,(1<<ADSC)
	cpi r16,0
	brne JOY_WAIT
	lds r16,ADCH
	andi r16,3
	ret

CHECK_PX:	
	cpi r16,3
	breq  P_RIGHT
	cpi r16,0
	breq P_LEFT
	rjmp PX_DONE
P_RIGHT: 
	call OFF_PIX
	lds r21,PX
	inc r21
	cpi r21,8
	brlo P_RIGHT_DONE
	dec r21
P_RIGHT_DONE:
	sts PX,r21
	ldi r22,5
	rjmp PX_DONE
P_LEFT:
	call OFF_PIX
	lds r21,PX
	dec r21
	cpi r21,255
	brne P_LEFT_DONE
	ldi r21,0
P_LEFT_DONE:
	sts PX,r21
	ldi r22,5
PX_DONE:
	ret


DELAY:
	ldi r16,200
D_LOOP:
	subi r16,1
	brne D_LOOP
	ret