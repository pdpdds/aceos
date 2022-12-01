;                         Ace OS OSLoader
; This module is used to load the Kernel from FAT file system. It also setups the required
;protected mode data structre. However the data structure created by OSLoader is static. ie it is hardcoded.

;Note:--------when you modify the code / data the IDT starting offset will be moved to new valur
; take care of it. It is refered in the Processor.asm->SetIDT(), GetIDT()

KernelAddress 	EQU 0x19000
KernelSegment 	EQU 0x1900
KernelLow	EQU 0x9000
KernelMedium	EQU 0x1

OSLoaderAddress EQU 0x800
OSLoaderSegment EQU 0x80

RootDirOffset	EQU 0xC800
FATOffset		EQU 0x7800

ACS_CODE		EQU 0X9A  ; (PRESENT | CSEG | READ)
ACS_DATA		EQU 0X92  ; (PRESENT | DSEG | WRITE)
ACS_STACK		EQU 0X96  ; (PRESENT | DSEG | WRITE | EXPAND DOWN )

struc GDTEnt
	.limit: resw 1
	.base_l: resw 1
	.base_m: resb 1
	.access: resb 1
	.attribs: resb 1
	.base_h: resb 1
endstruc

;--------------------------Starting Code Segment --------------------------
[BITS 16]		
[ORG 0x0000]
	jmp START
;--------------------------Skipping the Data Structure --------------------------------
;This processor data structure is put here so that the OSLoader code can be overwritten
;The data strure is aligned in 32 bits
[BITS 32]
GDTR:
	dw 39 				; Size of GDT - 1 ( 5*8 -1 )
	dd OSLoaderAddress+GDTNull	; Physical address of GDT address
IDTR:
	dw IDT_END - IDT -1 		; Size of IDT - 1
	dd OSLoaderAddress+IDT		; Base address
ALIGN 32
IDT:	
	resb 256*8 					;256 idt entries and each is 8 bytes long
IDT_END:	
ALIGN 32
GDTNull istruc GDTEnt
	at GDTEnt.limit,  dw 0
	at GDTEnt.base_l, dw 0
	at GDTEnt.base_m, db 0
	at GDTEnt.access, db 0
	at GDTEnt.attribs,db 0
	at GDTEnt.base_h, db 0
iend
GDTCode istruc GDTEnt
	at GDTEnt.limit,  dw 0xFFFF
	at GDTEnt.base_l, dw KernelLow
	at GDTEnt.base_m, db KernelMedium
	at GDTEnt.access, db ACS_CODE			
	at GDTEnt.attribs,db 0xDF
	at GDTEnt.base_h, db 0x0
iend
GDTData istruc GDTEnt
	at GDTEnt.limit,  dw 0xFFFF
	at GDTEnt.base_l, dw 0
	at GDTEnt.base_m, db 0
	at GDTEnt.access, db ACS_DATA
	at GDTEnt.attribs,db 0xDF
	at GDTEnt.base_h, db 0x0
iend
GDTStack istruc GDTEnt
	at GDTEnt.limit,  dw 25		; FFFF->65535=16 Pages(0-15)
	at GDTEnt.base_l, dw 0x0
	at GDTEnt.base_m, db 0x0
	at GDTEnt.access, db ACS_DATA
	at GDTEnt.attribs,db 0xD0
	at GDTEnt.base_h, db 0x0
iend
GDTVideo istruc GDTEnt
	at GDTEnt.limit,  dw 0x2	;1 page 80*25*2=4000->4096K
	at GDTEnt.base_l, dw 0xE000
	at GDTEnt.base_m, db 0xF
	at GDTEnt.access, db ACS_DATA
	at GDTEnt.attribs,db 0xD0
	at GDTEnt.base_h, db 0x0
iend
;---------------------------------------------Actual Code ---------------------------------
[BITS 16]
START:
	cli
	mov ax, RELOCATE
	mov ax, RELOCATE.Debug
	mov ax, DONE
	mov ax, OSLoaderSegment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; create stack
	mov ax, 0x0000
	mov ss, ax
	mov sp, 0xFFFF

	;loading orig boot parameters from 0x7C00
	push ds
	mov cx,62
	mov ax,7c0h
	mov ds,ax
	mov di,OEM_ID
	mov si,3
	cld
	rep movsb
	pop ds

	sti
	mov si, msgLoading
	call DisplayMessage

	call EnableA20

