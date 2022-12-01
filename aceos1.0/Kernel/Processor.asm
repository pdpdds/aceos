;------------------------------------------------------
;               Processor Manager for Ace Os
;                      Created by Sam 10-Aug-2002
;------------------------------------------------------
%include "Common.inc"
%include "VGAText.inc"

[BITS 32]
[SECTION .text]
GLOBAL _DetectProcessor
GLOBAL _ExceptionHandler
GLOBAL _SetIDT
GLOBAL _GetIDT
GLOBAL _SetExceptionHandlers


GLOBAL __CPU_ProcessorID
GLOBAL __CPU_VendorID
GLOBAL __CPU_Signature
GLOBAL __CPU_Features_EBX
GLOBAL __CPU_Features_EDX
GLOBAL __CPU_Features_ECX


IDTAddress		EQU 0x800+0x37
KernelAddress 	EQU 0x19000

__CPU_ProcessorID  	db '386/486     ',0
__CPU_VendorID	  	db 'GenuineIntel',0
__CPU_Signature 	dd 0
__CPU_Features_EBX 	dd 0
__CPU_Features_EDX 	dd 0
__CPU_Features_ECX 	dd 0

Intel_ID	db 'GenuineIntel',0
UMC_ID		db 'UMC UMC UMC ',0
AMD_ID		db 'AuthenticAMD',0
Cyrix_ID	db 'CyrixInstead',0

;this function will change the specified Interrrupt handler
;SetIDT(BYTE InterruptNo, void (* Handler));
tmpMsg1 db 13,10,'                 ',0
_SetIDT:
	prologue 8
	push eax
	push ecx
	push ds
	
	mov ax, DATA_SELECTOR
	mov ds, ax
	
	mov cl, 8
	mov eax, [ebp+8]
	mul cl
	add eax, IDTAddress
	
	mov ecx, [ebp+12]
	sub ecx, KernelAddress
	mov [ds:eax], cx		;low order
	shr ecx,16
	mov word [ds:eax+6],cx		; high order
	
	mov byte [ds:eax+5], 0x8e   ;type (32-bit Ring 0 interrupt gate)
	mov word [ds:eax+2], CODE_SELECTOR
	mov byte [ds:eax+4], 0

	pop ds
	pop ecx
	pop eax
	
	epilogue
	ret
_GetIDT:
	prologue 4
	
	epilogue
	ret
	
_SetExceptionHandlers:
	push dword _DivideByZero
	push dword 0
	call _SetIDT
	add esp, 8
	
	push dword _Debug
	push dword 1
	call _SetIDT
	add esp, 8
	
	push dword _NMI
	push dword 2
	call _SetIDT
	add esp, 8
	
	push dword _BreakPoint
	push dword 3
	call _SetIDT
	add esp, 8
	
	push dword _Overflow
	push dword 4
	call _SetIDT
	add esp, 8
	
	push dword _Bound
	push dword 5
	call _SetIDT
	add esp, 8
	
	push dword _IVOpcode
	push dword 6
	call _SetIDT
	add esp, 8
	
	push dword _DevNA
	push dword 7
	call _SetIDT
	add esp, 8
	
	push dword _DoubleFault
	push dword 8
	call _SetIDT
	add esp, 8
	
	push dword _CoProSeg
	push dword 9
	call _SetIDT
	add esp, 8
	
	push dword _IVTSS
	push dword 10
	call _SetIDT
	add esp, 8
	
	push dword _SegNP
	push dword 11
	call _SetIDT
	add esp, 8
	
	push dword _StackFault
	push dword 12
	call _SetIDT
	add esp, 8
	
	push dword _GPFault
	push dword 13
	call _SetIDT
	add esp, 8
	
	push dword _PageFault
	push dword 14
	call _SetIDT
	add esp, 8
	
	push dword _ExceptionHandler
	push dword 15
	call _SetIDT
	add esp, 8
	
	push dword _Floating
	push dword 16
	call _SetIDT
	add esp, 8
	
	push dword _AlignCheck
	push dword 17
	call _SetIDT
	add esp, 8
	
	push dword _MachineCheck
	push dword 18
	call _SetIDT
	add esp, 8
	
	push dword _SIMD
	push dword 19
	call _SetIDT
	add esp, 8

	push dword _HDD
	push dword 32+14
	call _SetIDT
	add esp, 8

	push dword _HDD
	push dword 32+15
	call _SetIDT
	add esp, 8

	push dword _HDD
	push dword 32+16
	call _SetIDT
	add esp, 8

	ret

;this routine handles the processor exception 0-31
_HDD:
	iret
_ExceptionHandler:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExCommon	
	
	hlt
	iret

