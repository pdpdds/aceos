;------------------------------------------------------
;               Memory Manager for Ace Os
;                      Created by Sam 03-Aug-2002
;						Redesigned on 07-Oct-2002
;------------------------------------------------------
%include "Common.inc"
%include "VGAText.inc"
[BITS 32]
[SECTION .text]
;----------------------------------------------------------------------------------
;To control GDT Entries usage information a bitmap at 0 is used
;LDT is located at 2KB+0x20 and GDT is follows LDT
;Page Directory is located at MIA + RAMFreePages * 4096 +  1 * 4096
;First Page Table is located at MIA + RAMFreePages * 4096 +  2 * 4096
;**********************************************************************
;RAMFreePages is assumed to fully available it is not correct in certain cases
;systems may contain only 4, 8, 90MB. That is modulo of 128 should be considered
;----------------------------------------------------------------------------------

GLOBAL _mmRAMInstalled
GLOBAL _mmKernelPDAddress	

GLOBAL _InitMemMan
GLOBAL _AllocateKernelPages
GLOBAL _CreatePageDirectory
GLOBAL _GetRAMHardwareAddress
GLOBAL _Test

MAX_KERNEL_PAGES 	EQU 25000			; 25000 Pages can be allocatable (100 MB)
INITIAL_KERNL_PAGES 	EQU 512 		

GDT_StartPage 		EQU 0				; It is used for GDT Bitmap and IDT, GDT 
GDT_TotalPages 		EQU 20
GDT_Address 		EQU 0x0

Stack_StartPage 	EQU 20				; Kernel stack
Stack_TotalPages 	EQU 5
Stack_Address 		EQU 0x14000

Kernel_StartPage 	EQU 25 				; Kernel Code, data
Kernel_TotalPages 	EQU 200
Kernel_Address 		EQU 0x19000

VGA_StartPage 		EQU 254				; VGA memory mapped IO. GDT enry FE000
VGA_TotalPages 		EQU 2
VGA_Address 		EQU 0xB8000

GRAPHICS_StartPage	EQU 1024
GRAPHICS_TotalPages	EQU 38
GRAPHICS_Address	EQU 0xA0000

PD_StartPage 		EQU 256				; Page Directory and Tables
PD_TotalPages 		EQU 260
PD_Address 		EQU 0x100000

%macro M_AllocatePageTable 2
	push dword %1
	push dword %2
	call AllocatePageTable
	add esp, 8
%endmacro
;input : StartPages, NoOfPages, HardwareAddress
%macro M_AllocateInitialKPages 3
	push dword %1
	push dword %2
	push dword %3
	call Allocate_Initial_KPages
	add esp, 12
%endmacro
%macro M_IsAvailableNextNPages 2
	push dword %1
	push dword %2
	call IsAvailableNextNPages
	add esp, 8
%endmacro
%macro M_FindFreePage 1
	push dword %1
	call FindFreePage
	add esp, 4
%endmacro
;this procedure intializes the Paging system for Ace OS. 
;It builds the Page Directory and Page Table for Kernel and sets the Paging Bit in CR3
_InitMemMan:
	mov edi, 1024*1024			; This will detect the amount of RAM present in the system the memory scan starts from 1MB since the minimum req is 4 MB
.Detect:
	mov byte [ds:edi], 0x0f		; writing and comparing a bit pattern
	cmp byte [ds:edi], 0x0f
	jne .DetectDone
	mov byte [ds:edi], 0xf0		; writing and comparing a bit pattern
	cmp byte [ds:edi], 0xf0
	jne .DetectDone

	add edi,1024				; Next Kilobyte

	jmp .Detect