LOAD_ROOT:
	;compute size of root directory and store in ‘cx’
	xor cx, cx
	xor dx, dx
	mov ax, 0x0020                          ; 32 byte directory entry
	mul WORD [MaxRootEntries]               ; total size of directory
	div WORD [BytesPerSector]               ; sectors used by directory
	xchg ax, cx
	;compute location of root directory and store in ‘ax’
	mov al, BYTE [TotalFATs]                ; number of FATs
	mul WORD [SectorsPerFAT]                ; sectors used by FATs
	add ax, WORD [ReservedSectors]          ; adjust for bootsector
	mov WORD [datasector], ax               ; base of root directory
	add WORD [datasector], cx
	
	;read root directory into memory 
	mov bx, RootDirOffset			; copy root dir above code
	call ReadSectors

	;browse root directory for binary image
	mov cx, WORD [MaxRootEntries]           ; load loop counter
	mov di, RootDirOffset			; locate first root entry
.LOOP:
	push cx
	mov cx, 0x000B                          ; eleven character name
	mov si, ImageName                       ; image name to find
	push di
	rep cmpsb                                       ; test for entry match
	pop di

	je LOAD_FAT
	pop cx
	add di, 0x0020                          ; queue next directory entry
	loop .LOOP
	jmp     FAILURE

LOAD_FAT:
	mov dx, WORD [di + 0x001A]
	mov WORD [cluster], dx                  ; file’s first cluster
	;compute size of FAT and store in ‘cx’
	xor ax, ax
	mov al, BYTE [TotalFATs]                ; number of FATs
	mul WORD [SectorsPerFAT]                ; sectors used by FATs
	mov cx, ax
	;compute location of FAT and store in ‘ax’
	mov ax, WORD [ReservedSectors]          ; adjust for bootsector
	;read FAT into memory 
	mov bx, FATOffset			     ; copy FAT above bootcode
	call ReadSectors

	
	;------------------Reading Kernel DLL into memory------------------
	mov ax, KernelSegment			; destination of image CS
	mov es, ax
	mov bx, 0x0000                          ; destination for image IP
	push bx
	mov byte [ShowProgress],1
LOAD_IMAGE:
	mov ax, WORD [cluster]                  ; cluster to read
	pop bx                                  ; buffer to read into
	call ClusterLBA                         ; convert cluster to LBA
	xor cx, cx
	mov cl, BYTE [SectorsPerCluster]        ; sectors to read
	call ReadSectors

	cmp bx, 1024				; Segment override (64kb)
	jne .SegmentOk
	mov ax, es				; next segment ( add 64)
	add ax, 64
	mov es, ax
	mov bx, 0
.SegmentOk:

	push    bx						    	;compute next cluster
	mov ax, WORD [cluster]                  ; identify current cluster
	mov cx, ax                              ; copy current cluster
	mov dx, ax                              ; copy current cluster
	shr dx, 0x0001                          ;
	;divide by two
	add cx, dx                              ; sum for (3/2)
	mov bx, FATOffset			     		; location of FAT in memory
	add bx, cx                              ; index into FAT
	mov dx, WORD [bx]                       ; read two bytes from FAT
	test ax, 0x0001
	jnz .ODD_CLUSTER
.EVEN_CLUSTER:
	and dx, 0000111111111111b               ; take low twelve bits
	jmp .DONE
.ODD_CLUSTER:
	shr dx, 0x0004                          ; take high twelve bits
.DONE:
	mov WORD [cluster], dx                  ; store new cluster
	cmp dx, 0x0FF0                          ; test for end of file
	jb LOAD_IMAGE

	mov si, msgRelocate
	call DisplayMessage
RELOCATE:	
	cld
	cli
	mov ax, KernelSegment
	mov bx, [KernelSize]
	.Loop:
		mov es, ax			;Destination
		mov edi,	0
		mov ds, ax			;Source
		mov esi, 0x2c0
		mov ecx, 1024			; one segment
		rep movsb
		.Debug
		add ax, 64
		cmp bx, 0		; Check for next segment 
		dec bx
	jne .Loop
	
DONE:
	mov ax,cs
	mov ds,ax
	mov es,ax

	mov si, msgSwitch
	call DisplayMessage

	cli	
	;hlt
	
	lgdt [ds:GDTR]
	lidt [ds:IDTR]			
	
	mov eax,cr0				;switching to PMode
	or eax,1b		
	mov cr0,eax
	jmp 0x8:0				;jmping to 32 bit code
	
	
FAILURE:
	mov si, msgFailure
	call DisplayMessage
	mov ah, 0x00
	int     0x16                                ; await keypress
	int     0x19                                ; warm boot computer

