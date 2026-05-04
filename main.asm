;
; AssemblerApplication1.asm
;
; Created: 2024-03-26 12:40:07
; Author : ahmha095
;


; Replace with your application code
;
; AssemblerApplication3.asm
;
; Created: 2024-02-16 20:02:21
; Author : ahmha095
;


; Replace with your application code
jmp COLD_START

.org 0x0020
	jmp INT_DAMATRIX

.dseg
.org 0x128
VMEM: .byte 32

LINE: .byte 1 // AVBROTT ONLY

PX: .byte 1

PY: .byte 1

CX: .byte 1

CY: .byte 1

OLD_PX: .byte 1

OLD_PY: .byte 1

COUNTER: .byte 1

COUNTER2: .byte 1

FULL_COUNTER: .byte 1

.cseg

.equ RED = 2

.equ GREEN = 1

.equ P1_X = $02

.equ P2_X = $00

.include "SPI.inc"

.include "INTERUPT.inc"

.include "DROP_DOT.inc"

.include "JOYSTICK.inc"

.include "LOGIC.inc"

.include "TWI.inc"

COLD_START:
	ldi r16,4
	sts PX,r16
	clr r16
	sts LINE,r16
	sts COUNTER, r16
	sts COUNTER2, r16
	sts FULL_COUNTER, r16
	sts PY,r16
	call SPI_INIT
	
	call INT_INIT
	call JOY_INIT

	
/*	ldi r17,0b00000000 ; magenta
	ldi r18,0b00000000 ; green
	ldi r24,~0x80     */  ; anode

	ldi ZH,HIGH(VMEM)
	ldi ZL,LOW(VMEM)

	call CLEAR_VMEM

	


	/*push ZH
	push ZL

	ldi ZH,HIGH(VMEM)
	ldi ZL,LOW(VMEM)
	
	ldi r24,~0x1
	ldi r17, 0b00000000
	st Z+,r17
	st Z+,r18
	st Z+,r17
	st Z+,r24
	
	ldi r24,~0x2
	ldi r17,0b00000000
	st Z+,r17
	st Z+,r18
	st Z+,r17
	st Z+,r24
	
	ldi r24,~0x4
	ldi r17,0b00000000
	st Z+,r17
	st Z+,r18
	st Z+,r17
	st Z+,r24

	ldi r24,~0x8
	ldi r17,0b00000000
	ldi r25,16
	st Z+,r17
	st Z+,r18
	st Z+,r17
	st Z+,r24

	ldi r24,~0x10
	ldi r17,0b00000000
	st Z+,r17
	st Z+,r18
	st Z+,r17
	st Z+,r24

	ldi r24,~0x20
	ldi r17,0b00000000
	st Z+,r17
	st Z+,r18
	st Z+,r17
	st Z+,r24

	ldi r24,~0x40
	ldi r17,0b00000000
	st Z+,r17
	st Z+,r18
	st Z+,r17
	st Z+,r24

	ldi r24,~0x80
	ldi r17,0b00000000
	st Z+,r17
	st Z+,r18
	st Z+,r17
	st Z+,r24

	pop ZL
	pop ZH*/

	clr r19
	sei
	call DAS_ROTT
MAIN:
	call P_HANDLE
	sbrc r28,0
	rjmp SWAP_P
	call WINCON
	rjmp MAIN
SWAP_P:
	call DROP_DOT
	com r19

	sbrc r19, 0
	call DAS_GRON

	sbrs r19, 1
	call DAS_ROTT
	ldi r28,0
	rjmp MAIN


	// SET_PIX(X,Y,COLOUR)--------------------------------------------------------------------------------
	// SETS A PIXLE IN THE FIELD PARAMETERS 0..7 FOR X AND Y CORD, RED OR GREEN FOR COLOUR :^)
