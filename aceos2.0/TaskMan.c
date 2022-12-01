/*
  Name: 		Task Manager
  Author: 			- Samuel (samueldotj@gmail.com)
  Description: 	This file contains neccessary routines  to manage tasks
  Date: 28-Oct-02 19:47
  Modified 04-Jun-2003 23:00
        Replaced Collection with List
        Standard Library is used
*/

#define _DEBUG_

#include <TypeDefs.h>
#include <TaskMan.h>
#include <Thread.h>
#include <List.h>
#include <Processor.h>
#include <Memory.h>
#include <malloc.h>
#include <Heap.h>
#include <stdio.h>
#include <string.h>
#include <Timer.h>
#include <SysInfo.h>
#include <PE.h>
#include <Environment.h>
#include <Util.h>

#define INITIAL_KERNEL_HEAP 2000

// 1 page for TSS contents and 2 pages for IOBase bitmap (65535 ports)
#define TASK_TSS_PAGES 3

#define TASK_STACK_SIZE 6000

/*this is the HEAD or ROOT of the task list. it is maintained since the time of initialization the kernel cannot do dynamic memory allocation.*/
LISTNODE sys_Init_TaskList;
LPLISTNODE sys_TaskList = NULL;

UINT32 sys_CurTaskID;
// the following variable is used to find the next task id for the task id allocations
UINT32 sys_NextTaskID = 1;

struct TSS *sys_TSS;
struct TaskInfo sys_KernelTask;
UINT32 sys_IOBasePhysicalAddress;

/*The following function will be called when the system is becoming idle. The hlt instruction makes the CPU to stop working*/
void IdleLoop()
{
    asm volatile("loop: \
                    hlt; \
                    jmp loop");
}
void RunScheduler()
{
    dwTaskSwitchRequest = 1;
    asm volatile("sti; int $32");
}

