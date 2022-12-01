/* $Id: morecore.c,v 1.1.1.1 2002/12/21 09:50:19 pavlovskii Exp $ */

#include <os/Kernel32.h>

void *__morecore(size_t nbytes)
{
	//return VmmAlloc(, 0, VM_MEM_USER | VM_MEM_READ | VM_MEM_WRITE);
    return VirtualAlloc(0, PAGE_ALIGN_UP((int) nbytes) / VASM_PAGE_SIZE, MEM_COMMIT | MEM_RESET, VASM_MEM_USER | PAGE_READWRITE );
}
