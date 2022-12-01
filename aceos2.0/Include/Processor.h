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
        
#define CPUID_MAX_BASIC_LEVEL       10
#define CPUID_MAX_EXTENDED_LEVEL    10

typedef struct 
{
    UINT32 EAX;
    UINT32 EBX;
    UINT32 EDX;
    UINT32 ECX;
}__attribute__ ((packed)) CPUID_DATA;

extern CPUID_DATA sysCPUBasicInformation[CPUID_MAX_BASIC_LEVEL];
extern CPUID_DATA sysCPUExtendedInformation[CPUID_MAX_EXTENDED_LEVEL];   
        
void DetectProcessor();

void SetExceptionHandlers();

#ifdef __cplusplus
	}
#endif

#endif


