;------------------------------------------------------
;               Keyboard routines for Ace Os
;                      Created by Sam 20-Jul-2002
;------------------------------------------------------
%include "Common.inc"
[SECTION .text]
[BITS 32]

extern _KeyboardHand
%macro KeyboardHandler 3
    push dword %3
    push dword %2
    push dword %1
    call _KeyboardHand
    add esp, 12
%endmacro

GLOBAL _KeyboardHandler
EXTERN _sysKeyboardMB

OnboardController	EQU	64h
KeyboardController 	EQU	60h
;this ISR is responsible for filling the Keyboard Scan codes raised by the Keyboard in the KeyCodes variable
extern _DEBUG
_KeyboardHandler:	
    cli
    call _DEBUG
    push eax
    push ds
    
    mov eax, DATA_SELECTOR      
    mov ds, eax
    
    xor eax, eax                ; zero eax
    
    in al, OnboardController	;reading the status of 8042
    and al, 000000010b
    cmp al, 0
    jne .Done
    in al, KeyboardController	;reading the keyboard data
    
    ;IPC_PostMessage [_sysKeyboardMB],0,eax,eax
    KeyboardHandler 0, eax, eax
    
    .Done:
    mov al,0x20		;EOI
    out 0x20,al
    out 0xA0,al
    
    pop ds
    pop eax
    
    sti
    iret
