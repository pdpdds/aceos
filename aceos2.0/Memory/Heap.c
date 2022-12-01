/*
	Heap Memory Manager for Ace OS Kernel / Applications
			- Created by Samuel on 08-Aug-2002 (samuelhard@yahoo.com)
*/

#include "TypeDefs.h"
#include "Memory.h"
#include "TaskMan.h"
#include "Heap.h"
#include "string.h"


//#define DEBUG

#define PADSIZE 0  

#ifdef DEBUG
    UINT32 DbgAllocated=0,DbgFreed=0,DbgAllocPages=0;
#endif
void DumpHeapInfo(LPHEAP Root )
{
    dbgprintf("\n HEAP :: Dump - ");
    while( Root )
    {
        dbgprintf("[%x]",Root);
        dbgprintf("%s(%ld)",Root->IsUsed?"*":"",Root->dwSize );
        Root=Root->Next;
    }
    dbgprintf(" $$ End");
}
LPHEAPINFO HeapCreate(UINT32 dwOptions, UINT32 dwInitialSize, UINT32 dwMaximumSize)
{
    UINT32 dwReqPages=(dwInitialSize+sizeof(HEAPINFO))/VASM_PAGE_SIZE+1;
    #ifdef DEBUG
        dbgprintf("\n HEAP :: Create(%X,%ld,%ld)", dwOptions,dwInitialSize, dwMaximumSize );
    #endif
    LPHEAPINFO hHeapInfo=(LPHEAPINFO)VirtualAlloc(0, dwReqPages , MEM_RESERVE | MEM_COMMIT, 0 );
    if ( hHeapInfo == NULL )
        return NULL;
    #ifdef DEBUG
        dbgprintf("\n\t VirtualAlloc() :: %X", hHeapInfo );
    #endif
    LPHEAP hHeapRoot=(LPHEAP)( (UINT32)hHeapInfo + sizeof(HEAPINFO) );
    hHeapRoot->Next=hHeapRoot->Previous=NULL;
    hHeapRoot->IsUsed=FALSE;
    hHeapRoot->dwSize=(dwReqPages * VASM_PAGE_SIZE)-sizeof(HEAPINFO);
    
    hHeapInfo->hHeapRoot=hHeapRoot;
    hHeapInfo->dwOptions=dwOptions;
    hHeapInfo->dwCurrentSize=(dwReqPages * VASM_PAGE_SIZE)-sizeof(HEAPINFO);
    hHeapInfo->dwMaximumSize=dwMaximumSize;
    return hHeapInfo;
}
//Allocate Heap memory to process
void * HeapAlloc(LPHEAPINFO hHeapInfo, UINT32 dwSize)
{
    dwSize+=PADSIZE;
    #ifdef DEBUG
        dbgprintf("\n\r HEAP :: Alloc(%X,%ld) Allocated=%ld Pages=%ld Freed=%d HEAPSIZE=%d",hHeapInfo,dwSize, DbgAllocated,DbgAllocPages,DbgFreed, sizeof(HEAP) );
    #endif
    if ( hHeapInfo == NULL )    //invalid Heap Info
        return NULL;
    if ( dwSize <= 0 )          //invalid size
        return NULL;
    LPHEAP hHeap = hHeapInfo->hHeapRoot;
    #ifdef DEBUG
        DumpHeapInfo( hHeap );
        dbgprintf("\n HEAP :: Free Node Search with size [%ld] ",dwSize);
    #endif
    while( hHeap )
    {
        #ifdef DEBUG
            dbgprintf("[%X%s(%ld)]->",hHeap, hHeap->IsUsed?"*":"" ,(UINT32) hHeap->dwSize);
        #endif
        if ( !hHeap->IsUsed && hHeap->dwSize >= dwSize + sizeof(HEAP) )
            break;
        hHeap=hHeap->Next;
    }
    if ( hHeap == NULL ) //no free node is found with specified size
    {
        #ifdef DEBUG
            dbgprintf("\n HEAP :: Allocating Virtual %ld Pages ...",(UINT32) (dwSize/VASM_PAGE_SIZE) + 1);
        #endif
        LPHEAP hNewHeap=(LPHEAP)VirtualAlloc( 0, (dwSize/VASM_PAGE_SIZE)  + 1 , MEM_RESERVE | MEM_COMMIT, 0 );
        LPHEAP hOldHeap=hHeapInfo->hHeapRoot;
        
        if ( hNewHeap == NULL )
        {
            dbgprintf(" Virtual Alloc Failed.");
            return NULL;
        }
        #ifdef DEBUG
            dbgprintf("done. Starting VAS %X",hNewHeap);
            DbgAllocPages+=dwSize/VASM_PAGE_SIZE  + 1 ;
        #endif
        hNewHeap->dwSize=( ( (dwSize/VASM_PAGE_SIZE ) + 1 ) * VASM_PAGE_SIZE  )- sizeof( HEAP );
        hNewHeap->IsUsed=FALSE;
        hHeapInfo->hHeapRoot=hNewHeap;
        hNewHeap->Next = hOldHeap;
        hNewHeap->Previous = NULL;
        if ( hOldHeap != NULL )
            hOldHeap->Previous = hNewHeap;
        
        hHeap = hNewHeap;
        #ifdef DEBUG
            dbgprintf("\n HEAP :: hNewHeap->dwSize=%ld hNewHeap->Next=%X",hNewHeap->dwSize, hNewHeap->Next);
            DumpHeapInfo( hHeapInfo->hHeapRoot );
        #endif
        
    }
    #ifdef DEBUG
        dbgprintf("\n HEAP :: Required Heap Node %X (%ld)",hHeap, (UINT32)hHeap->dwSize );
    #endif
    
    if ( hHeap->dwSize > dwSize + sizeof(HEAP)*2 ) // if the required node contains more space for spliting it to two nodes
    {
        #ifdef DEBUG
            dbgprintf("\n HEAP :: Spliting .. ");
        #endif
        #ifdef DEBUG
            dbgprintf(" hFreeHeap -");
        #endif
        LPHEAP hFreeHeap = hHeap; // creating a free node based on the excess space on the current node
        hFreeHeap->dwSize = hFreeHeap->dwSize -( dwSize + sizeof(HEAP)*2 ); // excess size 
        hFreeHeap->IsUsed =FALSE; //free
        
        #ifdef DEBUG
            dbgprintf(" %X(%ld)", hFreeHeap,hFreeHeap->dwSize);
            dbgprintf(" ReqNode -");
        #endif
        //the required node is placed after the excess space
        hHeap = (HEAP *) ((UINT32)hFreeHeap + sizeof(HEAP) + hFreeHeap->dwSize );
        hHeap->dwSize=dwSize;          //required size
        hHeap->IsUsed=TRUE;       //occupied
        hHeap->Next = hFreeHeap->Next; 
        hHeap->Previous = hFreeHeap;
        #ifdef DEBUG
            dbgprintf(" %X(%d)", hHeap,hHeap->dwSize);
        #endif
        hFreeHeap->Next = hHeap; 
        
        #ifdef DEBUG
            dbgprintf("Free Node %X(%ld) hHeap %X(%ld)",hFreeHeap,hFreeHeap->dwSize, hHeap, hHeap->dwSize );
        #endif
    }
    else
    {
        #ifdef DEBUG
            dbgprintf("\n HEAP :: Found a node with equal size %X(%d)=%d", hHeap, hHeap->dwSize, dwSize + sizeof(HEAP));
        #endif
        hHeap->IsUsed = TRUE;
    }
    #ifdef DEBUG
        dbgprintf("\n HEAP :: Return %X",(HEAP *)((UINT32)hHeap+sizeof(HEAP)) );
        DbgAllocated+=dwSize;
    #endif
    return (HEAP *)((UINT32)hHeap+sizeof(HEAP));
}

