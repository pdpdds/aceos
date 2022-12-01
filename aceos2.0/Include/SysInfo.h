
#ifndef __SYSINFO_H
#define __SYSINFO_H

#include <TypeDefs.h>
#include <Error.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SYSTEM_INFO_DEFINED
    #define _SYSTEM_INFO_DEFINED
    //this structure is used by Kernel32.h so modify kernel32.h if you do any changes here
    struct _SYSTEM_INFO 
    {
        char   CPU_VendorID[33]; 
        UINT32 CPU_Signature; 
        UINT32 CPU_FeatureFlags[3]; 
        
        UINT16 MEM_PhysicalAvailable;
        UINT16 MEM_PhysicalTotal;
        UINT16 MEM_PageSize;
        UINT32 MEM_AllocationGranularity;
    }__attribute__ ((packed)); 
    typedef struct _SYSTEM_INFO SYSTEM_INFO;
    typedef SYSTEM_INFO * LPSYSTEM_INFO;
#endif
    
extern DWORD dw_SysLastError;

DWORD GetSystemInfo(struct _SYSTEM_INFO * SysInfo );
DWORD GetLastError();
DWORD SetLastError(DWORD ErrorCode);
#ifdef __cplusplus
}
#endif
#endif

