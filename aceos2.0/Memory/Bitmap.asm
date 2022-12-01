;------------------------------------------------------------
;               Memory Usage Bitmap functions  for Ace Os
;                      Created by Sam 15-June-2003
;-----------------------------------------------------------
%include "Common.inc"
[SECTION .text]
[BITS 32]

[GLOBAL _mm_FindContinousInBitmap]
[GLOBAL _mm_C_S_BitsInBitmap]

;This function scans for a number of set bits with in limit of a bitmap
;Input : 
;        8  Bitmap Address
;        12 ScanFor         - 0 Scan for Cleared, 1 scan for Set bits
;        16 StartIndex      - Starting Index
;        20 Size            - Number of bits to scan
;        24 Limit           - Scan length
;Output :
;        Address where the first bit found in the Bitmap
_mm_FindContinousInBitmap:
    prologue 20

    push ds
    push edi
    push ebx
    push ecx
    push edx
    
    xor ebx, ebx
    
    ;mov ax, DATA_SELECTOR
    ;mov ds, ax
    
    mov edi, [ebp+8]                    ; Bitmap Start Address
    mov eax, [ebp+16]	                ; starting index
    mov ecx, [ebp+24]                   ; limit
    sub ecx, [ebp+20]                   ; ecx = limit-size
    ;Note:------------*** Only works upto 512 MB due to bt instruction*******************
    .Loop:
        bt [ds:edi], eax		; check the bit
        setc bl                         ; store the result in ebx
        cmp ebx, [ebp+12]               ; compare it with set/clear
        je .Found                       ; found set or clear bit
        inc eax				; not found goto next bit
        cmp eax,ecx        		; compare eax with limit-size
        jl .Loop
    jmp .NotFound
    
    .Found:   ;---eax contains the required page bit index 
    mov edx, [ebp+20]                  ; size
    .FoundLoop:
        bt [ds:edi], eax               ; check the bit
        setc bl                        ; store the result in ebx
        cmp ebx, [ebp+12]              ; compare it with set/clear
        jne .GotoMain
        
        inc eax                        ; next bit
        dec edx                        ; decrement the size
        cmp edx,0                      
        jg .FoundLoop
        
    sub eax, [ebp+20]                  ; eax = first bit found
    jmp .Done

    .GotoMain:
        jmp .Loop
    
    .NotFound:
        xor eax, eax		       ; not found return 0
    .Done:
    
    pop edx
    pop ecx
    pop ebx
    pop edi
    pop ds
    
    epilogue
    ret
;This function sets or clears a no of bits specified 
;Input : Bitmap Address
;        ClearOrSet - 0 Clear, 1 Set 
;        StartIndex - Starting Index bit
;        Size       - Number of bits to be set or cleared
_mm_C_S_BitsInBitmap:
    prologue 16
    
    push ds
    push ecx
    push edi
    
    mov edi, [ebp+8]            ; Bitmap 
    mov eax, [ebp+16]           ; starting index
    mov ecx, [ebp+20]           ; size 
    .Loop:
        cmp dword[ebp+12], 1    ; set / clear
        je .SetBit
        btr [ds:edi], eax       ; clear the bit
        jmp .AfterSet
        .SetBit:
        bts [ds:edi], eax       ; set the bit
        
        .AfterSet:
        inc eax
        
        dec ecx
        cmp ecx, 0
        jg .Loop
    
    pop edi
    pop ecx
    pop ds
    
    epilogue
    
    ret

