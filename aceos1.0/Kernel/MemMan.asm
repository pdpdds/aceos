;------------------------------------------------------
;               Memory Manager for Ace Os
;                      Created by Sam 03-Aug-2002
;------------------------------------------------------

;1MB - 3 MB is used by the Memory Manager to control and track virtual memory
%include "Common.inc"
%include "VGAText.inc"
[BITS 32]
[SECTION .text]

GDTStartRAM EQU 1024*1024

GDTStartPage EQU 0
GDTPages EQU 5			
GDTAddress EQU 0x800

StackStartPage EQU 15
StackPages EQU 1
StackAddress EQU 0xFFFF

OSStartPage EQU 25 
OSPages EQU 25
KernelAddress EQU 0x19000

VGAStartPage EQU 184
VGAPages EQU 2
VGAAddress EQU 0xB8000

PDStartPage EQU 256
PDPages EQU 260
PDAddress EQU 0x100000

GLOBAL _CreatePageDir
GLOBAL _AllocateOSPages
GLOBAL _AllocatePages
GLOBAL _AllocateMemory
GLOBAL _InitMemMan
GLOBAL _MemoryAmount

_MemoryAmount DD 100 	; physical memory present
KernelPagesAllocated  DW PDStartPage+PDPages	; no of pages allocated
VritualPagesAvailable DW 256*1024				; total no of pages = total PDE*1024


;this function must be called to initialize the memory manager
_InitMemMan:
	push edi
	mov edi, 1024*1024			; This will detect the amount of RAM present in the system
	
.Detect:
	mov byte [ds:edi], 0x0f		; writing and comparing a bit pattern
	cmp byte [ds:edi], 0x0f
	jne .Done

	mov byte [ds:edi], 0xf0		; writing and comparing a bit pattern
	cmp byte [ds:edi], 0xf0
	jne .Done

	inc word [_MemoryAmount]		
	add edi,1024			; Next Kilobyte

	jmp .Detect
.Done:
	call _CreatePageDir
	call _AllocateOSPages
	mov eax, GDTStartRAM    		;Physical address of OS page directory
	mov cr3, eax				;Store in Control Reg 3
	mov eax, cr0				;Get Control Reg 0
	or eax, 10000000000000000000000000000001b	;Set paging bit ON
	mov cr0, eax				;Store Control Reg 0
	jmp short .PageEnabled			;Clearing prefetch queue
.PageEnabled:

	pop edi
	ret

;This routine will create a PageDirectory at 1MB
;It also creates PageTables at 1MB+4KB
;Since the address of PageTables are predetermined their addresses are filled in the Page Directory when they are created
;It will not map the OS Kernel address in the Page Table entries, AllocateOSPages is meant for that.
_CreatePageDir:
	push edi
	push eax
	push ecx

	mov eax, GDTStartRAM ; 1MB
	mov edi, eax

	xor ecx,ecx
	.PageDirLoop:
		mov eax, 1024*4  ; each PD is 4kb
		mul cx		; PDE no * 4KB
		Mul32EAX	
		add eax, GDTStartRAM+(1024*4) ; 1MB+4KB+(PDE No*4KB)
	
		mov dword [ds:edi], eax
		and dword [ds:edi], 0FFFFF000h 	   ;Leave upper 20 Bits
		or  dword [ds:edi], 0011h 	   ;Supervisor, Present
		
		add edi,4		; each PDE is 32 bits (4bytes)

		inc cx
		cmp cx,1024
		jl .PageDirLoop
	

	mov edi, GDTStartRAM+(1024*4) ; 1MB + 4KB
	.PageTabLoop:
		mov dword [ds:edi], 0
		add edi, 4
		cmp edi, (2*GDTStartRAM)+(1024*4) ; 2MB + 4KB
		jl .PageTabLoop

	pop ecx
	pop eax
	pop edi
	ret


