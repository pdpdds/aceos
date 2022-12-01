;---------------------Ace OS source Code-----------------------
;		Created by Sam on 10-Aug-2002(samuelhard@yahoo.com)
; This file is a header file for Memory manager
;--------------------------------------------------------------
%ifndef MEMMAN_INC
%define MEMMAN_INC

EXTERN _InitMemMan
EXTERN _GetRAMHardwareAddress
;Input  : PageDirectoryAddress, Linear Address
;Output : RAMAddress
%macro GetRAMHardwareAddress 2
	push %1
	push %2 
	call _GetRAMHardwareAddress
	add esp, 8
%endmacro 


%endif