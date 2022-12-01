;------------------------------------------------------
;               Memory Manager for Ace Os
;                      Created by Sam 03-Aug-2002
;						Redesigned on 07-Oct-2002
; This file can be completed removed or replaced by a c file
;------------------------------------------------------
%include "Common.inc"
[BITS 32]
[SECTION .text]
[GLOBAL _DetectMemory]
_DetectMemory:
    push edi
    mov edi, 2*1024*1024		; This will detect the amount of RAM present in the system the memory scan starts from 2MB since the minimum req is 2 MB
    .Detect:
	mov byte [ds:edi], 0x0f		; writing and comparing a bit pattern
	cmp byte [ds:edi], 0x0f
	jne .DetectDone
	mov byte [ds:edi], 0xf0		; writing and comparing a bit pattern
	cmp byte [ds:edi], 0xf0
	jne .DetectDone

	add edi,1024			; Next Kilobyte

	jmp .Detect
    .DetectDone:	                ; now the edi contains the amount of RAM installed in bytes
    mov eax, edi                        ; returning the result
    
    pop edi
    ret
    
[SECTION .data]
    
[GLOBAL _mm_PhysicalTotal]
[GLOBAL _mm_PhysicalAvailable]

[GLOBAL _mm_PhysicalBitmap]
[GLOBAL _mm_PhysicalBitmapSize]

_mm_PhysicalTotal	dd 0		; amount of RAM installed in the system in MB
_mm_PhysicalAvailable   dd 0

_mm_PhysicalBitmap      dd 0            ; pointer to the RAM Free Bitmap
_mm_PhysicalBitmapSize  dw 0            ; size of the RAM Free Bitmap in Pages
