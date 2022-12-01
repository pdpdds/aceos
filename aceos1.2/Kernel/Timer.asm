%include "Common.inc"
%include "VGAText.inc"
%include "Processor.inc"
%include "HMess.inc"

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

EXTERN _Sys_TimerHardwareID
EXTERN _TimerTickStatus
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
	inc word [TimerInterval]
	
	cmp word [TimerInterval], 100
	jne .ResetIntervalDone
	mov word [TimerInterval], 0
	.ResetIntervalDone:
	
	cmp word [TimerInterval],0
	jne .Done

	cmp byte [_TimerTickStatus],0
	je .Done
	PutHardwareMessage [_Sys_TimerHardwareID],0,eax,eax

	.Done:
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

TimerValue  	dd 1,0,0
TimerInterval 	dw 0
