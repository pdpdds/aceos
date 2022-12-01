;------------------------------------------------------
;               Mouse routines for Ace Os
;                      Created by Sam 06-Sep-2002
;------------------------------------------------------
%include "Processor.inc"
%include "HMess.inc"

[SECTION .text]
[BITS 32]
GLOBAL _MouseHandler
EXTERN _SetIDT
EXTERN _SysMouseIOPort
EXTERN _Sys_MouseHardwareID

;this ISR is responsible for sending Hardware messages when mouse data received
_MouseHandler:	
	cli
	
	push eax
	push edx
	push ds
	
	mov eax, DATA_SELECTOR
	mov ds, ax

	mov dx, [_SysMouseIOPort] 
	in  al, dx 
	
	PutHardwareMessage [_Sys_MouseHardwareID],0,eax,eax
	
	.Done:
	mov al,0x20		;EOI
	out 0x20,al
	out 0xA0,al
	
	pop ds
	pop edx
	pop eax
	
	sti
	
	iret
