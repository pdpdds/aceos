/*
  Name: 		System Information
  Author: 			- Sam (samuelhard@yahoo.com)
  Date: 		11-Jun-2003 15:33
  
*/
#include <TypeDefs.h>
#include <Memory.h>
#include <Processor.h>
#include <SysInfo.h>
#include <stdio.h>
#include <string.h>

DWORD dw_SysLastError=0;
DWORD GetSystemInfo(struct _SYSTEM_INFO * SysInfo )
{
    int i;
    for(i=0;i<12;i++)
        SysInfo->CPU_VendorID[i]=((char *)&sysCPUBasicInformation)[4+i];
    SysInfo->CPU_VendorID[12]=0;
    SysInfo->CPU_Signature = SysInfo->CPU_FeatureFlags[0] =SysInfo->CPU_FeatureFlags[1] = SysInfo->CPU_FeatureFlags[2] = 0;
    //printf("\n\r GetSystemInfo : %X",SysInfo);
    //printf("\n\r GetSystemInfo : Basic Max Level %d Extended Max Level %d", sysCPUBasicInformation[0].EAX, sysCPUExtendedInformation[0].EAX);
    if ( sysCPUBasicInformation[0].EAX > 0 )
    {
        SysInfo->CPU_Signature = sysCPUBasicInformation[1].EAX;
        SysInfo->CPU_FeatureFlags[0] = sysCPUBasicInformation[1].EBX;
        SysInfo->CPU_FeatureFlags[1] = sysCPUBasicInformation[1].ECX;
        SysInfo->CPU_FeatureFlags[2] = sysCPUBasicInformation[1].EDX;
    }
    
    SysInfo->MEM_PhysicalAvailable = mm_PhysicalAvailable;
    SysInfo->MEM_PhysicalTotal = mm_PhysicalTotal;
    SysInfo->MEM_PageSize = VASM_GetPageSize() ;
    SysInfo->MEM_AllocationGranularity = 4096;
    
    //printf("\n\r GetSystemInfo :: %d %d", SysInfo->MEM_PhysicalAvailable, SysInfo->MEM_PhysicalTotal);
    return 0;
}
DWORD GetLastError()
{
    //printf ("\n\r KE GetLastError %X", dw_SysLastError );
    return dw_SysLastError;
}
DWORD SetLastError(DWORD dwErrorCode)
{
    return dw_SysLastError=dwErrorCode;
}
