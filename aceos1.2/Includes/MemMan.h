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
extern UINT16 mmRAMInstalled;
extern UINT32 mmKernelPDAddress;
DLLEXPORT UINT32 AllocateKernelPages(UINT32 NoOfPages);
DLLEXPORT UINT32 CreatePageDirectory(UINT32 CodeSize);
DLLEXPORT UINT32 GetRAMHardwareAddress(UINT32 * PageDirectory, UINT32 * LinearAddress);


#ifdef __cplusplus
	}
#endif

#endif
