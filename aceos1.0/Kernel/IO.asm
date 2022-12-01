;------------------------------------------------------
;               IO functions for Ace Os
;                      Created by Sam on 14-June-2002 (samuelhard@yahoo.com)
;------------------------------------------------------
%include "Common.inc"
[SECTION .text]
[BITS 32]
[GLOBAL _InPortByte]
[GLOBAL _InPortWord]
[GLOBAL _InPortDWord]

[GLOBAL _OutPortByte]
[GLOBAL _OutPortWord]
[GLOBAL _OutPortDWord]

;------------------------------------------------------------------
; C Proto type  BYTE InPortByte(WORD Port)
;
;------------------------------------------------------------------
_InPortByte:
	prologue 4
        
	mov dx, [ebp + 8 ]
	in  al, dx

	epilogue
	ret

;------------------------------------------------------------------
; C Proto type  WORD InPortWord(WORD Port)
;
;------------------------------------------------------------------
_InPortWord:
	prologue 4

	mov dx, [ebp + 8 ]
	in  ax, dx

	epilogue
	ret
;------------------------------------------------------------------
; C Proto type  DWORD InPortDWord(WORD Port)
;
;------------------------------------------------------------------
_InPortDWord:
	prologue 4

	mov dx, [ebp + 8 ]
	in  eax, dx

	epilogue
	ret
;------------------------------------------------------------------
; C Proto type  void OutPortByte(WORD Port,BYTE Value)
;
;------------------------------------------------------------------
_OutPortByte:
	prologue 8

	mov dx, [ebp + 8 ]
	mov al, [ebp + 12 ]
	out  dx, al
	
	epilogue
	ret

;------------------------------------------------------------------
; C Proto type  void OutPortWord(WORD Port, WORD Value)
;
;------------------------------------------------------------------
_OutPortWord:
	prologue 8
	mov dx, [ebp + 8 ]
	mov ax, [ebp + 12 ]
	out dx, ax
	epilogue
	ret
;------------------------------------------------------------------
; C Proto type  void OutPortDWord(WORD Port,DWORD Value)
;
;------------------------------------------------------------------
_OutPortDWord:
	prologue 8
	
	mov dx, [ebp + 8 ]
	mov eax, [ebp + 12 ]
	out dx, eax

	epilogue
	ret