.DetectDone:							; now the edi contains the amount of RAM installed in bytes
	mov eax, edi
	xor edx, edx
	mov ecx, 1024*1024					;converting it to KB
	div ecx
	mov word [_mmRAMInstalled], ax  ;saving amount of RAM installed in MB
	
	mov cl,128						;calculating pages required to manage RAM Free Pages = RAMInstalled in MB/128+1
	div cl
	inc al
	mov byte [_mmRAMFreePages], al
	;--------setting the GDT Bitmap all to 0 
	mov edi, 0						; 0 - 4096
	mov ecx, 4096 / 4					; using dword fill 4096 bytes = 1024 dwords
	xor eax, eax						; fill content all 0's
	cld
	rep stosd
	;--------setting the RAMPagesFree to all 1 (all pages are free. later the Kernel will mark it pages as used)
	mov edi, [_mmKernel_MIA]				;filling 1 bitwise from 1 MB to 1MB+RAMFreePages 
	xor eax, eax
	mov al, [_mmRAMFreePages]				;each page is 4096 bytes long
	mov cx, 4096 
	Mul32EAX cx						;Now eax contains the double word required
	;add edi, eax
	mov ecx, eax
	mov eax, 0xFFFFFFFF					;fill content all 1's
	rep stosd 
	;-------setting other Memory Informations to 0
	mov edi, [_mmKernel_MIA]				;filling 1 bitwise from 1 MB to 1MB+RAMFreePages 
	xor eax, eax
	mov al, [_mmRAMFreePages]				;each page is 4096 bytes long
	mov cx, 4096 
	Mul32EAX cx						;Now eax contains the double word required
	add edi, eax
	
	mov dword[_mmKernelPDAddress], edi		
	add dword[_mmKernelPDAddress], 4096			;filling the kernels PageDirectory address


	mov eax, 3						; 3 pages(1 page for Control Bitmap, 1 for PD, 1 for PT)
	mov cx, 4096 
	Mul32EAX cx						;Now eax contains the double word required
	mov ecx, eax
	xor eax, eax						;fill content all 0's
	rep stosd 

	;--------------------------*********************----------------------------------
	xor edi,edi						; setting first 5 GDT entries as assigned in GDTBitmap
	mov byte [ds:edi],0x1F
	;----setting up One Page Directory 
	xor eax, eax	
	mov al, [_mmRAMFreePages]		
	inc eax							; 1 page for Bitmap
	mov cx, 4096
	Mul32EAX cx						
	add eax, [_mmKernel_MIA]
	add eax, 4096						;eax points the starting address of first Page Table
	M_AllocatePageTable eax, 0
	add eax, 4096
	M_AllocatePageTable eax, 1


	;----Assign first PT entries from 0 to end of MIA 
	M_AllocateInitialKPages GDT_Address,  GDT_TotalPages, GDT_StartPage
	M_AllocateInitialKPages Stack_Address, Stack_TotalPages, Stack_StartPage
	M_AllocateInitialKPages Kernel_Address, Kernel_TotalPages, Kernel_StartPage
	M_AllocateInitialKPages VGA_Address, VGA_TotalPages, VGA_StartPage
	M_AllocateInitialKPages PD_Address, PD_TotalPages, PD_StartPage
	M_AllocateInitialKPages GRAPHICS_Address, GRAPHICS_TotalPages, GRAPHICS_StartPage

	xor eax, eax						; return value
	mov al, [_mmRAMFreePages]		
	inc eax							; 1 page for Bitmap
	mov cx, 4096
	Mul32EAX cx						;Now eax contains Page Directory starting byte offset from 1 MB
	add eax, [_mmKernel_MIA]				; Page Directory address

	ret
;this function allocates a Page Directory and updates the Page Directory Entry
;input : Page Table No, Page Table Address
;returns none
AllocatePageTable:
	prologue 8

	push edi
	push eax
	push ecx

	mov edi, [_mmKernel_MIA]				; pointing start of Memory Info Addresss (1MB)
	xor eax, eax	
	mov al, [_mmRAMFreePages]		
	inc eax							; 1 page for Bitmap
	mov cx, 4096
	Mul32EAX cx						
	add edi, eax						; Now eax contains Page Directory starting byte offset from 1 MB
	mov eax, [ebp+8]					; Page Table No
	mov cx, 4
	Mul32EAX cx
	add edi, eax

	mov eax, [ebp+12]					;Page Table Address
	and eax, 0FFFFF000h 	 				;Leave upper 20 Bits
	or  eax, 0011b 	  	 				;Supervisor, Present
	mov dword [ds:edi], eax					;setting first PDEntry

	pop ecx
	pop eax
	pop edi

	epilogue
	ret