;*************************************************************************
; PROCEDURE DisplayMessage
; display ASCIIZ string at ds:si via BIOS
;*************************************************************************
DisplayMessage:
	lodsb                                       ; load next character
	or al, al                              ; test for NUL character
	jz .DONE
	mov ah, 0x0E                            ; BIOS teletype
	mov bh, 0x00                            ; display page 0
	mov bl, 0x07                            ; text attribute
	int 0x10                                ; invoke BIOS
	jmp DisplayMessage
.DONE:
	ret
;*************************************************************************
; PROCEDURE ReadSectors
; reads ‘cx’ sectors from disk starting at ‘ax’ into
;memory location ‘es:bx’
;*************************************************************************
ReadSectors:
.MAIN:
	mov     di, 0x0005                          ; five retries for error
.SECTORLOOP:
	push    ax
	push    bx
	push    cx
	call    LBACHS
	mov     ah, 0x02                            ; BIOS read sector
	mov     al, 0x01                            ; read one sector
	mov     ch, BYTE [absoluteTrack]            ; track
	mov     cl, BYTE [absoluteSector]           ; sector
	mov     dh, BYTE [absoluteHead]             ; head
	mov     dl, BYTE [DriveNumber]              ; drive
	int     0x13                                ; invoke BIOS
	jnc     .SUCCESS                            ; test for read error
	xor     ax, ax                              ; BIOS reset disk
	int     0x13                                ; invoke BIOS
	dec     di                                  ; decrement error counter
	pop     cx
	pop     bx
	pop     ax
	jnz     .SECTORLOOP                         ; attempt to read again
	int     0x18
.SUCCESS:
	cmp byte [ShowProgress],0
	je .PROGRESSDONE
	mov     si, msgProgress
	call    DisplayMessage
	.PROGRESSDONE:
	pop     cx
	pop     bx
	pop     ax
	add bx, WORD [BytesPerSector]           ; queue next buffer
	inc     ax                                  ; queue next sector
loop    .MAIN                               ; read next sector
	ret

;*************************************************************************
; PROCEDURE ClusterLBA
; convert FAT cluster into LBA addressing scheme
; LBA = (cluster - 2) * sectors per cluster
;*************************************************************************
ClusterLBA:
	sub     ax, 0x0002                          ; zero base cluster number
	xor     cx, cx
	mov     cl, BYTE [SectorsPerCluster]        ; convert byte to word
	mul     cx
	add     ax, WORD [datasector]               ; base data sector
	ret

;*************************************************************************
; PROCEDURE LBACHS
; convert ‘ax’ LBA addressing scheme to CHS addressing scheme
; absolute sector = (logical sector / sectors per track) + 1
; absolute head   = (logical sector / sectors per track) MOD number of heads
; absolute track  = logical sector / (sectors per track * number of heads)
;*************************************************************************
LBACHS:
	xor     dx, dx                              ; prepare dx:ax for operation
	div     WORD [SectorsPerTrack]              ; calculate
	inc     dl                                  ; adjust for sector 0
	mov     BYTE [absoluteSector], dl
	xor     dx, dx                              ; prepare dx:ax for operation
	div     WORD [NumHeads]                     ; calculate
	mov     BYTE [absoluteHead], dl
	mov     BYTE [absoluteTrack], al
	ret
