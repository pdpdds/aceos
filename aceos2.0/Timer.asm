;                            Ace OS Source Code - Timer Interrupt Handler
;                                       Created On : 20-Jun-2002 
;                                       Developer  : Samuel (samueldotj@gmail.com)

%include "Common.inc"

;this structure's equivalent (struct ThreadReg) is declared in the include/Thread.h 
struc ThreadStackInfo
    .SS0 resd 1
    .SS1 resd 1
    .SS2 resd 1
    .SS3 resd 1
    
    .ESP0 resd 1
    .ESP1 resd 1
    .ESP2 resd 1
    .ESP3 resd 1
endstruc

[SECTION .text]
[BITS 32]

TMR_INTERVAL_VALUE EQU 100

GLOBAL _TimerInterruptHandler

EXTERN _dwTimerValue
EXTERN _dwTimerInterval
EXTERN _dwTaskSwitchRequest

EXTERN _Scheduler
EXTERN _sysCurThreadStackInfo
EXTERN _sys_TSS
EXTERN _DEBUG


align 32
;This is the default kernel handler for the Timer interrupt it also used as task switching routine
;This function should be very minimal so that it can handl all the timer interrupts
_TimerInterruptHandler:
    cli                                 ;Clear Interrupt Flag
    
    pusha                               ;Push EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI
    push ds                             ;Push Segment registers
    push es 
    push fs 
    push gs
    
    mov eax, DATA_SELECTOR              ;select kernel data selector into datasegment and extrasegment registers
    mov ds, eax
    mov es, eax
    
    inc dword [_dwTimerValue]             
    inc dword [_dwTimerInterval]
    
    cmp dword [_dwTaskSwitchRequest], 1
    je .TaskSwitch
    
    cmp dword [_dwTimerInterval], TMR_INTERVAL_VALUE
    jne .Done
    
    .TaskSwitch:
        mov dword [_dwTimerInterval], 0           ;reset the TimerInterval
        mov dword [_dwTaskSwitchRequest], 0           ;reset 
        
        mov eax, [_sysCurThreadStackInfo]       ;update the ESP in sysCurThreadStackInfo->ESP0
        mov [eax+ThreadStackInfo.ESP0], esp
        call _Scheduler                         ;Invoke Scheduler 
        cmp eax,0                               ;check the return value of Scheduler
        je .Done
        
        mov eax, [_sysCurThreadStackInfo]       ;Update ESP with previously stored sysCurThreadStackInfo->ESP0
        mov esp, [eax+ThreadStackInfo.ESP0]
        
        mov eax, [_sys_TSS]                     ;Update sysTSS->ESP0 with expected ESP after process switch
        mov ebx, esp                            ;previous value
        add ebx, 68                              ;after poping the esp will be added with 68, to compensenate automatic increment of systss->esp during task switch
        mov [eax+4], ebx                        ;sysTSS->ESP0
        
        ;call _DEBUG
        
    .Done:
    
    mov al,0x20		; End of Interrupt to PIC
    out 0x20,al
        
    pop gs              ;Restore segment registers
    pop fs
    pop es
    pop ds
        
    popa                ; Restore GP registers
    
    sti                 ; Set IF
    iret                ; task switch

