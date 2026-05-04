//GAMELOGIC------------------------------------------------------------------------------------------------------
WINCON:
	//Vertical
	//lds r16, PX
	//lds r17, PY
	//lds r18, COUNTER2

	//r25 har x, r26 har y
	
	//sts PX, r25 //Tar punktens X
VERTICAL:
	call GET_C_CLM
	ldi r17,$F0
	ldi r16,4
CHECK_VER_WIN: 
	cp r18,r17
	breq WIN_SHORTCUT
	lsr r17
	dec r16
	brne CHECK_VER_WIN
	call CHANGE_PTR
	call COUNTER_RESET
	rjmp HORIZ_DIA_LOOP
H_RIGHT:
	inc r16
	cpi r16, 8
	breq NEXT_SHORTCUT
	rjmp HD_BEGIN
H_LEFT:
	dec r16
	cpi r16, 255
	breq NEXT_SHORTCUT
	rjmp HD_BEGIN
DOWN_LEFT:
	dec r16
	cpi r16, 255
	breq NEXT_SHORTCUT
	dec r17
	cpi r17, 255
	breq NEXT_SHORTCUT
	rjmp HD_BEGIN
UP_RIGHT:
	inc r16
	cpi r16, 8
	breq NEXT_SHORTCUT
	inc r17
	cpi r17, 8
	breq NEXT_SHORTCUT
	rjmp HD_BEGIN
WIN_SHORTCUT:
	jmp WIN
NEXT_SHORTCUT:
	jmp NEXT
DOWN_RIGHT:
	inc r16
	cpi r16, 8
	breq NEXT_SHORTCUT
	dec r17
	cpi r17, 0
	breq NEXT_SHORTCUT
	rjmp HD_BEGIN
UP_LEFT:
	dec r16
	cpi r16, 255
	breq NEXT
	inc r17
	cpi r17, 8
	breq NEXT
	rjmp HD_BEGIN
HORIZ_DIA_LOOP:
	call RESET_PTR
	lds r16, PX
	lds r17, PY
	lds r18, COUNTER2
	cpi r18, 0
	breq H_RIGHT
	cpi r18, 1
	breq H_LEFT
	cpi r18, 2
	breq DOWN_LEFT
	cpi r18, 3
	breq UP_RIGHT
	cpi r18, 4
	breq DOWN_RIGHT
	cpi r18, 5
	breq UP_LEFT
	HD_BEGIN:
	sts PX, r16
	sts PY, r17

	call GET_C_CLM
	mov r17,r18

	lds r16,PY
	call GET_YCORD

	and r17,r18
	cpi r17,0
	breq NEXT

	lds r16,COUNTER
	inc r16
	cpi r16,3
	brsh WIN
	sts COUNTER,r16
	rjmp HORIZ_DIA_LOOP

NEXT:
	lds r18, COUNTER2
	cpi r18, 0
	breq STANDARD_RESET
	cpi r18, 1
	breq COUNTERED_RESET
	cpi r18, 2
	breq STANDARD_RESET
	cpi r18, 3
	breq COUNTERED_RESET
	cpi r18, 4
	breq STANDARD_RESET
	cpi r18, 5
	breq EXIT
	NEXT_NEXT:
	inc r18
	sts COUNTER2,r18
	rjmp HORIZ_DIA_LOOP 


EXIT:
	ldi r16,0
	sts COUNTER,r16
	sts COUNTER2,r16
	call CHANGE_BACK

	lds r16, FULL_COUNTER
	cpi r16, 56
	breq DRAW_SCREEN_J
	ret
DRAW_SCREEN_J:
	jmp DRAW_SCREEN

STANDARD_RESET:
	call CHANGE_BACK
	call CHANGE_PTR
	rjmp NEXT_NEXT
COUNTERED_RESET:
	call CHANGE_BACK
	call CHANGE_PTR
	call COUNTER_RESET
	rjmp NEXT_NEXT
	
WIN:
	call VICTORY_SCREEN
	ret


CHANGE_PTR:
	lds r16,PX
	sts OLD_PX,r16
	lds r16,PY
	sts OLD_PY,r16

	lds r16,CX
	sts PX,r16
	lds r16,CY
	sts PY,r16
	ret

CHANGE_BACK:
	lds r16,OLD_PX
	sts PX,r16
	lds r16,OLD_PY
	sts PY,r16
	ret

COUNTER_RESET:
	ldi r16, 0
	sts COUNTER, r16
	ret


VICTORY_SCREEN:
	call RESET_PTR
	call CLEAR_VMEM
	sbrc r19, 0
	rjmp P1_Victory
	
	sbrs r19, 0
	rjmp P2_Victory
	
	P1_Victory:
	rcall P1_VIC_SCR

	rjmp VIC_DELAY

	P2_Victory:
	rcall P2_VIC_SCR

	VIC_DELAY:
	ldi r17, 100
VIC_D_LOOP1:
	ldi r16, 255
VIC_D_LOOP2:
	dec r16
	brne VIC_D_LOOP2
	dec r17
	brne VIC_D_LOOP1

	call CLEAR_VMEM
	ret
	
DRAW_SCREEN:
	push ZH
	push ZL
	push XH
	push XL
	
	ldi ZH, HIGH(DRAW_VS*2)
	ldi ZL, LOW(DRAW_VS*2)

	ldi XH, HIGH(VMEM)
	ldi XL, LOW(VMEM)
	
	ldi r16, 8
DRAW_LOOP:
	lpm r17, Z+
	st X, r17
	subi XL, -4
	dec r16
	brne DRAW_LOOP

	pop XL
	pop XH
	pop ZL
	pop ZH
	rjmp VIC_DELAY

P1_VIC_SCR:
	push ZH
	push ZL
	push XH
	push XL
	
	ldi ZH, HIGH(P1_VS*2)
	ldi ZL, LOW(P1_VS*2)

	ldi XH, HIGH(VMEM)
	ldi XL, LOW(VMEM)
	
	ldi r16, 8
	subi XL, -2
P1_V_LOOP:
	lpm r17, Z+
	st X, r17
	subi XL, -4
	dec r16
	brne P1_V_LOOP

	pop XL
	pop XH
	pop ZL
	pop ZH

	ret

P2_VIC_SCR:

	push ZH
	push ZL
	push XH
	push XL
	
	ldi ZH, HIGH(P2_VS*2)
	ldi ZL, LOW(P2_VS*2)

	ldi XH, HIGH(VMEM)
	ldi XL, LOW(VMEM)
	
	ldi r16, 8
	subi XL, -1
P2_V_LOOP:
	lpm r17, Z+
	st X, r17
	subi XL, -4
	dec r16
	brne P2_V_LOOP

	pop XL
	pop XH
	pop ZL
	pop ZH
	ret


P1_VS:
	.db 0b11111111, 0b00001001, 0b00001001, 0b00001111, 0b00000000, 0b00010000, 0b11111000, 0b00000000
P2_VS:
	.db 0b11111111, 0b00001001, 0b00001001, 0b00001111, 0b00000000, 0b11101000, 0b10101000, 0b10111000
DRAW_VS:
	.db 0b11111111, 0b11111111, 0b11000011, 0b11000011, 0b01111110, 0b00111100, 0b00000000, 0b00000000