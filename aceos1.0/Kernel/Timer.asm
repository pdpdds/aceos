%include "Common.inc"
%include "VGAText.inc"
%include "Processor.inc"

[SECTION .text]
[BITS 32]

GLOBAL _TimerHandler
GLOBAL _InitTimer
GLOBAL _GetTimerTick
GLOBAL _MDelay

TMR_CONTROL  EQU 43h
TMR_COUNTER0 EQU 40h
TMR_COUNTER1 EQU 41h
TMR_COUNTER2 EQU 42h

TimerValue  dd 1,0,0
tmpMsg db '                     ',0
;-----------------------------------------------------------
;this function intialize the PIT 8254. It must be called by the kernel only once
_InitTimer:
	push eax

	mov al, 00110110B ; control register
	out TMR_CONTROL, al
	mov al, 10101001B ; LSB of clock divisor
	out TMR_COUNTER0, al
	mov al, 00000100B ; MSB of clock divisor
	out TMR_COUNTER0, al
	
	mov al, 01010100B ; control register
	out TMR_CONTROL, al
	mov al, 18 ; LSB only clock divisor
	out TMR_COUNTER1, al

	push dword _TimerHandler
	push dword 32
	call _SetIDT
	add esp,8
	
	pop eax
	ret
;-----------------------------------------------
;This is the default kernel handler for the Timer interrupt
;----------------------------------------------
_TimerHandler:
	cli
	pusha
	
	push ds
	mov ax, DATA_SELECTOR
	mov ds, ax
	
	inc dword [TimerValue]

	ULongToString 10,tmpMsg,[TimerValue]
	PrintString 0,70,tmpMsg

	mov al,0x20		; End of Interrupt to PIC
	out 0x20,al
	out 0xA0,al
	
	pop ds
	popa
	sti
	iret
;--------------------------------------------------
;returns the time tick variable content
_GetTimerTick:
	mov eax, [TimerValue]
	ret
	
_MDelay:
	prologue 4
	push eax
	push ecx
	
	mov eax, [ebp+8]		; arugment passed
	mov ecx, [TimerValue]
	.Count:
		cmp eax, 0
		je .Done	
		cmp ecx, [TimerValue]
		jl .DecCounter
		jmp .Count
		.DecCounter:
			dec eax
			mov ecx,[TimerValue]
		jmp .Count
	.Done:
	
	pop ecx
	pop eax
	epilogue	
	ret