;*************************************************************************
; This subroutine will enable the A20 address line in the keyboard
; controller.  Takes no arguments.  Returns 0 in EAX on success,
; -1 on failure.
;*************************************************************************
EnableA20:

	pusha

	;; Make sure interrupts are disabled
	cli

	;; Keep a counter so that we can make up to 5 attempts to turn
	;; on A20 if necessary
	mov CX, 5

	.startAttempt1:		
	;; Wait for the controller to be ready for a command
	.commandWait1:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait1

	;; Tell the controller we want to read the current status.
	;; Send the command D0h: read output port.
	mov AL, 0D0h
	out 64h, AL

	;; Wait for the controller to be ready with a byte of data
	.dataWait1:
	xor AX, AX
	in AL, 64h
	bt AX, 0
	jnc .dataWait1

	;; Read the current port status from port 60h
	xor AX, AX
	in AL, 60h

	;; Save the current value of (E)AX
	push AX			; 16-BIT
	;; push EAX		; 32-BIT

	;; Wait for the controller to be ready for a command
	.commandWait2:
	in AL, 64h
	bt AX, 1
	jc .commandWait2

	;; Tell the controller we want to write the status byte again
	mov AL, 0D1h
	out 64h, AL	

	;; Wait for the controller to be ready for the data
	.commandWait3:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait3

	;; Write the new value to port 60h.  Remember we saved the old
	;; value on the stack
	pop AX			; 16-BIT
	;; pop EAX		; 32-BIT

	;; Turn on the A20 enable bit
	or AL, 00000010b
	out 60h, AL

	;; Finally, we will attempt to read back the A20 status
	;; to ensure it was enabled.

	;; Wait for the controller to be ready for a command
	.commandWait4:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait4

	;; Send the command D0h: read output port.
	mov AL, 0D0h
	out 64h, AL	

	;; Wait for the controller to be ready with a byte of data
	.dataWait2:
	xor AX, AX
	in AL, 64h
	bt AX, 0
	jnc .dataWait2

	;; Read the current port status from port 60h
	xor AX, AX
	in AL, 60h

	;; Is A20 enabled?
	bt AX, 1

	;; Check the result.  If carry is on, A20 is on.
	jc .success

	;; Should we retry the operation?  If the counter value in ECX
	;; has not reached zero, we will retry
	loop .startAttempt1


	;; Well, our initial attempt to set A20 has failed.  Now we will
	;; try a backup method (which is supposedly not supported on many
	;; chipsets, but which seems to be the only method that works on
	;; other chipsets).


	;; Keep a counter so that we can make up to 5 attempts to turn
	;; on A20 if necessary
	mov CX, 5

	.startAttempt2:
	;; Wait for the keyboard to be ready for another command
	.commandWait6:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait6

	;; Tell the controller we want to turn on A20
	mov AL, 0DFh
	out 64h, AL

	;; Again, we will attempt to read back the A20 status
	;; to ensure it was enabled.

	;; Wait for the controller to be ready for a command
	.commandWait7:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait7

	;; Send the command D0h: read output port.
	mov AL, 0D0h
	out 64h, AL	

	;; Wait for the controller to be ready with a byte of data
	.dataWait3:
	xor AX, AX
	in AL, 64h
	bt AX, 0
	jnc .dataWait3

	;; Read the current port status from port 60h
	xor AX, AX
	in AL, 60h

	;; Is A20 enabled?
	bt AX, 1

	;; Check the result.  If carry is on, A20 is on, but we might warn
	;; that we had to use this alternate method
	jc .warn

	;; Should we retry the operation?  If the counter value in ECX
	;; has not reached zero, we will retry
	loop .startAttempt2


	;; OK, we weren't able to set the A20 address line.  Do you want
	;; to put an error message here?
	jmp .fail


	.warn:
	;; Here you may or may not want to print a warning message about
	;; the fact that we had to use the nonstandard alternate enabling
	;; method

	.success:
	sti
	popa
	xor EAX, EAX
	ret

	.fail:
	sti
	popa
	mov EAX, -1
	ret
;*************************************************************************
;Data starts here
	KernelSize      dw 95
	absoluteSector db 0x00
	absoluteHead   db 0x00
	absoluteTrack  db 0x00

	datasector  	dw 0x0000
	cluster     		dw 0x0000
	ImageName   	db "KERNEL  EXE"
	msgLoading  	db 0x0D, 0x0A, "Loading Kernel ",  0x00
	msgCRLF     	db 0x0D, 0x0A, 0x00
	msgProgress 	db ".", 0x00
	msgFailure  	db 0x0D, 0x0A, "ERROR : Press Any Key to Reboot", 0x00
	msgRelocate	db 0x0D, 0x0A, "Relocating kernel code...",0x00
	msgSwitch   	db 0x0D, 0x0A, "Switching to kernel...",0x00
	ShowProgress	db 0

	OEM_ID		      db "ACE   OS"
	BytesPerSector        dw 0x0200
	SectorsPerCluster     db 0x01
	ReservedSectors       dw 0x0001
	TotalFATs             db 0x02
	MaxRootEntries        dw 0x00E0
	TotalSectorsSmall     dw 0x0B40
	MediaDescriptor       db 0xF0
	SectorsPerFAT         dw 0x0009
	SectorsPerTrack       dw 0x0012
	NumHeads              dw 0x0002
	HiddenSectors         dd 0x00000000
	TotalSectorsLarge     dd 0x00000000
	DriveNumber           db 0x00
	Flags                 db 0x00
	Signature             db 0x29
	VolumeID              dd 0xFFFFFFFF
	VolumeLabel           db "ACE    BOOT"
	SystemID              db "FAT12   "