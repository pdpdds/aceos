/*---------------------Ace OS source Code-----------------------
		Created by Sam on 11-Aug-2002(samuelhard@yahoo.com)
 This file is a header file for Heap manager
--------------------------------------------------------------*/
#ifndef HEAP_H
#define HEAP_H

#include "TypeDefs.h"
#ifdef __cplusplus
	extern "C" {
#endif

void * AllocHeap(UINT32 NoOfBytes);
void * DeAllocHeap(void * Address);

#ifdef __cplusplus
	}
#endif

#endif
