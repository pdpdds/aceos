;----------------------------------------------------------------------------
; Programmable Interrupt Controller and Interrupt related routines for Ace Os
;                      Created by Sam 10-Aug-2002
;----------------------------------------------------------------------------
[BITS 32]
[SECTION .text]
PICU1          EQU 0020h
PICU2          EQU 00A0h
GLOBAL _InitPIC
_InitPIC:
	mov al,00010001b
	out PICU1+0,al          	;ICW1 - MASTER

	mov al,20h
	out PICU1+1,al          	;ICW2 - MASTER
	mov al,00000100b
	out PICU1+1,al          	;ICW3 - MASTER
	mov al,00000001b
	out PICU1+1,al          	;ICW4 - MASTER
	mov al,0			;No Mask
	out PICU1+1,al          	;MASK - MASTER (0= Ints ON)

	mov al,00010001b
	out PICU2+0,al          	;ICW1 - SLAVE
	mov al,28h
	out PICU2+1,al          	;ICW2 - SLAVE
	mov al,00000010b
	out PICU2+1,al          	;ICW3 - SLAVE
	mov al,00000001b
	out PICU2+1,al          	;ICW4 - SLAVE
	mov al,0
	out PICU2+1,al          	;MASK - SLAVE
	ret