//Deallocates the already allocated memory and retains the address for future use
BYTE HeapFree(LPHEAPINFO hHeapInfo, void * lpAddress)
{
    #ifdef DEBUG
        dbgprintf("\n\r HEAP :: Free(%X,%X)",hHeapInfo,lpAddress);
    #endif
    LPHEAP hHeap = hHeapInfo->hHeapRoot;
    
    while( hHeap )
    {
        #ifdef DEBUG
            dbgprintf("[%X%s]->",hHeap, hHeap->IsUsed?"*":"");
        #endif
        if ( !hHeap->IsUsed && (LPHEAP) ((UINT32)hHeap+sizeof(HEAP)) == (LPHEAP)lpAddress )
            break;
        hHeap=hHeap->Next;
        
    }
    if ( hHeap == NULL )
        return 0; // address specified not found or already freed
    
    hHeap->IsUsed=FALSE;
    return 1;
}

void *HeapRealloc(LPHEAPINFO lpHeapInfo, void * lpAddress, UINT32 dwSize)
{
    void * lpNewMem = HeapAlloc(lpHeapInfo, dwSize);
    if ( lpNewMem == NULL )
        return NULL;
    LPHEAP lpOldHeap = lpHeapInfo->hHeapRoot;
    while( lpOldHeap )
    {
        if ( (LPHEAP) ((UINT32)lpOldHeap+sizeof(HEAP)) == (LPHEAP)lpAddress )
            break;
        lpOldHeap=lpOldHeap->Next;
    }
    if ( lpOldHeap )
    {
        memcpy( lpNewMem, lpAddress, lpOldHeap );
        HeapFree( lpHeapInfo, lpAddress );
        return lpNewMem;
    }
    dbgprintf("\n HeapRealloc Failure ");
    return NULL;
}
void * GlobalAlloc(UINT32 dwSize)
{
    dbgprintf("\nGlobalAlloc called with %d", dwSize);
    LPTASK lpTask = GetTaskInfo( GetTaskID() );
    if ( lpTask == NULL )
        return NULL;
    LPHEAPINFO lpHeapInfo = lpTask->lpHeapInfo;
    if ( lpHeapInfo == NULL )
        return NULL;
    return HeapAlloc( lpHeapInfo, dwSize );
}
void * GlobalRealloc(void * lpAddress, UINT32 dwSize)
{
    LPTASK lpTask = GetTaskInfo( GetTaskID() );
    if ( lpTask == NULL )
        return NULL;
    LPHEAPINFO lpHeapInfo = lpTask->lpHeapInfo;
    if ( lpHeapInfo == NULL )
        return NULL;
    return HeapRealloc( lpHeapInfo, lpAddress, dwSize );
}
void * GlobalFree(void * lpAddress)
{
    LPTASK lpTask = GetTaskInfo( GetTaskID() );
    if ( lpTask == NULL )
        return NULL;
    LPHEAPINFO lpHeapInfo = lpTask->lpHeapInfo;
    if ( lpHeapInfo == NULL )
        return NULL;

    if ( HeapFree( lpHeapInfo, lpAddress ) )
        return NULL;
    else
        return lpAddress;
}


