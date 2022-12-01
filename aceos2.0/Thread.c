/*
  Name: 		Thread Specific Functions for Ace OS
  Author: 			- Samuel
  Description: 	This file contains neccessary routines to manage threads
  Date: 08-Aug-2003 22:12
*/

#define _DEBUG_

#include <TypeDefs.h>
#include <Memory.h>
#include <malloc.h>
#include <stdio.h>
#include <TaskMan.h>
#include <Thread.h>
#include <SysInfo.h>
#include <Util.h>

#define THREAD_IO_BASE_PAGES    2

/* These variables are used as a bridge between Timer and Scheduler*/
LPTHREAD sys_CurThread=NULL;
LPTHREAD sys_IdleThread=NULL;

struct ThreadStackInfo * sysCurThreadStackInfo=NULL;
    
/*This function creates a thread as a part of the calling task
*/
LPTHREAD GetCurrentThread()
{
    return sys_CurThread;
}
LPTHREAD CreateThread( void(*lpThreadAddress)(),  UINT16 wStackSize, UINT16 wCodeSelector, UINT16 wDataSelector, UINT32 dwEFlag, UINT32 dwRunState, UINT32 dwPriority, BOOLEAN IsV86Thread, BOOLEAN bAddToSysThreadList)
{
    LPTHREAD hThread;
    LPTASK lpTask;
    int i;
    
    lpTask = (LPTASK)GetTaskInfo(GetTaskID());    
    if ( lpTask == NULL )
    {
        DEBUG_PRINT_INFO("returns  NULL.");
        SetLastError( ERROR_INVALID_HANDLE );
        return NULL;
    }
    ALLOC_AND_CHECK( hThread, LPTHREAD, sizeof(struct ThreadInfo), NULL );
        
    if ( wStackSize < (STACK_MIN_SIZE+2) *4 )    //check for minimum requested stack size
        wStackSize = (STACK_MIN_SIZE+2)*4;       //if it is less than the minimum then choose the minimum size
        
    UINT32 * lpStack;
    ALLOC_AND_CHECK(lpStack, UINT32 *,  wStackSize, NULL); //allocate stack 1
    
    UINT32 * lpStackEnd = (UINT32 *)((UINT32)lpStack + wStackSize);
    hThread->pStackInfo.ESP1 = (UINT32)lpStackEnd;
    
    ALLOC_AND_CHECK(lpStack, UINT32 *,  wStackSize, NULL); //allocate stack 1
    lpStackEnd = (UINT32 *) ( (UINT32)lpStack + wStackSize );  //end position of stack
    for( i=0; i<STACK_MIN_SIZE; i++ )                     
        lpStackEnd[-i]=0;                               //zeroing
    
    lpStackEnd[-SP_CS] = wCodeSelector;              
    lpStackEnd[-SP_EIP] = (UINT32)lpThreadAddress;                 //Task starting position
    lpStackEnd[-SP_EFLAGS] = dwEFlag;
    hThread->pStackInfo.ESP0 = (UINT32)lpStackEnd - (STACK_MIN_SIZE*4);     
    
    lpStackEnd[-SP_P_SS] = lpStackEnd[-SP_DS] = \
    lpStackEnd[-SP_ES] = lpStackEnd[-SP_FS] = lpStackEnd[-SP_GS] = wDataSelector;        ///**********other rings???

    lpStackEnd[-SP_P_ESP] = lpStackEnd[-SP_ESP] = hThread->pStackInfo.ESP1;                 ///ring 1 stack
    
    hThread->lpStack = lpStack;                                 // not used?       
    
    hThread->pStackInfo.SS0 = KERNEL_DATA_SELECTOR;             //this should be KERNEL's Data selector because it is accessed during task switch
    hThread->pStackInfo.SS1 = wDataSelector;
    hThread->pStackInfo.SS2 = 0;
    
    DEBUG_PRINT_OBJECT2("ESP0 = %X ESP1 = %X", hThread->pStackInfo.ESP0,hThread->pStackInfo.ESP1); 
    //TODO:create only one stack here - remove others
    
    /*lpStack = (UINT32 *)malloc(StackSize);
    lpStackEnd = (UINT32)lpStack + StackSize;
    hThread->pStackInfo.ESP2=lpStackEnd;*/
        
    hThread->dwRunState = dwRunState;//thread is ready and not started
    hThread->dwPriority = dwPriority;
    hThread->dwWaitingTime=0;
    
    hThread->lpMessageBox=IPC_CreateMessageBox(DEFAULT_THREAD_MESSAGES);
    if ( hThread->lpMessageBox == NULL )
    {
        DEBUG_PRINT_INFO("IPC_CreateMessageBox() Failed.");
        //here the error message will be set by the IPC_CreateMessageBox()
        return NULL;
    }
    hThread->IOBASE = (BYTE *)VirtualAllocEx( sys_KernelTask.TaskID, 0, THREAD_IO_BASE_PAGES, MEM_RESERVE|MEM_COMMIT, TASK_MEM_PROTECTION );
    if ( hThread->IOBASE == NULL )
    {
        DEBUG_PRINT_INFO("VirtualAlloc failed while allocating memory for IOBase");
        SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        return NULL;
    }
    DEBUG_PRINT_OBJECT1("hThread->IOBASE %X", hThread->IOBASE);
    if ( IsV86Thread )
    {
        //set Interrupt Redirection bitmap and IO permission bitmap 
        for(i=0;i<THREAD_IO_BASE_PAGES*VASM_PAGE_SIZE;i++)
            hThread->IOBASE[i]=0;
        
        hThread->IOBASE += 32;          //skip Interrupt Redirection bitmap
    }
    else
    {
        //set IO permission bitmap
        for(i=0;i<THREAD_IO_BASE_PAGES*VASM_PAGE_SIZE;i++)
            hThread->IOBASE[i]=0xFF;
    }
    //add to the system thread list so that it will be scheduled for future execution
    if ( bAddToSysThreadList )
        List_Add( &lpTask->ThreadQueue, "", hThread);           
    
    SetLastError( ERROR_SUCCESS );
    return hThread;
}

