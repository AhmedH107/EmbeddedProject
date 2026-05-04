/*
 * TWI.inc
 *
 *  Created: 2024-03-09 20:26:07
 *   Author: danma565
 */ 

 .equ SCL = PC5
 .equ SDA = PC4

/* SENDLYS:
	call READ_BUTT
	ldi r17,$26
	sbrs r16,3
	call DAS_GRON
	sbrs r16,2
	call DAS_ROTT
	ret*/
TWI_INIT:
	clr r25
	out DDRC, r25
	ret

DAS_GRON:
	call TWI_INIT
	ldi r25,$FE
	ldi r26,$26
	call TWI_WRITE
	call JOY_INIT
	ret

DAS_OFF:
	ldi r25,$3
	ldi r26,$26
	call TWI_WRITE
	ret

DAS_ROTT:
	call TWI_INIT
	ldi r26,$26
	ldi r25,$FD
	call TWI_WRITE
	call JOY_INIT
	ret

TWI_WRITE:
	call START
	lsl r26
	call TWI_SEND
	mov r26,r25
	call TWI_SEND
	call STOP
	ret

TWI_READ:
	lsl r26
	ori r26,1
	call START
	call TWI_SEND
	call READ_BYTE
	call STOP
	ret
	
TWI_SEND:
	ldi r18,8
SLOOP:
	lsl r26 // start with MSB, me thinks
	brcs HIGHBIT
LOWBIT:
	call SDL
	rjmp SCHECK
HIGHBIT:
	call SDH
SCHECK:
	dec r18
	brne SLOOP
	call SDH
	ret



READ_BYTE:
	ldi r18,8
	clr r26
	clr r25
SWITCHES:
	call SCP
	or r25,r26
	lsl r25
	dec r18
	brne SWITCHES
	call SDH // for ACK
	call STOP
	lsr r25 // A BRIDGE TOO FAR!
	andi r25,$3F
	ret

SCP: // ein klockcycel
	cbi DDRC, SCL
	call WAIT       //  |
	in r26,PINC     //  V        |
	swap r26 // pc4 000DATA 0000 V
	andi r26,1 // pc4 0000 000DATA
	sbi DDRC,SCL  // Kommande klockcycel!
	call WAIT
	cbi DDRC,SCL
	call WAIT // FOR LATERS
	ret
	

START:
	sbi DDRC,SDA
	call WAIT
	sbi DDRC,SCL
	call WAIT
	ret

STOP:
	sbi DDRC,SDA
	call WAIT
	cbi DDRC,SCL
	call WAIT
	cbi DDRC,SDA
	call WAIT
	ret


SDL:
	sbi DDRC,SDA
	call WAIT
	cbi DDRC,SCL
	call WAIT
	sbi DDRC,SCL
	call WAIT
	ret

SDH:
	cbi DDRC,SDA 
	call WAIT
	cbi DDRC,SCL
	call WAIT
	sbi DDRC,SCL
	call WAIT
	ret


WAIT:
	push r20
	ldi r20,10
WAIT_LOOP:
	subi r20,1
	brne WAIT_LOOP
	pop r20
	ret