;This procedure allocate certain PageTable entries to point kernel code
;It will allocate from the beginning of page table 0
_AllocateOSPages:	
	push edi
	push eax
	push ecx

	xor ecx,ecx
	mov edi, GDTStartRAM+(1024*4)+(StackStartPage*4)
	.StackPageTabLoop:
		mov eax, 4*1024			; Each page is 4 Kb
		mul ecx				; 
		Mul32EAX	
		add eax, StackAddress		; Hardware address required

		mov dword [ds:edi], eax
		and dword [ds:edi], 0FFFFF000h 	   ;Leave upper 20 Bits
		or  dword [ds:edi], 0011h 	   ;Supervisor, Present

		add edi, 4			; PTE is 32 bits(4 bytes)
	
		inc ecx
		cmp ecx, StackPages
		jl .StackPageTabLoop



	xor ecx,ecx
	mov edi, GDTStartRAM+(1024*4)+(GDTStartPage*4) ; 1MB + 4KB - 0th page
	.GDTPageTabLoop:
		mov eax, 4*1024			; Each page is 4 Kb
		mul ecx				; 
		Mul32EAX	
		add eax, GDTAddress		; Hardware address required

		mov dword [ds:edi], eax
		and dword [ds:edi], 0FFFFF000h 	   ;Leave upper 20 Bits
		or  dword [ds:edi], 0011h 	   ;Supervisor, Present

		add edi, 4			; PTE is 32 bits(4 bytes)
	
		inc ecx
		cmp ecx, GDTPages
		jl .GDTPageTabLoop


	xor ecx,ecx
	mov edi, GDTStartRAM+(1024*4)+(OSStartPage*4) ; 1MB + 4KB + (osstartpage)
	.PageTabLoop:
		mov eax, 4*1024			; Each page is 4 Kb
		mul ecx				; 
		Mul32EAX	
		add eax, KernelAddress		; Hardware address required

		mov dword [ds:edi], eax
		and dword [ds:edi], 0FFFFF000h 	   ;Leave upper 20 Bits
		or  dword [ds:edi], 0011h 	   ;Supervisor, Present

		add edi, 4			; PTE is 32 bits(4 bytes)
	
		inc ecx
		cmp ecx, OSPages
		jl .PageTabLoop


	xor ecx,ecx
	mov edi, GDTStartRAM+(1024*4)+(PDStartPage*4)
	.PDPageTabLoop:
		mov eax, 4*1024			; Each page is 4 Kb
		mul ecx				; 
		Mul32EAX	
		add eax, PDAddress		; Hardware address required
		mov dword [ds:edi], eax
		and dword [ds:edi], 0FFFFF000h 	   ;Leave upper 20 Bits
		or  dword [ds:edi], 0011h 	   ;Supervisor, Present

		add edi, 4			; PTE is 32 bits(4 bytes)
	
		inc ecx
		cmp ecx, PDPages
		jl .PDPageTabLoop

	xor ecx,ecx
	mov edi, GDTStartRAM+(1024*4)+(VGAStartPage*4)
	.VGAPageTabLoop:
		mov eax, 4*1024			; Each page is 4 Kb
		mul ecx				; 
		Mul32EAX	
		add eax, VGAAddress		; Hardware address required

		mov dword [ds:edi], eax
		and dword [ds:edi], 0FFFFF000h 	   ;Leave upper 20 Bits
		or  dword [ds:edi], 0011h 	   ;Supervisor, Present

		add edi, 4			; PTE is 32 bits(4 bytes)
	
		inc ecx
		cmp ecx, VGAPages
		jl .VGAPageTabLoop

	pop ecx
	pop eax
	pop edi
	
	ret

;allocate pages for the kernel
;Input  : NoOfBytes required
;Output : Starting LinearSpace address in eax
_AllocateMemory:
	prologue 4

	push ecx
	push edx

	xor edx,edx
	mov eax, dword [ebp+8]
	mov ecx, 1024*4
	div dword ecx

	cmp edx,0
	je .SkipAdd
	inc eax

	.SkipAdd:
	
	push eax
	call _AllocatePages
	add esp,4	

	pop ecx
	pop edx
		
	epilogue	
	ret
;allocate pages for the kernel
;Input :  NoOfPages
;Output : Starting LinearSpace address
_AllocatePages:
	prologue 4

	push edi
	push ecx

	xor eax,eax	
	mov cx,[KernelPagesAllocated]
	add cx, [ebp+8]
	cmp [VritualPagesAvailable],cx 
	ja .PageNotAvailable
	
	xor ecx,ecx
	mov eax, 4
	mul word [KernelPagesAllocated]			; start page
	Mul32EAX	
	mov edi, GDTStartRAM+(1024*4)
	add edi, eax

	.PageTabLoop:
		mov eax, 4096			; Each page is 4 Kb
		mul word [KernelPagesAllocated]	; 
		Mul32EAX
		
		mov dword [ds:edi], eax
		and dword [ds:edi], 0FFFFF000h 	; Leave upper 20 Bits
		or  dword [ds:edi], 0011h 	; Supervisor, Present

		add edi, 4			; PTE is 32 bits(4 bytes)
	
		inc word [KernelPagesAllocated]
		inc ecx
		cmp ecx, [ebp+8] 		; no of pages offset 20 because we pushed
		jl .PageTabLoop

	xor ecx,ecx				;Calculating the linear address required to return
	mov cx, [KernelPagesAllocated]
	sub ecx, [ebp+8]
	mov eax, 4096
	mul cx
	Mul32EAX	
	
	.PageNotAvailable:
	pop ecx
	pop edi
	
	epilogue
	ret