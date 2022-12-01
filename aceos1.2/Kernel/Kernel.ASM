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

EXTERN _DataTest

GLOBAL _KernelEntry

GLOBAL _DEBUG
GLOBAL _TEST

EXTERN DEBUG
EXTERN _Printf
;----------------------------------------------------------------------------------------
;this is the start of kernel, OSLoader hands over the control to here
;----------------------------------------------------------------------------------------
_KernelEntry:

mov eax,_InitMemMan
mov ax,STACKSELECTOR			; Setting up new stack
mov ss,ax				;********try LSS instruction
mov esp,24*4096

mov ax,DATASELECTOR			; loading segment registers with data selector
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax

mov eax, _DataTest
mov eax, [_DataTest]


call _DEBUG
call _InitMemMan			; Initializing memory manager- returns Page Directory in eax
mov cr3, eax				;Store in Control Reg 3
mov eax, cr0				;Get Control Reg 0
or eax, 10000000000000000000000000000001b	;Set paging bit ON
mov cr0, eax				;Store Control Reg 0
jmp short PageEnabled			;Clearing prefetch queue
PageEnabled:

call _DEBUG
ClearScreen
call _DEBUG

call _DetectProcessor			; Detect processor features and preserve result

call _SetExceptionHandlers
call _InitPIC				; Initializing PIC to generate interrupts at IDT 32

;push dword 0				; changing the EFlags register IOPL to 0 ( only kernel can access the IO ports )
;popf					; change this code



call _StartCKernel

jmp $   				; endless loop ***** should not be reached
_DEBUG:
	mov eax,0
	ret
_TEST:
	iret