_DivideByZero:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExDivideByZero	
	hlt
	iret

_Debug:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExDebug	
	hlt
	iret

_NMI:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExNMI	
	hlt
	iret

_BreakPoint:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExBreakPoint
	hlt
	iret

_Overflow:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExOverflow	
	hlt
	iret

_Bound:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExBound	
	hlt
	iret

_IVOpcode:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExIVOpcode	
	hlt
	iret

_DevNA:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExDevNA
	hlt
	iret

_DoubleFault:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExDoubleFault
	hlt
	iret

_CoProSeg:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExCoProSeg
	hlt
	iret

_IVTSS:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExIVTSS
	hlt
	iret

_SegNP:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExSegNP
	hlt
	iret

_StackFault:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExStackFault
	hlt
	iret

_GPFault:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExGPFault
	hlt
	iret

_PageFault:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExPageFault
	mov eax,CR2
	ULongToString 16,tmpMsg+3,eax
	Print tmpMsg
	
	hlt
	iret

_Floating:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExFloating
	hlt
	iret

_AlignCheck:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExAlignCheck
	hlt
	iret

_MachineCheck:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExMachineCheck
	hlt
	iret

_SIMD:
	cli
	mov ax, DATA_SELECTOR
	mov ds, ax	

	Print ExSIMD
	hlt
	iret



;this routine detects the processor using the CPUID instruction and
;fills the local variables
;********this code is copied from Intel CPUID document AP485 and some of 
;advance processor values are got from www.sandpile.org
_DetectProcessor:
	push ebx ; save registers
	push esi
	push edi
	
	pushfd
	pop eax			; get original EFLAGS
	xor eax, 200000h 	; flip ID bit in EFLAGS
	push eax 		; save new EFLAGS value on stack
	popfd 			; replace current EFLAGS value
	pushfd 			; get new EFLAGS
	pop eax 		; store new EFLAGS in EAX
	xor eax, ecx 		; can’t toggle ID bit,
	je .Done	 	; processor=80486/80386

	; Execute CPUID instruction to determine vendor, family,
	; model, stepping and features. For the purpose of this
	; code, only the initial set of CPUID information is saved.
	mov eax, 0 		; set up for CPUID instruction
	cpuid			; get and save vendor ID
	mov [__CPU_VendorID], ebx
	mov [__CPU_VendorID+4], edx
	mov [__CPU_VendorID+8], ecx

	cmp eax, 1 		; make sure 1 is valid input for CPUID
	jl .Done 		; if not, jump to end
	mov eax, 1
	cpuid 			; get family/model/stepping/features
	mov [__CPU_Signature], eax
	mov [__CPU_Features_EBX], ebx
	mov [__CPU_Features_EDX], edx
	mov [__CPU_Features_ECX], ecx
.Done:

	pop edi ; restore registers
	pop esi
	pop ebx
	ret
;*********************************************************************
tmpMsg			db 10,13, '               ',0
ExCommon		db 10,13, 'EXCEPTION:: Unhandled Exception Occured',0
ExDivideByZero	db 10,13, 'EXCEPTION:: Divide by zero',0
ExDebug			db 10,13, 'EXCEPTION:: Debug',0
ExNMI			db 10,13, 'EXCEPTION:: NMI',0
ExBreakPoint	db 10,13, 'EXCEPTION:: Breakpoint',0
ExOverflow		db 10,13, 'EXCEPTION:: Overflow',0
ExBound			db 10,13, 'EXCEPTION:: Bound range exceeded',0
ExIVOpcode		db 10,13, 'EXCEPTION:: Invalide opcode',0
ExDevNA			db 10,13, 'EXCEPTION:: Device not available',0
ExDoubleFault	db 10,13, 'EXCEPTION:: Double fault',0
ExCoProSeg		db 10,13, 'EXCEPTION:: Coprocessor segment overrun',0
ExIVTSS			db 10,13, 'EXCEPTION:: Invalid TSS',0
ExSegNP			db 10,13, 'EXCEPTION:: Segment not present',0
ExStackFault	db 10,13, 'EXCEPTION:: Stack fault',0
ExGPFault		db 10,13, 'EXCEPTION:: General protection fault',0
ExPageFault		db 10,13, 'EXCEPTION:: Page fault',0
ExFloating		db 10,13, 'EXCEPTION:: Floating point error',0
ExAlignCheck	db 10,13, 'EXCEPTION:: Alignment check',0
ExMachineCheck	db 10,13, 'EXCEPTION:: Machine check',0
ExSIMD			db 10,13, 'EXCEPTION:: SIMD Floating point',0
