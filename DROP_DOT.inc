 /*
 * IncFile3.inc
 *
 *  Created: 2024-02-27 10:20:58
 *   Author: ahmha095
 */ 
 ldi r16, (1<<PD1) | (1<<PD0)
out DDRD, r16

DROP_DOT:
	//PX är rest från SET_PIX, vilket funkar utmärkt
	
	//call GET_COLUMN
	lds r25, PY //Placeholder för spelarens PY
	
	call GET_COLUMN

	sbrs r18, 7
	rjmp PUT_PIX7

	sbrs r18, 6
	rjmp PUT_PIX6

	sbrs r18, 5
	rjmp PUT_PIX5

	sbrs r18, 4
	rjmp PUT_PIX4

	sbrs r18, 3
	rjmp PUT_PIX3

	sbrs r18, 2
	rjmp PUT_PIX2

	sbrs r18, 1
	rjmp PUT_PIX1 

	rjmp DROP_END

PUT_PIX7:
	ldi r16, 7
	rjmp DROP_END

PUT_PIX6:
	ldi r16, 6
	rjmp DROP_END
	
PUT_PIX5:
	ldi r16, 5
	rjmp DROP_END

PUT_PIX4:
	ldi r16, 4
	rjmp DROP_END

PUT_PIX3:
	ldi r16, 3
	rjmp DROP_END

PUT_PIX2:
	ldi r16, 2
	rjmp DROP_END

PUT_PIX1:
	ldi r16, 1
	rjmp DROP_END

PUT_PIX0:
	ldi r16, 0
	rjmp DROP_END
	
DROP_END:
	sts PY, r16
	sts CY,r16
	lds r16,PX
	sts CX,r16
	call SET_PIX
	
	sts PY, r25
	call OFF_PIX
	call Drop_Sound

	lds r16, FULL_COUNTER
	inc r16
	sts FULL_COUNTER, r16

	ret
	
 Drop_Sound:
	call SOUND_DROP
	ret

SOUND_DROP:
	sbi PORTB, PB1

	cbi PORTB, PB1

	ret