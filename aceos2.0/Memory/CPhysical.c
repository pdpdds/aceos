/*
------------------------------------------------------
               Physical Memory Manager for Ace Os
                      Created by Sam 13-June-2003
------------------------------------------------------
*/
#include <TypeDefs.h>
#include <TaskMan.h>
#include <Memory.h>
#include <SysInfo.h>

UINT32 mm_FreePhysicalPages;
UINT32 mm_AvailablePhysicalPages;

#define TOTAL_LOW_PHYSICAL_PAGES ((1024*1024)/VASM_PAGE_SIZE)
#define LOW_PHYSICAL_PAGE_BITMAP_SIZE (TOTAL_LOW_PHYSICAL_PAGES/8)
BYTE mm_LowPhysicalPageBitmap[LOW_PHYSICAL_PAGE_BITMAP_SIZE];    //to manage the first 1 MB physical memory

UINT32 VASM_GetTotalPhysicalPages()
{
    return mm_FreePhysicalPages;
}
UINT32 VASM_GetAvailablePhysicalPages()
{
    return mm_AvailablePhysicalPages;
}
/*The following LowPhysicalPage functions will work only on the memory below 1MB
----It does not touch the orginal physical bitmap, instead it uses it own bitmap mm_LowPhysicalPageBitmap for tracking
*/
void VASM_InitLowPhysicalPageStructures()
{
    int i;
    //-------replace this code by asking BIOS for the memory map
    for(i=0;i<25;i++)
        mm_LowPhysicalPageBitmap[i]=0xFF;  // first 100 kb is occupied
    for(;i<LOW_PHYSICAL_PAGE_BITMAP_SIZE;i++)
        mm_LowPhysicalPageBitmap[i]=0x00;   //all are free
    for(i=160;i<256;i++)
        mm_LowPhysicalPageBitmap[i]=0xFF;  // first 640KB to 1MB is occupied
    //------------------------------------------
}

/* This function try to allocate the given number of pages below 1 MB*/
UINT32 VASM_AllocateLowPhysicalPages(UINT32 dwStartPage, UINT32 dwLimitPage, UINT32 dwSize)
{
    DWORD dwAllocatedPage=0;
    //check paramater range
    if ( dwStartPage > TOTAL_LOW_PHYSICAL_PAGES || dwSize > TOTAL_LOW_PHYSICAL_PAGES )
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }
    
    //allocate page by searching bitmap
    dwAllocatedPage = mm_FindInBitmap( mm_LowPhysicalPageBitmap, PAGE_FREE, dwStartPage, dwSize, dwLimitPage);
    if ( dwAllocatedPage )
    {
        //now reserve the pages
        mm_C_S_BitsInBitmap( mm_LowPhysicalPageBitmap, PAGE_USED, dwAllocatedPage, dwSize );
        return dwAllocatedPage * VASM_PAGE_SIZE; //allocation succeed
    }
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    return 0; //allocation failed
}
UINT32 VASM_GetFirstLowPhysicalPage(UINT32 dwStartPage, UINT32 dwLimitPage)
{
    if ( dwStartPage > TOTAL_LOW_PHYSICAL_PAGES || dwLimitPage > TOTAL_LOW_PHYSICAL_PAGES )
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }
    return mm_FindInBitmap( mm_LowPhysicalPageBitmap, PAGE_FREE, dwStartPage, 1, dwLimitPage);
}
UINT32 VASM_FreeLowPhysicalPages(UINT32 dwStartPage, UINT32 dwSize)
{
    if ( dwStartPage > TOTAL_LOW_PHYSICAL_PAGES || dwSize > TOTAL_LOW_PHYSICAL_PAGES )
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return ERROR_INVALID_PARAMETER;
    }
    //clear the bitmap
    mm_C_S_BitsInBitmap( mm_LowPhysicalPageBitmap, PAGE_FREE, dwStartPage, dwSize );
    return ERROR_SUCCESS;
}
