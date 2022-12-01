;---------------------Ace OS source Code-----------------------
;		Created by Sam on 10-Aug-2002(samuelhard@yahoo.com)
; This file is a header file for VGAText, VGACursor rountines
;--------------------------------------------------------------
%ifndef VGATEXT_INC
%define VGATEXT_INC


EXTERN _PrintCharacter
EXTERN _PutCh
EXTERN _PrintString
EXTERN _Print
EXTERN _Fill
EXTERN _InsertRow


%macro ClearScreen 0 
	push dword 'C'
	push dword ' '
	push dword 79
	push dword 24
	push dword 0
	push dword 0
	call _Fill
	add esp, 24
	push dword 'A'
	push dword 0x1
	push dword 79
	push dword 24
	push dword 0
	push dword 0
	call _Fill
	add esp, 24
%endmacro 

%macro PrintCharacter 3
	push dword %1
	push dword %2
	push dword %3
	call _PrintCharacter
	add esp, 12
%endmacro 

%macro PutCh 1
	push dword %1
	call _PutCh
	add esp, 4
%endmacro 

%macro PrintString 3
	push dword %1
	push dword %2
	push dword %3
	call _PrintString
	add esp, 12
%endmacro 

%macro Fill 6
	push dword %1
	push dword %2
	push dword %3
	push dword %4
	push dword %5
	push dword %6
	call _Fill
	add esp, 24
%endmacro 

%macro Print 1
	push dword %1
	call _Print
	add esp, 4
%endmacro 

%macro InsertRow 0
	call _InsertRow
%endmacro 


%endif