;------------------------------------------------------
;		ACE OS Source Code
;	Created by sam (samuelhard@yahoo.com)
;	80 X 25 Text Mode VGA Routines
;------------------------------------------------------
%include "Common.inc"
[SECTION .text]
[BITS 32]

VGASelector EQU 0x20
;---------Constants
MAX_COL    EQU 80
MAX_ROW    EQU 25


VGA_CRT_ADDRESS EQU 0x3D4
VGA_CRT_DATA    EQU 0x3D5

VGA_CRT_CURSOR_START EQU 0x0A
VGA_CRT_CURSOR_END   EQU 0x0B
VGA_CRT_H_START_ADDRESS EQU 0x0C
VGA_CRT_H_END_ADDRESS   EQU 0x0D
VGA_CRT_CURSOR_H_LOCATION EQU 0x0E
VGA_CRT_CURSOR_L_LOCATION EQU 0x0F

;---------Variables
CurRow db 0
CurCol db 0
CurAttribute db 0x1
CursorBottom db 14
CursorTop db 13

;---------global functions
GLOBAL _SetTextAttribute
GLOBAL _GetTextAttribute
GLOBAL _PrintCharacter
GLOBAL _PutCh
GLOBAL _PrintString
GLOBAL _Print
GLOBAL _Fill
GLOBAL _InsertRow

;sets the current video attribute
;Input  - Attribute
;Output - Attribute
_SetTextAttribute:
	prologue 4
	mov eax, [ebp+4]
	mov byte [CurAttribute], al
	epilogue
	ret
;returns the current video attribute
;Input  - None
;Output - Attribute
_GetTextAttribute:
	mov eax, 0
	mov al, [CurAttribute]
	ret


;prints the character using the attribute specified
;Input - Row, Col, Character in asm C type proto PrintCharacter(Character,Row,Col)
;Output - None
_PrintCharacter:
	prologue 12	

	push eax
	push es
	push di

	mov ax, VGASelector	; Selecting the VGA Memory Selector
	mov es, ax
	
	mov ax, 160		; Getting the VGA memory offset
	mul byte [ebp+16]	; Row * 160
	mov di,ax		; Preserve result
	mov al, 2
	mul byte [ebp+12]	; Col * 2
	add di, ax		; (Col * 2)+ (Row * 160)

	mov al, [ebp+8]		; Character

	mov byte [es:di],al	; moving to the VGA memory
	inc di
	mov al, [CurAttribute]
	mov byte [es:di],al	; moving to the VGA memory

	pop di
	pop es
	pop eax
	
	epilogue
	ret
;prints the character at the current cursor possition, using default attribute specified
;Input  - Character
;Output - None
_PutCh:
	prologue 4

	push eax
	push es
	push di

	mov ax, VGASelector	; Selecting the VGA Memory Selector
	mov es, ax
	
	mov ax, 160		; Getting the VGA memory offset
	mul byte [CurRow]	; Row * 160
	mov di,ax		; Preserve result
	mov al, 2
	mul byte [CurCol]	; Col * 2
	add di, ax		; (Col * 2)+ (Row * 160)

	mov al, [ebp+8]		; Character
	cmp al, 10
	je	.ColReset
	cmp al, 13
	je	.NextRow

	mov byte [es:di],al	; moving to the VGA memory
	inc di
	mov al, [CurAttribute]
	mov byte [es:di],al	; moving to the VGA memory


	;cursor management
	inc byte [CurCol]
	cmp byte [CurCol],80		
	jb .Done
	.NextRow:
		inc byte [CurRow]
		mov byte [CurCol],0
		cmp byte [CurRow], 24
		jb .Done
		mov byte [CurRow], 24
		call _InsertRow
		jb .Done
	.ColReset:
		mov byte [CurCol],0
	.Done:

	pop di
	pop es
	pop eax
	
	epilogue
	ret
	
	

;Fills the character/Attribute in the screen
;Specify 'C' - in the C/A argument to fill character otherwise it will fill Color Attribute
;Input - RowStart, ColStart, RowEnd ,ColEnd, Character/Attribute, C/A
;Output - None
_Fill:
	prologue 24

	pusha
	push es
	push di

	mov ax, VGASelector	; Selecting the VGA Memory Selector
	mov es, ax
	
	mov ebx, [ebp+8]        ; Row start
	.RowLoop:
		cmp ebx,[ebp+16]	 ; Row end
		ja .RowLoopExit
		mov ecx, [ebp+12]	 ; Col start
		.ColLoop: 
			cmp cl, [ebp+20] ; col end
			ja .ColLoopExit
			;actual printing
			mov al, 160
			mul bl
			mov di, ax
			mov al,2
			mul cl
			add di, ax
			cmp byte [ebp+28], 'C'   ;type
			je .Character
			inc di
			.Character:
			mov al, [ebp+24]       ;character/attribute
			mov byte [es:di],al
			inc cx 			; next character
			jmp .ColLoop
		.ColLoopExit:
		inc bx
		jmp .RowLoop
	.RowLoopExit:

	pop di
	pop es
	popa

	epilogue
	ret
