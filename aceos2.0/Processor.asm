;------------------------------------------------------
;               Processor Manager for Ace Os
;                      Created by Sam 10-Aug-2002
; Todo : This file can be complete replaced by a c file
;------------------------------------------------------
%include "Common.inc"

[BITS 32]
[SECTION .text]
GLOBAL _DetectProcessor

GLOBAL _SetExceptionHandlers
GLOBAL _ExceptionHandler

GLOBAL __CPU_ProcessorID
GLOBAL __CPU_VendorID
GLOBAL __CPU_Signature
GLOBAL __CPU_Features_EBX
GLOBAL __CPU_Features_EDX
GLOBAL __CPU_Features_ECX

EXTERN _V86GPHandler
EXTERN _DumpCPU
EXTERN _SysIntHandler

struc CPUID_DATA
    .EAX resd 1
    .EBX resd 1
    .EDX resd 1
    .ECX resd 1
endstruc

EXTERN _sysCPUBasicInformation
EXTERN _sysCPUExtendedInformation


%macro DumpCPU 1
    cli
    push eax
    mov eax, ds
    push eax
    
    mov eax, DATA_SELECTOR
    mov ds, eax

    mov eax, cr4
    push eax
    mov eax, cr3
    push eax
    mov eax, cr2
    push eax
    mov eax, 0 ;cr1
    push eax
    mov eax, cr0
    push eax
    pushf
    push edi
    push esi
    push esp
    push ebp
    mov eax, fs
    push eax
    mov eax, gs
    push eax
    mov eax, ss
    push eax
    mov eax, es
    push eax
    push edx
    push ecx
    push ebx
    push dword %1
    call _DumpCPU
    add esp, 21*4
%endmacro
	
%macro SetIDTEntry 2
    push dword %1
    push dword %2
    call _SetIDTEntry
    add esp, 8
%endmacro

;this routine handles the processor exception 0-31	
_SetExceptionHandlers:
    SetIDTEntry _DivideByZero, 0
    SetIDTEntry _Debug , 1
    SetIDTEntry _NMI , 2
    SetIDTEntry _BreakPoint , 3
    SetIDTEntry _Overflow , 4
    SetIDTEntry _Bound, 5
    SetIDTEntry _IVOpcode, 6
    SetIDTEntry _DevNA, 7
    SetIDTEntry _DoubleFault, 8
    SetIDTEntry _CoProSeg, 9
    SetIDTEntry _IVTSS, 10
    SetIDTEntry _SegNP, 11
    SetIDTEntry _StackFault, 12
    SetIDTEntry _GPFault, 13
    SetIDTEntry _PageFault, 14
    SetIDTEntry _ExceptionHandler, 15
    SetIDTEntry _Floating, 16
    SetIDTEntry _AlignCheck, 17
    SetIDTEntry _MachineCheck, 18
    SetIDTEntry _SIMD, 19
    
    SetIDTEntry _HDD, 32+14
    SetIDTEntry _HDD, 32+15
    SetIDTEntry _HDD , 32+16

    SetIDTEntry _SysIntHandler, 50
    ret

_HDD:
    iret

_ExceptionHandler:
    DumpCPU 29
    hlt
    iret

_DivideByZero:
    DumpCPU 0
    hlt
    iret

extern _DEBUG
_Debug:
    ;DumpCPU 1
    ;hlt
    call _DEBUG
    iret

_NMI:
    DumpCPU 2
    hlt
    iret

_BreakPoint:
    DumpCPU 3
    hlt
    iret

_Overflow:
    DumpCPU 4
    hlt
    iret

_Bound:
    DumpCPU 5
    hlt
    iret

_IVOpcode:
    DumpCPU 6
    hlt
    iret

_DevNA:
    DumpCPU 7
    hlt
    iret

_DoubleFault:
    DumpCPU 8
    hlt
    iret

_CoProSeg:
    DumpCPU 9
    hlt
    iret

_IVTSS:
    DumpCPU 10
    hlt
    iret

_SegNP:
    DumpCPU 11
    hlt
    iret

_StackFault:
    DumpCPU 12
    hlt
    iret

_GPFault:
    push eax
    mov eax, [esp+16]
    and eax, 0x20000            ;check the EFLAG value
    cmp eax, 0
    pop eax
    jne .V86Mode
    DumpCPU 13
    hlt
    iret
    
.V86Mode:
    
    push eax            ; save eax
    mov eax, DATA_SELECTOR
    mov ds, eax         ; change the DS
    push esp            ; pointer to V86 structure
    push edx            ; why this edx, ecx changed by this call?
    push ecx
    call _V86GPHandler
    cmp eax, 0
    je .HLT
    pop ecx
    pop edx
    add esp, 4          ; pointer structure
    pop eax             ; restore eax
    add esp, 4          ; skip error code
    iret
.HLT:
    add esp, 12
    cli
    hlt
    iret

_PageFault:
    DumpCPU 14
    hlt
    iret

_Floating:
    DumpCPU 16
    hlt
    iret

_AlignCheck:
    DumpCPU 17
    hlt
    iret

_MachineCheck:
    DumpCPU 18
    hlt
    iret

_SIMD:
    DumpCPU 19
    hlt
    iret

;this routine detects the processor using the CPUID instruction and
;fills the local variables
;********this code is copied from Intel CPUID document AP485 and some of 
;advance processor values are got from www.sandpile.org
_DetectProcessor:
    pusha
    push ebx ; save registers
    push esi
    push edi
    
    pushfd
    pop eax		; get original EFLAGS
    xor eax, 200000h 	; flip ID bit in EFLAGS
    push eax 		; save new EFLAGS value on stack
    popfd 		; replace current EFLAGS value
    pushfd 		; get new EFLAGS
    pop eax 		; store new EFLAGS in EAX
    xor eax, ecx 	; can’t toggle ID bit,
    je .Done	 	; processor=80486/80386

    
    mov edi, _sysCPUBasicInformation      ;store the datastructure address
    mov esi, edi                            ;it will be used for EAX MAX_LEVEL
    xor eax, eax                            ;store 0 in eax
.CPUIDLoop:
    push eax                                ;save the current value of eax in the stack (for later use -> Current Level )
    
    ;cmp esi, _sysCPUExtendedInformation     ;check whether the current loop is for extended information or basic information
    ;jne .Basic
    
.Basic:
    cpuid                                   ;excute the cpuid instruction
    mov [edi+CPUID_DATA.EAX], eax           ;store the result
    mov [edi+CPUID_DATA.EBX], ebx
    mov [edi+CPUID_DATA.ECX], ecx
    mov [edi+CPUID_DATA.EDX], edx
    add edi, 4 * 4                          ;move to next array element
    
    pop eax                                 ;get the last executed level
    inc eax                                 ;next level
    
    cmp eax, [esi]                          ;compare tthe level with max level
    
    jle .CPUIDLoop                          ;if not equal loop
    
    cmp esi, _sysCPUExtendedInformation
    jne .Done
    mov edi, _sysCPUExtendedInformation     ;another loop for Extened CPUID information.
    mov esi, edi                            
    mov eax, 0x80000000                     ;store 0x80000000 in eax    
    
.Done:

    pop edi ; restore registers
    pop esi
    pop ebx
    popa
    ret
	
