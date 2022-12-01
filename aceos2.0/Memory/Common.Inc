;---------------------Ace OS source Code-----------------------
;		Created by Sam on 10-Aug-2002(samueldot@gmail.com)
; This file contains various macros required by the Ace Kernel
;--------------------------------------------------------------
;the following two macros useful when writing functions which is using
;C standard parameter passing 
%ifndef COMMON_INC
%define COMMON_INC

%define CODE_SELECTOR 0x8
%define DATA_SELECTOR 0x10
%define STACK_SELECTOR 0x18
%define VGA_SELECTOR 0x20

EXTERN _Beep
EXTERN _IPC_PostMessage
EXTERN _GetGDTEntry
EXTERN _SetGDTEntry
EXTERN _GetIDTEntry
EXTERN _SetIDTEntry


KernelAddress EQU 0xC0000000
KernelPhysicalAddress EQU 0x100000

%macro prologue 1 
	push ebp 
	mov ebp,esp 
	sub esp,%1 
%endmacro 

%macro epilogue 0
	mov esp,ebp 
	pop ebp 
%endmacro 

;this macro will useful when you want to make MSB and LSB of previous mul operation into EAX reg
%macro Mul16EAX 1
	push edx
	mul %1
	xchg eax,edx		; now moving the MSB in dx to eax
	shl eax,16
	mov ax,dx
	pop edx
%endm

%macro Mul32EAX 1
	push ecx
	mov ecx, %1
	mul ecx
	pop ecx
%endm

;input Dividend , divisor
;output : quotient - ax , modulo - dx
%macro Div16EAX 2
	push ecx

	mov eax, %1
	mov edx, %1
	shr edx, 16
	mov ecx, %2
	div cx
	
	pop ecx
%endm

;make beep using sound function
%macro Beep 0
	call _Beep
%endmacro

;device drivers must use this function to post messages
;input hMessageBox, MessageID, Para1, Para2
%macro IPC_PostMessage 4
	push dword %4
	push dword %3
	push dword %2
	push dword %1
	call _IPC_PostMessage
	add esp,16
%endmacro

%endif