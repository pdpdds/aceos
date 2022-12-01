;---------------------------------FLOPPY INTERRUPT HANDLER--------------------------------
;                                             Ace OS Source Code
;                                                               samueldotj@yahoo.co.in
;--------------------------------------------------------------------------------------------------------------------------------
%include "Common.inc"

[SECTION .text]
[BITS 32]
GLOBAL _FloppyInterruptHandler
EXTERN _FDD_InterruptReceived

align 32
_FloppyInterruptHandler:
    
    mov byte [_FDD_InterruptReceived], 1  ; Set the interrupt received flag
    
    mov al,0x20		                ; End of Interrupt to PIC
    out 0x20,al

    iret                                ; interrupt return

