;---------------------Ace OS source Code-----------------------
;		Created by Sam on 10-Aug-2002(samuelhard@yahoo.com)
;		Hardware Messaging
;--------------------------------------------------------------
%ifndef HMESS_INC
%define HMESS_INC

%include "common.inc"

EXTERN _PutHardwareMessage
EXTERN _GetNextHardwareMessage


struc HMessage
	.HardwareID: resb 1
	.SubSystem: resw 1	
	.Para1: resd 1
	.Para2: resd 1
	.Reserved: resb 4
endstruc

;device drivers must use this function to messages
;input HardwareID, SubSystem, Para1, Para2
%macro PutHardwareMessage 4
	push dword %4
	push dword %3
	push dword %2
	push dword %1
	call _PutHardwareMessage
	add esp,16

%endmacro

;kernel uses this function to read the messages posted by device drivers
;returns the pointer to structure HMessages
%macro GetNextHardwareMessage 0
	call _GetNextHardwareMessage
%endmacro

%endif