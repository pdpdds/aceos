;VESA Initialization code for Ace OS
; Stores the VESA Info at 200KB and Mode Info array at 210KB
;       Created by Samuel on 24-Jul-2004 3:03 PM
[SECTION .text]
[BITS 16]
;Set Segement Registers
mov ax, 0x4B00   ;300KB
mov ss, ax
mov ax, 0x1000
mov ds, ax
mov es, ax

;Get VESA Info(512 bytes) - Store it in 200 KB.
;segment = 200*1024/16
mov ax, 0x3200
mov es, ax
mov di, 0
push es
mov ax, 0x4f00
int 0x10
pop es
cmp ah, 0       ;if error goto last
jne DONE

mov di, 14      ; video mode ptr location
push dword [es:di]
pop si
pop ds


mov ax, 0x3480  ; 210 KB
mov es, ax
mov di, 0
NextMode:
    mov ax, 0x4f01  ;get video mode info
    mov cx, [ds:si]
    push es
    int 0x10
    pop es
    add di, 256     ; next mode info location
    add si, 2       ; next mode
    cmp word [ds:si], -1
jne NextMode


mov ax, 0x4f02
mov bx, 0x4101
int 0x10


DONE:
hlt