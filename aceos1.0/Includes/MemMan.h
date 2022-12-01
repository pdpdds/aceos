/*---------------------Ace OS source Code-----------------------
		Created by Sam on 11-Aug-2002(samuelhard@yahoo.com)
 This file is a header file for Memory manager
--------------------------------------------------------------*/
#ifndef MEMMAN_H
#define MEMMAN_H

#include "TypeDefs.h"
#ifdef __cplusplus
	extern "C" {
#endif
extern UINT32 MemoryAmount;
void * AllocateMemory(UINT32 NoOfBytes);
void DeAllocateMemory(void * Address);
#ifdef __cplusplus
	}
#endif

#endif
