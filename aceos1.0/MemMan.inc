;---------------------Ace OS source Code-----------------------
;		Created by Sam on 10-Aug-2002(samuelhard@yahoo.com)
; This file is a header file for Memory manager
;--------------------------------------------------------------
%ifndef MEMMAN_INC
%define MEMMAN_INC


EXTERN _MemoryAmount
EXTERN _InitMemMan
EXTERN _AllocateMemory
%macro AllocateMemory 1
	push dword %1
	call _AllocateMemory
	add esp, 4
%endmacro 

%endif