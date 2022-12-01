/*------------------------Interrupt / IDT related Functions-----------------------------
				Created by Samuel on 27-Aug-2002
				samuelhard@yahoo.com
*/
#ifndef __PROCESSOR_H
#define __PROCESSOR_H

#include <TypeDefs.h>

#ifdef __cplusplus
	extern "C" {
#endif

extern char _CPU_ProcessorID[];
extern char _CPU_VendorID[];
extern UINT32 _CPU_Signature;
extern UINT32 _CPU_Features_EBX;
extern UINT32 _CPU_Features_EDX;
extern UINT32 _CPU_Features_ECX;

void DetectProcessor();

UINT16 CreateGDTEntry(UINT32 FirstDWORD, UINT32 SecondDWORD);
void ClearGDTEntry(UINT16 Selector);

void SetIDT(BYTE InterruptNo, void (* IntHandler)());
void * GetIDT();
void SetExceptionHandlers();

#ifdef __cplusplus
	}
#endif

#endif


