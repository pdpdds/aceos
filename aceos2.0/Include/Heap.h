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
#ifndef _HHEAPINFO_DEFINED
    #define _HHEAPINFO_DEFINED
    struct Heap
    {
        UINT32 dwSize;             //size of the memory segement
        BOOLEAN IsUsed;          //Free or Occupied  
        struct Heap * Next;      //pointer to next memory segment
        struct Heap * Previous;  //pointer to next memory segment
    }__attribute__ ((packed));
    typedef struct Heap HEAP;
    typedef HEAP * LPHEAP;
    
    struct HeapInfo
    {
        LPHEAP hHeapRoot;
        UINT32 dwOptions;
        UINT32 dwMaximumSize;
        UINT32 dwCurrentSize;
    }__attribute__ ((packed));
    typedef struct HeapInfo HEAPINFO;
    typedef HEAPINFO * LPHEAPINFO;
#endif
    
LPHEAPINFO HeapCreate(UINT32 dwOptions, UINT32 dwInitialSize, UINT32 dwMaximumSize);
void * HeapAlloc(LPHEAPINFO hHeapInfo, UINT32 dwSize);
void *HeapRealloc(LPHEAPINFO lpHeapInfo, void * lpAddress, UINT32 dwsize);
BYTE HeapFree(LPHEAPINFO hHeapInfo, void * lpAddress);

void * GlobalAlloc(UINT32 dwSize);
void * GlobalRealloc(void * lpAddress, UINT32 dwSize);
void * GlobalFree(void * lpAddress);

#ifdef __cplusplus
	}
#endif

#endif