;this function allocates required pages in initial stage, it also sets the PagesFree control bitmap
;**********only the InitMemMan() should call this function.
;input : StartPages, NoOfPages, HardwareAddress
;returns none
Allocate_Initial_KPages:
	prologue 12
	
	push ecx
	push edi
	push esi
	
	xor ecx,ecx
	xor eax, eax
	
	mov ebx, [ebp+8]					; Starting Page No 
	mov esi, [_mmKernel_MIA]				; calculating address of KernelPagesFree Control Bitmap
	mov al , [_mmRAMFreePages]		
	mov cx , 4096						; each page is 4096
	Mul32EAX cx							
	add eax, 1024 / 8					; Skipping the PageDirectory entries (1024 entries)
	add esi, eax						; esi points to KernelPagesFree Control Bitmap
		
	mov edi, [_mmKernel_MIA]
	xor eax, eax
	mov al , [_mmRAMFreePages]		
	add eax, 2						; 1 page for Bitmap and 1 for PD
	mov cx, 4096						; each page is 4096
	Mul32EAX cx						; Now eax contains Page Directory starting byte offset from 1 MB
	add edi, eax					
	
	mov eax, [ebp+8]					; Calculation for PT Entries
	mov cx, 4						; PT Entry is 4 byte long
	Mul32EAX cx						; eax = required page directory entry
	add edi, eax					

	xor ecx,ecx
	xor eax,eax

	.PageTableLoop:
		mov eax, 4096					; Each page is 4 Kb
		Mul32EAX ecx				 
		add eax, [ebp+16]	   			; Hardware address 

		and eax , 0FFFFF000h 	   			; Leave upper 20 Bits
		or  eax , 0011b 	   	   		; Supervisor, always Present
		mov dword [ds:edi], eax

		bts [ds:esi], ebx				; setting the bitmap 
		inc ebx							; *****only 65535 entries can be settable
	
		add edi, 4						; PTE is 32 bits(4 bytes)
	
		inc ecx
		cmp ecx, [ebp+12]				; comparing no of pages
		jl .PageTableLoop
		
	pop esi
	pop edi
	pop ecx
	
	epilogue
	ret
