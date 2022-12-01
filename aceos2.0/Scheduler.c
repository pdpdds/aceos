/*           Ace OS Source Code - Task / Thread Scheduler
                      Developer - Samuel (samueldotj@gmail.com) 
                      Date - 16-Aug-2003 10:24pm
Credits :
        1) The Scheduling Algorithirm is borrowed from the VISOPSYS.(Task Priority, Waiting Time, Priority Ratio)
*/
//#define _DEBUG_

#include <TypeDefs.h>
#include <TaskMan.h>
#include <Thread.h>
#include <Scheduler.h>
#include <List.h>
#include <Memory.h>
#include <ObjMan.h>
#include <Timer.h>
#include <Util.h>

ATOMIC AtomSync={0};

struct ThreadReg * Scheduler()
{
    struct ListNode * pTaskNode=sys_TaskList;
    LPTASK lpTask=NULL;
    LPTHREAD lpThread=NULL;
    
    //dbgprintf("%d ",GetTimerTick());
    
    DEBUG_PRINT_INFO("--------------------------------------------");
    
    //TODOH : a leakage here - if two threads enters here at a same time then there is a problem
    if ( AtomSync.Counter != 0 )
        return 0;
    AtomicAdd( 1, &AtomSync );
    
    
    //selecting task
    if ( sysTimerQueue )
        TimerHand();
    if ( sysWaitForObjects )
        HandleObjects();
    DEBUG_PRINT_INFO("Selecting task :: ");
    while( pTaskNode )
    {
        #ifdef _DEBUG_
            dbgprintf(" [%X] ", pTaskNode);
        #endif
        LPTASK hNextTask=(LPTASK)pTaskNode->ptrData;              //get next Task handle
        if ( hNextTask )
        {
            UINT32 Weight1,Weight2=0;                           //determine the weight of the task
            Weight1=(TASK_PRIORITY_LEVELS-hNextTask->dwPriority)*TASK_PRIORITY_RATIO + hNextTask->dwWaitingTime;
            if ( lpTask )
                Weight2=(TASK_PRIORITY_LEVELS-lpTask->dwPriority)*TASK_PRIORITY_RATIO + lpTask->dwWaitingTime;
            if ( Weight1 > Weight2 )                           //select the weighteset task
                lpTask = hNextTask;
            
            hNextTask->dwWaitingTime++;                         //increase waiting time
        }
        pTaskNode=pTaskNode->Next;                              //move to next thread node
    }
    DEBUG_PRINT_OBJECT1("Selected Task - %X", lpTask);
    //selecting thread
    if ( lpTask )
    {
        if ( sys_CurTaskID  != lpTask->TaskID )
        {
            sys_CurTaskID = lpTask->TaskID;
            //here change the page directory
        }
        
        struct ListNode * pThreadNode=lpTask->ThreadQueue;       //get the ThreadQueue of the task
        UINT32 Weight1,Weight2=0;                       //determine the weight of the thread
        while( pThreadNode )
        {
            LPTHREAD hNextThread=(LPTHREAD)pThreadNode->ptrData;      //get next Thread handle
            
            if ( hNextThread )                                  
            {
                //selecting thread which is interruptable and not stopped
                //if ( !(hNextThread->dwRunState & (TASK_INTERRUPTIBLE|TASK_RUNNING) ) || hNextThread->dwRunState & TASK_STOPPED  )       
                if ( !(hNextThread->dwRunState & TASK_RUNNABLE)  )       
                {
                    DEBUG_PRINT_OBJECT1("Thread is Uninterruptable or Stopped. dwRunState = %X", hNextThread->dwRunState);
                    goto SkipThreadNoWaitTimeIncrease;
                }
                
                /*if ( ! (hNextThread->dwRunState & TASK_RUNNING) ) //thread is not running and woken only by messages
                {
                    if ( hNextThread->lpMessageBox == NULL )
                        goto SkipThread;
                    if ( hNextThread->lpMessageBox->wTotalMessages <= 0 )  //if no message then continue
                        goto SkipThread;
                }*/
                //now thread is runnable or interruptable with message
                Weight1=(TASK_PRIORITY_LEVELS-hNextThread->dwPriority)*TASK_PRIORITY_RATIO + hNextThread->dwWaitingTime;
                
                if ( Weight1 > Weight2 )                       //select the weighteset thread
                {
                    lpThread=hNextThread;        
                    Weight2=(TASK_PRIORITY_LEVELS-lpThread->dwPriority)*TASK_PRIORITY_RATIO + lpThread->dwWaitingTime;
                }
                else 
                {
                    if ( lpThread == NULL)
                        lpThread=hNextThread;
                }
                #ifdef _DEBUG_
                    if ( hNextThread!=lpThread && lpThread != NULL )
                    {
                        dbgprintf("\nhNextThread %X Waiting Time %d dwPriority %X Weight1 %ld ", hNextThread, hNextThread->dwWaitingTime, hNextThread->dwPriority, Weight1);
                        dbgprintf("\nlpThread    %X Waiting Time %d dwPriority %X Weight2 %ld", lpThread, lpThread->dwWaitingTime, lpThread->dwPriority, Weight2);
                    }
                #endif
            SkipThread:
                hNextThread->dwWaitingTime++;                   //increase the waiting time
            }    
            SkipThreadNoWaitTimeIncrease:
            
            pThreadNode=pThreadNode->Next;                      //goto next thread node
        }
        DEBUG_PRINT_OBJECT2("New Thread - %X old Thread %x", lpThread, sys_CurThread);
        //now lpTask and lpThread contains required task and thread structure respectively
        lpTask->dwWaitingTime=0;
        if ( lpThread == NULL )
        {
            DEBUG_PRINT_INFO("Selects IDLE Thread (lpThread is NULL)");
            //AtomicSub( 1, &AtomSync );
            //return NULL;
            lpThread = sys_IdleThread;
        }
       
        lpThread->dwWaitingTime=0;
        if ( lpThread == sys_CurThread ) //same thread no need to change anything
        {
            if ( lpThread->dwRunState & TASK_RUNNABLE )
            {
                DEBUG_PRINT_OBJECT1("Returns NULL [%d]",lpThread->dwRunState);
                AtomicSub( 1, &AtomSync );
                return NULL;
            
            }
            DEBUG_PRINT_OBJECT1("Returns IDLE THREAD [%X]", sys_IdleThread);
            lpThread = sys_IdleThread;
            //return sys_IdleThread;
        }
        if ( ! (lpThread->dwRunState & TASK_STOPPED) ) 
            lpThread->dwRunState |= TASK_RUNNING;
        
        if( sys_CurThread != lpThread )
        {
            sys_CurThread = lpThread;
            sysCurThreadStackInfo=&lpThread->pStackInfo;
        
            sys_TSS->SS0=sysCurThreadStackInfo->SS0;
            sys_TSS->SS1=sysCurThreadStackInfo->SS1;
            sys_TSS->SS2=sysCurThreadStackInfo->SS2;
        
            sys_TSS->ESP0=sysCurThreadStackInfo->ESP0;
            sys_TSS->ESP1=sysCurThreadStackInfo->ESP1;
            sys_TSS->ESP2=sysCurThreadStackInfo->ESP2;
        
            #ifdef _DEBUG_
                dbgprintf("\n New Thread = %X sysCurThreadStackInfo %X", sys_CurThread, sysCurThreadStackInfo );
                dbgprintf("\n SS0 %X:%X SS1 %X:%X SS2 %X:%X",sys_TSS->SS0,sys_TSS->ESP0,sys_TSS->SS1,sys_TSS->ESP1,sys_TSS->SS2,sys_TSS->ESP2);
            #endif
    
            if ( lpThread->IOBASE != NULL ) //if IO Bitmap is exists then map physical address of IO bitmap to the TSS->IOBase linear addresss
            {
                DEBUG_PRINT_OBJECT1("Setting Customized IOBASE %X", lpThread->IOBASE );
                SetPageEntry( ((UINT32 )( ((UINT32)sys_TSS)+sys_TSS->IOBASE))/VASM_PAGE_SIZE,    mm_KernelPageDirectory, VASM_TPT, (void *) GetPageEntry((UINT32)lpThread->IOBASE/VASM_PAGE_SIZE,     VASM_TPT )  , TASK_MEM_PROTECTION);
                SetPageEntry( (((UINT32)( ((UINT32)sys_TSS)+sys_TSS->IOBASE))/VASM_PAGE_SIZE)+1, mm_KernelPageDirectory, VASM_TPT, (void *) GetPageEntry(((UINT32)lpThread->IOBASE/VASM_PAGE_SIZE)+1, VASM_TPT )  , TASK_MEM_PROTECTION);
                DEBUG_PRINT_OBJECT2("lpThread->IOBASE - %X sys_TSS->IOBASE %X", lpThread->IOBASE, sys_TSS->IOBASE);
                
            }
            else            //if IO Bitmap not present then map the system default IO bitmap
            {
                DEBUG_PRINT_OBJECT2("lpThread->IOBASE is NULL - Setting Default IOBASE sys_TSS %X -> IOBase %X", sys_TSS, sys_TSS->IOBASE);
                SetPageEntry( ((UINT32) (((UINT32)sys_TSS)+sys_TSS->IOBASE))/VASM_PAGE_SIZE,    mm_KernelPageDirectory, VASM_TPT, (void *) sys_IOBasePhysicalAddress  , TASK_MEM_PROTECTION);
                SetPageEntry( (((UINT32)(((UINT32)sys_TSS)+sys_TSS->IOBASE))/VASM_PAGE_SIZE)+1, mm_KernelPageDirectory, VASM_TPT, (void *) sys_IOBasePhysicalAddress+VASM_PAGE_SIZE, TASK_MEM_PROTECTION);
            }
            //invalidate the IOBitmap pages
            __asm__ __volatile__ ("invlpg (%0)": :"r" (((UINT32)sys_TSS)+sys_TSS->IOBASE));
            __asm__ __volatile__ ("invlpg (%0)": :"r" (((UINT32)sys_TSS)+sys_TSS->IOBASE+ VASM_PAGE_SIZE));
        }
        
    }
    DEBUG_PRINT_OBJECT1("Returns %X",sysCurThreadStackInfo);
    AtomicSub( 1, &AtomSync );
    //DEBUG();
    return  (struct ThreadReg *)sysCurThreadStackInfo;
}
   
