%include "Common.inc"
%include "VGAText.inc"
%include "Processor.inc"

;Ace OS Source Code - Memory Move / Copy Functions
[SECTION .text]
[BITS 32]

GLOBAL _MoveData
;copies the source data to the destination 
;Input - Dest, Source , Length
;Output - None
_MoveData:
	prologue 12	

	mov edi,[ebp+8]			;Load destination address
	mov esi,[ebp+12]		;Load source address
	mov ecx,[ebp+16]		;Load count of bytes to move
	cld						;Auto incrementing move
	shr ecx,1				;Check odd byte
	jnc .NoByteC
	movsb					;Handle the odd byte
.NoByteC:
	shr ecx,1				;Check odd word
	jnc .NoWordC
	movsw					;Handle the odd word
.NoWordC:
	rep movsd				;Move all DWORDS that are left
	epilogue 
	ret