;Allocate pages for the kernel
;Input :  NoOfPages
;Output : Starting LinearSpace address
;---------Note
;stack local variables - [esp-4]  - FindFreePageResult
;						 [esp-8]  - No of pages required - decrement
;						 [esp-12] - FindFreePageResult - increment
_AllocateKernelPages:
	prologue 4
	
	push edi
	push ecx
	push ebx
	push edx
	
	sub esp, 20			; local variable space
	
	xor ecx, ecx
	.FindPageLoop:			;---------finding the free pages avl by searching the control bitmap
		M_FindFreePage ecx					; finds the free page and stores the result in eax
		cmp eax, 0
		jz .shortNotAvailable
		mov edx, eax						; preserving the result of M_FindFreePage
		mov [esp+4], edx
		mov [esp+12], edx
		
		M_IsAvailableNextNPages [ebp+8], eax	; sets eax to 0 if no of pages not avl
		cmp eax, 0
		jnz .FindRAMPage
		inc ecx									; for testing next M_FindFreePage
		jmp .FindPageLoop
	.shortNotAvailable:				; this is created because to avoid short jump out of range
		jmp .NotAvailable
	.FindRAMPage:			;--------finding free RAM pages avl
	mov ecx, [ebp+8]		;no of pages required
	mov [esp+8], ecx		;storing it in the local variable
	.FindRAMPageLoop:
		call FindFreeRAMPage 
		cmp eax, 0
		jne .AfterSwapping
		Call SwapPage  		; If no page is avl swap a RAM page
		jmp .NotAvailable
		.AfterSwapping:
		mov ebx, eax		; moving the result to ebx
		mov edi, [_mmKernel_MIA]		;----seting the page occupied bit and RAM occupied bit
		;Note:------------*** Only works upto 512 MB due to bt instruction*******************
		btr [ds:edi], eax			; clear the bit for the page 
		
		xor eax, eax					;----seting the page occupied bit and PagesFree occupied bit
		mov edi, [_mmKernel_MIA]			; calculating address of KernelPagesFree Control Bitmap
		mov al , [_mmRAMFreePages]		
		mov cx , 4096						; each page is 4096
		Mul32EAX cx							
		add eax, 1024 / 8					; Skipping the PageDirectory entries (1024 entries)
		add edi, eax						; esi points to KernelPagesFree Control Bitmap
		bts [ds:edi], edx
	
		mov eax, 4096		; operations for obtaining RAM Hardware address
		Mul32EAX bx			; now eax contains the RAM address of the Page
		mov ebx, eax
		
		;----storing it in the page table
		
		;finding byte offset 
		mov edi, [_mmKernelPDAddress] 		; edi points PD
		Div32EAX edx, 1024
		mov cx, 4							; PDE are 4 bytes
		Mul32EAX cx
		add edi, eax						; edi points to required PDE		
		mov edi, [ds:edi]
		and edi, 0FFFFF000h 				; Leave upper 20 Bits
		
		mov cx, 4							; each PTE is 4 bytes
		mov ax, dx
		Mul32EAX cx
		add edi, eax
	
		;---refining the hardware address and access type		
		and ebx, 0FFFFF000h 	; Leave upper 20 Bits
		or  ebx, 0011b 			; Supervisor, Present
		mov dword [ds:edi], ebx
		
		inc dword [esp+12]
		mov edx, [esp+12]
		
		dec dword [esp+8]		; no of pages required
		cmp dword [esp+8], 0
		
		jg .FindRAMPageLoop
		mov eax, [esp+4]		;find free page result
		jmp .Done
	.NotAvailable:
		xor eax, eax
	.Done:
	
	mov cx, 4096
	Mul32EAX cx
	
	add esp, 20			;recover local variable space
	
	pop edx
	pop ebx
	pop ecx
	pop edi
	 
	epilogue
	ret
;this function is not implemented
SwapPage:
	ret
;this internal function returns the first RAM free page found
;Input  : none
;Output : FreePageNo
FindFreeRAMPage:
	push ecx
	push ebx
	push edi
	push edx
	
	xor eax, eax
	mov al, [_mmRAMFreePages]
	mov cx, 4096 * 8				; Each Page is 4096 bytes and each byte is 8 entires
	Mul32EAX cx						; Number of entires in the RAMFreePage Bitmap
	mov ecx, eax					; preserving it to ecx

	xor ebx, ebx					; it contains 128 MB increments
	mov edi, [_mmKernel_MIA]
	mov eax, INITIAL_KERNL_PAGES	; starting page
	;Note:------------*** Only works upto 512 MB due to bt instruction*******************
	.Loop:
		bt [ds:edi], eax			; check the page 
		jc .Done					; found a free page
		inc eax						; not free goto next page
		cmp eax, ecx				
		jl .Loop
	
	xor eax, eax					; not found return 0
	.Done:
	
	pop edx
	pop edi
	pop ebx
	pop ecx
	
	ret