;prints the string using the attribute specified
;Input - Row, Col, string pointer
;Output - None
_PrintString:
	prologue 12
	
	push eax
	push es
	push edi
	push esi

	mov ax, VGASelector	; Selecting the VGA Memory Selector
	mov es, ax
	
	mov ax, 160		; Getting the VGA memory offset
	mul byte [ebp+16]	; Row * 160
	mov di,ax		; Preserve result
	mov al, 2
	mul byte [ebp+12]	; Col * 2
	add di, ax		; (Col * 2)+ (Row * 160)
				
	mov esi,[ebp+8]	; string pointer 

	mov al, [CurAttribute]
	cld
	.StrLoop:
		lodsb
		cmp al,0
		je .Done
		mov byte [es:di],al	; moving from data segment to vga
		inc di
		cmp di, 4000
		ja .Done
		mov al, [CurAttribute]
		mov byte [es:di],al	; moving attribute to the VGA memory
		inc di
		jmp .StrLoop
	.Done:	

	pop esi
	pop edi
	pop es
	pop eax

	epilogue 
	ret

;prints the string at col, row using current attribute
;Input - String pointer
;Output - None
_Print:
	prologue 4

	push eax
	push es
	push edi
	push esi

	mov ax, VGASelector	; Selecting the VGA Memory Selector
	mov es, ax

	mov esi,[ebp+8]		; string pointer 
	
	mov ax, 160		; Getting the VGA memory offset
	mul byte [CurRow]	; Row * 160
	mov di,ax		; Preserve result
	mov al, 2
	mul byte [CurCol]	; Col * 2
	add di, ax		; (Col * 2)+ (Row * 160)
				

	mov al, [CurAttribute]
	cld
	.StrLoop:
		lodsb
		cmp al,13
		je .NextRow
		cmp al,10
		je .ColReset
		cmp al,0
		je .Done
		mov byte [es:di],al	; moving from data segment to vga
		inc di
		cmp di, 4000
		ja .Done
		mov al, [CurAttribute]
		mov byte [es:di],al	; moving attribute to the VGA memory
		inc di

		;cursor management
		inc byte [CurCol]
		cmp byte [CurCol],80		
		jb .ResetDone
		.NextRow:
			inc byte [CurRow]
			cmp byte [CurRow], 24
			jb .RowDone
			mov byte [CurRow],0
			call _InsertRow
		.RowDone:
		.ColReset:
			mov byte [CurCol],0
			
			mov ax, 160		; Getting the VGA memory offset
			mul byte [CurRow]	; Row * 160
			mov di,ax		; Preserve result
			mov al, 2
			mul byte [CurCol]	; Col * 2
			add di, ax		; (Col * 2)+ (Row * 160)

		.ResetDone:
		jmp .StrLoop
	.Done:	

	pop esi
	pop edi
	pop es
	pop eax

	epilogue
	ret
;Inserts a blank row below the last line
;Input - None
;Output - None
_InsertRow:
	push es
	push gs
	push edi
	push esi
	push eax
	push ecx
	push edx

	cld	; direction
	
	mov dl,[CurAttribute]
	;copying 2-25 lines in 1-24 lines

	mov ax, VGASelector
	mov gs, ax
	mov es, ax
	mov esi, 160		; 2nd row
	mov edi, 0			; 1st row
	mov ecx, 3840		
	
	.LoopCopy:
	mov eax, [gs:esi]
	mov dword [es:edi], eax
	add edi,4
	add esi,4
	sub ecx,4
	jnz .LoopCopy

	mov al, ' '				; character
	mov ah, dl				; assigning attribute already preserved in dl
	mov ecx,160				; one row
	rep stosw
	
	mov byte [CurRow],24
	mov byte [CurCol],0	
	
	pop edx
	pop ecx
	pop eax	
	pop esi
	pop edi
	pop gs
	pop es
	ret
 	
	
	