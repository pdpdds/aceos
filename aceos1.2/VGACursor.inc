;---------------------Ace OS source Code-----------------------
;		Created by Sam on 19-Nov-2002(samuelhard@yahoo.com)
; This file is a header file for VGACursor rountines
;--------------------------------------------------------------
%ifndef VGATEXT_INC
%define VGATEXT_INC
EXTERN _SetCursorType
EXTERN _MoveCursor

%macro SetCursorType 2
	push dword %1
	push dword %2
	call _SetCursorType
	add esp, 8
%endmacro 
%macro MoveCursor 2
	push dword %1
	push dword %2
	call _MoveCursor
	add esp, 8
%endmacro 

%endif