;this internal function returns the first free page after the page specified
;Input  : AfterPage
;Output : FreePageNo
FindFreePage:
	prologue 4
	
	push edi
	push ecx
	push edx
	
	mov edi, [_mmKernel_MIA]
	xor eax, eax
	mov al, [_mmRAMFreePages]
	mov cx, 4096 
	Mul32EAX cx						; Now eax contains the number of bytes occupied by RAMFreePage Bitmap
	add eax, 1024 / 8				; Skipping the PageDirectory entries (1024 entries)
	add edi, eax


	mov eax, [ebp+8]				; starting page
	.Loop:
		bt [ds:edi], eax			; check the page 
		jnc .Done					; found a free page
		inc eax						; not free goto next page
		cmp eax, MAX_KERNEL_PAGES
		jl .Loop
	
	xor eax, eax					; not found return 0
	.Done:
	
	pop edx
	pop ecx
	pop edi
	
	epilogue
	ret
;this internal function checks the PageFree Bitmap and returns true (1) if N pages are free.
;Input  : AfterPage, NoOfPages
;Output : 1 if all are available
IsAvailableNextNPages:
	prologue 8
	
	push edi
	push ecx
	push edx
	
	mov edi, [_mmKernel_MIA]
	xor eax, eax
	mov al, [_mmRAMFreePages]
	mov cx, 4096 
	Mul32EAX cx						; Now eax contains the address where PageFreeBitmap starts
	add eax, 1024 / 8				; Skipping the PageDirectory entries (1024 entries)
	mov edi, eax
	
	mov eax, [ebp+8]				; starting page
	mov ecx, [ebp+12]				; no of pages
	.Loop:
		bt [ds:edi], eax			; check the page 
		jc .BitSet					; found a occupied page 
		inc eax						; free goto next page
		cmp eax, MAX_KERNEL_PAGES
		jge .BitSet					; Exceed the limit
		dec ecx
		cmp ecx, 0					; test completed?
		jnz .Loop
	jmp .Done
	.BitSet:
		xor eax, eax				; not all free return 0
	.Done:
	
	pop edx
	pop ecx
	pop edi
	
	epilogue
	ret
;this function returns the hardware address of the page
;Input  : PageDirectory Address, Linear Address
;Output : Hardware address
_GetRAMHardwareAddress:
	prologue 12
	push edi
	push edx
 	push ecx
	
	xor eax, eax
	Div32EAX [ebp+12], 4096		; finding the page (ax - contains the required page)
	mov cx, ax
	xor eax, eax
	mov ax, cx	
	Div32EAX eax, 1024			; now ax-Page Table no and dx-PTE for the required Page
	
	mov edi, [ebp+8]			; page directory address
	mov cx, 4					; each PDE is 4 byte
	Mul32EAX cx
	add edi, eax				; edi points the required PDE
	
	mov edi, [ds:edi]			; getting the Page Table address
	and edi, 0FFFFF000h 		; Leave upper 20 Bits
	
	xor eax,eax					; skipping page table entry 
	mov ax, dx					; Page offset
	mov cx, 4					; each PTE is 4 byte
	Mul32EAX cx
	add edi, eax				; edi points the required PTE
	
	mov eax, [ds:edi]
	and eax, 0FFFFF000h 		; Leave upper 20 Bits
	
	pop edi
	pop edx
	pop ecx
	epilogue
	ret
;this function allocates the required pages for the task and returns the starting linear address
;Input : PageDirectory Address in Kernel Linear Space, NoOfPages
;C Style call AllocatePageForTask(UINT32 PDAddress,UINT32 NoOfPages)
;Note: - This function should call the GetTaskPageBitmap(PageBitmap) to get the Page Free Bitmap of the task
_AllocatePageForTask:
	ret

_mmRAMInstalled		dw 0		; amount of RAM installed in the system in MB
_mmRAMFreePages		db 0  		; amount of Pages required to manage RAM Free Pages in the system
ALIGN 32				; aligned to 32 bits since this variable will be accessed frequently
_mmKernel_MIA 		dd 1024*1024	; this variable holds the address where MIA of kernel begins. 
_mmKernelPDAddress	dd 0		; Kernel's Page Directory Address
