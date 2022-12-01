;------------------------------------------------------
;               Keyboard routines for Ace Os
;                      Created by Sam 20-Jul-2002
;------------------------------------------------------
%include "Processor.inc"
%include "HMess.inc"

[SECTION .text]
[BITS 32]
GLOBAL _KeyboardHandler
EXTERN _SetIDT
EXTERN _Sys_KeyboardHardwareID


OnboardController	EQU	64h
KeyboardController 	EQU	60h
;this ISR is responsible for filling the Keyboard Scan codes raised by the Keyboard in the KeyCodes variable
_KeyboardHandler:	
	cli
	
	push eax
	push ds
	
	mov eax, DATA_SELECTOR
	mov ds, ax

	in al, OnboardController	;reading the status of 8042
	and al, 000000010b
	cmp al, 0
	jne .Done
	in al, KeyboardController	;reading the keyboard data
	
	PutHardwareMessage [_Sys_KeyboardHardwareID],0,eax,eax
	
	.Done:
	mov al,0x20		;EOI
	out 0x20,al
	out 0xA0,al
	
	pop ds
	pop eax
	
	sti
	
	iret
