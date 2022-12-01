#ifndef __TASK_MAN_H
#define __TASK_MAN_H
/* 	   Task Management - Ace OS Source Code
		Created by Sam ( 01-Nov-2002)
				samuelhard@yahoo.com
*/
#include <TypeDefs.h>
#include <List.h>
#include <Heap.h>
//#include <GFS.h>
//#include <ObjMan.h>
#ifdef __cplusplus
    extern "C" {
#endif
        
#define TASK_INITIAL_HEAP_SIZE  1024
#define TASK_MAX_HEAP_SIZE      (1024 * 1024)
        

#define TASK_PAGE_BITMAP_SIZE 32         //size of the pagebitmap in pages
            
#define KERNEL_CODE_SELECTOR 0x8
#define KERNEL_DATA_SELECTOR 0x10
        
#define TASK_CODE_SELECTOR 0x19         
#define TASK_DATA_SELECTOR 0x21
        
#define TASK_FLAG_VALUE 0x202 //0x3202 

#define TASK_PRIORITY_LEVELS 7
#define TASK_PRIORITY_RATIO  5          //0 - no priority 
        
#define TP_TIME_CRITICAL 0
#define TP_HIGHEST       1
#define TP_ABOVE_NORMAL  2
#define TP_NORMAL        3
#define TP_BELOW_NORMAL  4
#define TP_LOWEST        5
#define TP_ABOVE_IDLE    6
#define TP_IDLE          7

#define TASK_MEM_PROTECTION     3

#define CUR_DIR_SIZE     500

struct TaskInfo
{
    //LPOBJECT lpObject; 
    
    UINT32 TaskID;
    UINT32 dwRunState;
    UINT32 dwPriority;
    UINT32 dwWaitingTime;       

    void * m_PageDirectoryAddress;
    BYTE * m_PageBitmapAddress;
    
    LPHEAPINFO lpHeapInfo;
    
    LPLISTNODE ThreadQueue;
    
    LPLISTNODE PEExportList;     //the symbols exported by this task
        
    LPLISTNODE EnvironmentVariables;
    
    char szCurrentDirectory[CUR_DIR_SIZE];
}__attribute__ ((packed));

typedef struct TaskInfo TASK;
typedef TASK * LPTASK;

extern LPLISTNODE sys_TaskList;
extern struct TSS * sys_TSS;
extern UINT32 sys_IOBasePhysicalAddress;

extern struct TaskInfo sys_KernelTask;        
//extern UINT32 KernelTaskID;

extern UINT32 sys_CurTaskID;
extern LPTASK sys_CurTaskInfo;

void InitTaskManager();

UINT32 GetTaskID();
LPTASK GetTaskInfo(UINT32 TaskID);
UINT32 RemoveTask(UINT32 TaskID);
LPTASK CreateTask(LPCSTR szFilePath, LPCSTR szFileName);
void InitTaskManager();
void RunScheduler();
void IdleLoop();
void ExitProcess(DWORD dwExitCode);


#ifdef __cplusplus
    }
#endif

#endif
