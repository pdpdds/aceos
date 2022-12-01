;---------------------Ace OS source Code-----------------------
;		Created by Sam on 10-Aug-2002(samuelhard@yahoo.com)
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

EXTERN _UIntToString
EXTERN _ULongToString
EXTERN _Beep

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
%macro Mul32EAX 0
	xchg eax,edx		; now moving the MSB in dx to eax
	shl eax,16
	mov ax,dx
%endm

;this macro provide an easy way to access the function defined in C
;UIntToString Base,StringBuffer,Value
%macro UIntToString 3
	push dword %1
	push dword %2
	push dword %3
	call _UIntToString
	add esp, 12
%endmacro 
;ULongToString Base,StringBuffer,Value
%macro ULongToString 3
	push dword %1
	push dword %2
	push dword %3
	call _ULongToString
	add esp, 12
%endmacro 
;make beep using sound function
%macro Beep 0
	call _Beep
%endmacro


%endif