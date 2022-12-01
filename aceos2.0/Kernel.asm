;------------------------------------------------------
;               Ace Os Kernel Source Code
;                      Created by Sam 1-Jul-2002 (samueldotj@gmail.com)
;------------------------------------------------------
%include "Common.inc"

EXTERN _InitMemManPhaseI
EXTERN _StartCKernel

GLOBAL _KernelEntry
GLOBAL _DEBUG

MULTIBOOT_PAGE_ALIGN   equ 1<<0
MULTIBOOT_MEMORY_INFO  equ 1<<1
MULTIBOOT_AOUT_KLUDGE  equ 1<<16

MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
CHECKSUM               equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
VIRTUAL_BASE_ADDRESS   equ 0xC0000000
PHYSICAL_ADDRESS       equ 0x100000
extern _sbss 
extern _ebss


[SECTION .text]
[BITS 32]
; The Multiboot header
align 4
mboot:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd CHECKSUM
    ; fields used if MULTIBOOT_AOUT_KLUDGE is set in MULTIBOOT_HEADER_FLAGS
    dd mboot - VIRTUAL_BASE_ADDRESS	+ PHYSICAL_ADDRESS			; these are PHYSICAL addresses
    dd PHYSICAL_ADDRESS					; start of kernel .text (code) section
    dd _sbss - VIRTUAL_BASE_ADDRESS + PHYSICAL_ADDRESS			; end of kernel .data section
    dd _ebss - VIRTUAL_BASE_ADDRESS + PHYSICAL_ADDRESS			; end of kernel BSS
    dd _KernelEntry - VIRTUAL_BASE_ADDRESS + PHYSICAL_ADDRESS	; kernel entry point (initial EIP)
    
    align 4
_KernelEntry:
    
    ;setting IOPL
    push dword 0x2 ;0x3002 
    popf
    
    mov eax, _DEBUG + PHYSICAL_ADDRESS - VIRTUAL_BASE_ADDRESS
    
    mov edi, _sbss                       ; Zeroing BSS section
    mov ecx, _ebss
    sub ecx, edi
    add edi, PHYSICAL_ADDRESS - VIRTUAL_BASE_ADDRESS
    xor eax, eax
    rep stosb
    
    mov [lpMultiBootInfo], ebx
    
    push ebx
    push dword mboot + PHYSICAL_ADDRESS - VIRTUAL_BASE_ADDRESS
    call _InitMemManPhaseI			        ;Initializing memory manager- returns Page Directory in eax
    
    mov cr3, eax                                    ;Store in Control Reg 3
    mov eax, cr0				        ;Get Control Reg 0
    or  eax, 10000000000000000000000000000001b	;Set paging bit ON
    mov cr0, eax				        ;Store Control Reg 0
    
    jmp short PageEnabled			        ;Clearing prefetch queue
PageEnabled:

    mov esp, ebx                                    ;Store the new Stack position(calculated during InitMemManPhaseI)
    
    jmp JMP_3_GB + VIRTUAL_BASE_ADDRESS - PHYSICAL_ADDRESS
JMP_3_GB:                                       

;mov ebx, [lpMultiBootInfo]
    push dword [lpMultiBootInfo]
    push dword mboot + PHYSICAL_ADDRESS - VIRTUAL_BASE_ADDRESS
    call _StartCKernel

    jmp $   				; endless loop ***** should not be reached

_DEBUG: 
    ret
    
[SECTION .data]
lpMultiBootInfo dd 0