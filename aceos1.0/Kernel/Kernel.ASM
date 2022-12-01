;------------------------------------------------------
;               Ace Os Kernel Source Code
;                      Created by Sam 1-Jul-2002
;------------------------------------------------------
%include "Common.inc"
%include "MemMan.inc"
%include "VGAText.inc"
%include "Heap.inc"
%include "Processor.inc"
[SECTION .text]
[BITS 32]
NULLSELECTOR  EQU 0x0
CODESELECTOR  EQU 0x8
DATASELECTOR  EQU 0x10
STACKSELECTOR EQU 0x18
VGASELECTOR   EQU 0x20

EXTERN _SetExceptionHandlers
EXTERN _InitKeyboard
EXTERN _KeyboardHandler
EXTERN _InitTimer
EXTERN _InitPIC
EXTERN _StartCKernel
;----------------------------------------------------------------------------------------
;this is the start of kernel, OSLoader hands over the control to here
;----------------------------------------------------------------------------------------
mov eax, _DB
mov ax,STACKSELECTOR
mov ss,ax
mov ax,DATASELECTOR
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax

call _InitMemMan			; Initializing memory manager

ClearScreen
call _DetectProcessor

call _SetExceptionHandlers
call _InitPIC				; Initializing PIC to generate interrupts at IDT 32
call _InitTimer
push dword 0				; changing the EFlags register IOPL to 0 ( only kernel can access the IO ports )
popf

sti
call _StartCKernel

jmp $   ; endless loop
global _DB
_DB:
	ret