OFF_PIX:
	call RESET_PTR
	call GET_XCORD

	sbrs r19, 0
	subi ZL,-RED

	sbrc r19, 0
	subi ZL,-GREEN

	ld r17,Z
	lds r16,PY
	call GET_YCORD
	eor r18,r17
	st Z,r18
	ret

SET_PIX:
	call RESET_PTR
	call GET_XCORD

	sbrs r19, 0
	subi ZL,-RED

	sbrc r19, 0
	subi ZL,-GREEN

	ld r17,Z
	lds r16,PY
	call GET_YCORD
	or r18,r17
	st Z,r18
	ret

GET_XCORD:
	lds r16,PX
X_LOOP:
	cpi r16,0 
	breq X_DONE
	subi ZL,-4
	dec r16
	rjmp X_LOOP
X_DONE:
	ret

GET_YCORD:
	ldi r18,0b00000001
Y_LOOP:
	cpi r16,0
	breq Y_DONE
	lsl r18
	dec r16
	rjmp Y_LOOP
Y_DONE:
	ret
	//---------------------------------------------------------------------------------------------------------------------------


	// GET_COLUMN(Y)--------------------------------------------------------------------------------------------------------------
	// RETURNS AN ENTIRE COLUMN, WE THEN READ FROM R18 REGISTER, MAKE PART OF BIGGER FUNC LATERS
GET_COLUMN:
	call RESET_PTR
	call GET_XCORD
	subi ZL,-1
	ld r17,Z
	subi ZL,-1
	ld r18,Z
	or r18,r17
	ret



	//GET_COLOUR_COLUMN(COLOUR)-------------------------------------------------------------------------------------------------------------------------
	//RETURNS VMEM RED(+2) OR GREEN(+1), LOAD RED/GREEN IN R27 BEFORE HAND
GET_C_CLM:
	call RESET_PTR
	call GET_XCORD
	add ZL,r27
	ld r18,Z
	ret

	//-------------------------------------------------------------------------------------------------------------------------
RESET_PTR:
	ldi r30,$28 
	ldi r31,$1
	ret

CLEAR_VMEM:	
	push ZH
	push ZL
	push XH
	push XL
	push r16
	push r17
	push r18
	push r20

	ldi ZH,HIGH(VMEM)
	ldi ZL,LOW(VMEM)

	ldi	r20, ~1 ; first column

	clr r17
	ldi r16,8
CLEAR_LOOP2:
	ldi r18,3
CLEAR_LOOP1:
	st Z+,r17 
	dec r18 
	brne CLEAR_LOOP1
	st Z+,r20
	
	lsl	r20
	ori	r20, 1
	dec r16
	brne CLEAR_LOOP2

	pop r20
	pop r18
	pop r17
	pop r16
	pop XL
	pop XH
	pop ZL
	pop ZH
	ret

	//PLAYER_HANDLING------------------------------------------------------------------------------------------------

P_HANDLE:
	sbrc r19,0
	rjmp PLAYER2_TURN
	call P1_LOOP
	rjmp H_DONE 
PLAYER2_TURN:
	call P2_LOOP
H_DONE:
	ret



P1_LOOP:
	ldi r27, GREEN //Fungerar inte om man byter green och red... tnker inte ens frska fixa det haha ;)
	call OFF_PIX
	call DELAY
	call SET_PIX
	call P1_INPUT
	call CHECK_P1_INPUT
	call DELAY
	ret

P2_LOOP:
	ldi r27, RED
	call OFF_PIX
	call DELAY
	call SET_PIX
	call P2_INPUT
	call CHECK_P2_INPUT 
	call DELAY

	ret

CHECK_P1_INPUT:
	sbis PIND, PD1
	call CHECK_CLM
	ret

CHECK_P2_INPUT:
	sbis PIND, PD0
	call CHECK_CLM
	ret

CHECK_CLM:
	call GET_COLUMN
	cpi r18,$FF
	breq FULL
NOT_FULL:
	call SWITCH_P
FULL:
	ret

SWITCH_P:
	ldi r28,1
	ret