/*useless, it returns the same hThread if it is valid*/
LPTHREAD GetThreadInfo(UINT32 TaskID, LPTHREAD hThread)
{
    LPTASK lpTask = GetTaskInfo(TaskID);
    if ( lpTask == NULL )
        return NULL;
    UINT32 TotThreads=List_Count( lpTask->ThreadQueue );
    int i;
    for(i=0;i<TotThreads;i++)     // Search througth tasks to get thread memory info
    {
        LPTHREAD TI=(LPTHREAD)List_GetData(lpTask->ThreadQueue, "", i);
        if ( TI != NULL &&  TI == hThread )
        {
            SetLastError( ERROR_SUCCESS );
            return TI;
        }
    }
    SetLastError( ERROR_INVALID_HANDLE );
    return NULL;
}
/* This function removes the given thread from the task list*/
UINT32 RemoveThread(UINT32 TaskID, LPTHREAD hThread)
{
    LPTASK lpTask = GetTaskInfo(TaskID);
    if ( lpTask == NULL )
        return 0;
    UINT32 TotThreads=List_Count( lpTask->ThreadQueue );
    int i;
    for(i=0;i<TotThreads;i++)     // Search througth tasks to get thread memory info
    {
        LPTHREAD TI=(LPTHREAD)List_GetData(lpTask->ThreadQueue, "", i);
        if ( TI != NULL &&  TI == hThread )
        {
            List_Delete(&lpTask->ThreadQueue, "", i);
            SetLastError( ERROR_SUCCESS );
            return 1;
        }
    }
    SetLastError( ERROR_INVALID_HANDLE );
    return 0;
}