/* This function Initialize the Task Manager. you must call this function before any TaskManagement Calls*/
void InitTaskManager()
{
    UINT32 i;
    sys_TaskList = NULL;

    // filling kernel task data structure
    sys_KernelTask.TaskID = sys_NextTaskID++;
    sys_KernelTask.m_PageDirectoryAddress = mm_KernelPageDirectory;
    sys_KernelTask.m_PageBitmapAddress = (BYTE *)mm_KernelPageBitmap;
    sys_KernelTask.ThreadQueue = NULL;
    sys_KernelTask.EnvironmentVariables = NULL;
    strcpy(sys_KernelTask.szCurrentDirectory, "C:\\");

    SetEnvironmentVariable(&sys_KernelTask, "PATH", ".");

    sys_Init_TaskList.ptrData = (void *)&sys_KernelTask;
    sys_Init_TaskList.szKey[0] = 0;
    sys_Init_TaskList.Next = NULL;
    sys_TaskList = &sys_Init_TaskList;

    sys_CurTaskID = sys_KernelTask.TaskID;

    // any heap creation should after TaskList creation and TaskId assignment
    sys_KernelTask.lpHeapInfo = HeapCreate(0, INITIAL_KERNEL_HEAP, 0); // create kernel heap

    // creating Task State Segment, it must present on memory at all time
    sys_TSS = (struct TSS *)VirtualAlloc(0, TASK_TSS_PAGES, MEM_RESERVE | MEM_COMMIT, TASK_MEM_PROTECTION); // 1 page for TSS contents and 2 pages for IOBase bitmap (65535 ports)
    if (sys_TSS == 0)
    {
        DEBUG_PRINT_INFO("Unable to allocate memory for System TSS");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return;
    }
    // filling the Kernel TSS with zero
    for (i = 0; i < VASM_PAGE_SIZE / 4; i++)
        ((UINT32 *)sys_TSS)[i] = 0;

    sys_TSS->IOBASE = VASM_PAGE_SIZE; //+32;     //system IO Bitmap base address from sys_TSS base + Interrupt redirection bitmap
    sys_IOBasePhysicalAddress = GetPageEntry((((UINT32)sys_TSS) / VASM_PAGE_SIZE) + 1, VASM_TPT);
    // all Interrupts are redirected to V86
    for (i = 0; i < 8; i++)
        ((UINT32 *)(sys_TSS))[(VASM_PAGE_SIZE / 4) + i] = 0;
    // by default access to all IO ports denied for device drivers and applications
    for (i = 8; i < VASM_PAGE_SIZE / 2; i++)
        ((UINT32 *)(sys_TSS))[(VASM_PAGE_SIZE / 4) + i] = 0xFFFFFFFF; // all are 1s

    sys_TSS->SS0 = KERNEL_DATA_SELECTOR;

    // loading the Kernel Page Directory Address
    sys_TSS->CR3 = ((UINT32)GetPageEntry((UINT32)mm_KernelPageDirectory / VASM_PAGE_SIZE, VASM_TPT) & 0xFFFFF000) | 3;

    // creating Task State Segment Selector
    UINT16 TSSSelector = CreateGDTEntry(sys_TSS, TASK_TSS_PAGES * VASM_PAGE_SIZE, 0x80, 0xE9);
    if (TSSSelector == 0)
    {
        DEBUG_PRINT_INFO("Unable to create system TSS Seletor");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return;
    }
    // just loads the Kernel TSS into memory **ltr instruction
    asm volatile("ltr %0"
                 :
                 : "m"(TSSSelector));

    // create kernel thread
    LPTHREAD lpThread = (LPTHREAD)malloc(sizeof(THREAD));
    if (lpThread == NULL)
    {
        DEBUG_PRINT_INFO("Kernel thread creation failed.");
        // here the error will be set by the CreateThread()
        return;
    }

    sys_CurThread = lpThread;
    lpThread->dwRunState = TASK_RUNNABLE; // ready and not running
    lpThread->dwPriority = TP_NORMAL;
    lpThread->dwWaitingTime = 0;
    lpThread->lpMessageBox = IPC_CreateMessageBox(DEFAULT_THREAD_MESSAGES);
    sysCurThreadStackInfo = &lpThread->pStackInfo;
    sysCurThreadStackInfo->ESP0 = (UINT32)malloc(TASK_STACK_SIZE); // allocate stack
    sysCurThreadStackInfo->ESP1 = 0;
    sysCurThreadStackInfo->ESP2 = 0;
    sysCurThreadStackInfo->SS0 = KERNEL_DATA_SELECTOR;
    sysCurThreadStackInfo->SS1 = 0;
    lpThread->IOBASE = NULL;
    // adding the base thread to the thread queue
    List_Add(&sys_KernelTask.ThreadQueue, "", lpThread);

    // creating Idle thread
    sys_IdleThread = CreateThread(IdleLoop, TASK_STACK_SIZE, KERNEL_CODE_SELECTOR, KERNEL_DATA_SELECTOR, TASK_FLAG_VALUE, TASK_RUNNABLE, TP_IDLE, FALSE, FALSE);

    SetLastError(ERROR_SUCCESS);
}

