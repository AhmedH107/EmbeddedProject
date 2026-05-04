 /*
 * IncFile1.inc
 *
 *  Created: 2024-02-19 14:13:50
 *   Author: ahmha095
 */ 


 SPI_INIT:
	ldi		r16, (1<<PB3)|(1<<PB5)|(1<<PB2)|(1<<PB1)
	out		DDRB, r16
	ldi		r16, (1<<SPE)|(1<<MSTR)|(2<< SPR0) 
	out		SPCR, r16
	ret


SEND_SS:
	sbi PORTB,PB2
	nop
	cbi PORTB,PB2
	ret

SPI_TRANSMIT:
	out SPDR,r16
SPI_WAIT:
	in r16,SPSR
	sbrs r16,SPIF
	rjmp SPI_WAIT
	ret

SPI_WRITE:
	push r18

	ldi r18,4
DA_LOOP:
	ld r16,Z+ 
	call SPI_TRANSMIT
	dec r18
	brne DA_LOOP
	call SEND_SS

	pop r18
	ret
