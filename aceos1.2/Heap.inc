;---------------------Ace OS source Code-----------------------
;		Created by Sam on 11-Aug-2002(samuelhard@yahoo.com)
; This file is a header file for Heap manager
;--------------------------------------------------------------
%ifndef HEAP_INC
%define HEAP_INC


EXTERN _AllocHeap
EXTERN _DeAllocHeap
%macro AllocHeap 1
	push dword %1
	call _AllocHeap
	add esp, 4
%endmacro 
%macro DeAllocHeap 1
	push dword %1
	call _DeAllocateHeap
	add esp, 4
%endmacro 

%endif