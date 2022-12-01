;                            Ace OS Source Code - System Call Interrupt Handler
;                                       Created On : 29-May-2004 7:31PM   
;                                       Developer  : Samuel (samueldotj@gmail.com)

%include "Common.inc"

SYSCALL_END EQU 100

EXTERN _ExecuteSysCall
EXTERN __SysCallDispatchTable
GLOBAL _SysIntHandler


[SECTION .text]
[BITS 32]
;This function should responsible for the followings
;       1) It should switch/allocate Stack for ring 0 calls
;       2) It should copy all the required parameters from user stack to kernel stack
;       3) It must clean up the used stack space when the call is finished running
;Arguments:
;       EAX - Function No

;_sys_Old_ESP dw 0               ; this will create problems----todo
extern _DEBUG
_SysIntHandler:

    ;Intel Documentation :: When an interrupt is handled through an interrupt gate, the IF flag is automatically cleared, which disables maskable hardware interrupts.
    ;to avoid this I have globally enabled interrupts for all system calls - or enable it indiviually for routines which requires interrupt eg - KB_Read()
    sti 
    
    push ebp
    push ebx
    push ecx
    push esi
    push edi
    
    mov ebp, esp
    ;call _DEBUG
    
    ;check the valid address range
    cmp eax, SYSCALL_END
    jg .ERROR
    
    ;find index for the __SysCallDispatchTable
    mov cx, 8
    Mul16EAX cx
    mov ebx, [__SysCallDispatchTable+eax+4]     ; number of parameters
    mov eax, [__SysCallDispatchTable+eax]       ; system function call
    
    mov ecx, ebx                                ; set loop count
    
    ;if within kernel to kernel
    ;mov esi, ebp                                ; set source address
    ;add esi, 20+24                               ; kernel code selector
    
    ;call from other rings
    mov esi, [ebp+32];                          ; set source address
    add esi, 8                                  ; skip the pushed return address ---todo - this will change if ring x to ring 0 happens where x!=0
    ;add esi, 12                             
    
    mov edi, esp                                ; set destination address
    shl ebx, 2                                  ; multiply by 4 to get byte offset
    sub edi, ebx
    
    rep movsd
    
    sub esp, ebx                                ; set the new stack adddress
    
    call eax
    jmp .DONE
    
    .ERROR:
        mov eax, 1                              ;set error code - INVALID FUNCTION
    
    .DONE:
    mov esp, ebp
    
    pop edi
    pop esi
    pop ecx
    pop ebx
    pop ebp
    
    iret
    