LPTASK CreateTask(LPCSTR szFilePath, LPCSTR szFileName)
{
    LPTASK lpTask;
    PE_IMAGE_LOAD_INFO PI;
    PI.lpExportDirTableList = NULL;
    if (PE_LoadFile(szFilePath, szFileName, &PI))
    {
        PI.lpLoadedAddress = PI.lpImage;
        INT32 Delta = (UINT32)PI.lpLoadedAddress - (UINT32)PI.lpPreferedLoadAddress;
        PE_RelocateImage(Delta, PI.lpPEFile, PI.lpImage, PI.lpRelocationSection);
        // free(PI.lpPEFile); //free the file -- check this one

        if (PI.AddressOfEntryPoint == 0)
        {
            DEBUG_PRINT_INFO("PI.AddressOfEntryPoint is NULL. ");
            return 0;
        }
        ALLOC_AND_CHECK(lpTask, LPTASK, sizeof(TASK), NULL);

        lpTask->TaskID = sys_NextTaskID++;
        lpTask->dwRunState = TASK_RUNNABLE;
        lpTask->dwPriority = TP_NORMAL;
        lpTask->dwWaitingTime = 0;
#warning check the below assignment
        lpTask->m_PageDirectoryAddress = 0;
        lpTask->m_PageBitmapAddress = 0;
        lpTask->lpHeapInfo = HeapCreate(0, TASK_INITIAL_HEAP_SIZE, TASK_MAX_HEAP_SIZE);
        lpTask->ThreadQueue = NULL;
        lpTask->PEExportList = NULL;
        lpTask->EnvironmentVariables = NULL;

        DEBUG_PRINT_OBJECT1("Creating Thread for PI.AddressOfEntryPoint %X", PI.lpLoadedAddress + PI.AddressOfEntryPoint);
        LPTHREAD lpThread = CreateThread(PI.lpLoadedAddress + PI.AddressOfEntryPoint, TASK_STACK_SIZE, TASK_CODE_SELECTOR, TASK_DATA_SELECTOR, TASK_FLAG_VALUE, TASK_RUNNABLE, TP_NORMAL, FALSE, FALSE);
        if (lpThread == NULL)
        {
            DEBUG_PRINT_INFO("Thread creation failed.");
            return NULL;
        }
        if (List_Add(&lpTask->ThreadQueue, "", lpThread))
        {
            DEBUG_PRINT_OBJECT1("Adding thread to the list %X failed", &lpTask->ThreadQueue);
            return NULL;
        }
        if (List_Add(&sys_TaskList, "", lpTask))
        {
            DEBUG_PRINT_OBJECT1("Adding task to the sys_TaskList %X failed", &sys_TaskList);
        }
        DEBUG_PRINT_OBJECT1("Returns %x", lpTask);
        printf("Returns %x", lpTask);
        return lpTask;
    }
    printf("\nReturns null");
    return NULL;
}

/* This function returns the task id of the currently running process*/
UINT32 GetTaskID()
{
    return sys_CurTaskID;
}

LPTASK GetTaskInfo(UINT32 TaskID)
{
    UINT32 TotTasks = List_Count(sys_TaskList);
    int i;
    for (i = 0; i < TotTasks; i++) // Search througth tasks to get task memory info
    {
        struct TaskInfo *TI = (struct TaskInfo *)List_GetData(sys_TaskList, "", i);

        if (TI != NULL)
            if (TI->TaskID == TaskID)
            {
                SetLastError(ERROR_SUCCESS);
                return TI;
            }
    }
    SetLastError(ERROR_INVALID_HANDLE);
    return NULL;
}

/* This function removes the task from the system task list*/
UINT32 RemoveTask(UINT32 TaskID)
{
    UINT32 TotTasks = List_Count(sys_TaskList);
    int i;
    for (i = 0; i < TotTasks; i++) // Search througth tasks to get task memory info
    {
        struct TaskInfo *TI = (struct TaskInfo *)List_GetData(sys_TaskList, "", i);
        if (TI != NULL)
            if (TI->TaskID == TaskID)
            {
                List_Delete(&sys_TaskList, "", i);
                // clear task, thread, memory everything only after confirming that no thread of that task is in RUNNING state.
                // free( TI );

                SetLastError(ERROR_SUCCESS);
                return 1;
            }
    }
    SetLastError(ERROR_INVALID_HANDLE);
    return 0;
}

void ExitProcess(DWORD dwExitCode)
{
    DEBUG_PRINT_OBJECT2("ExitProcess(%d) - Removing task %d", dwExitCode, GetTaskID());
    RemoveTask(GetTaskID());
    RunScheduler();
    // here we may clear all the objects and resources used by the task
}
