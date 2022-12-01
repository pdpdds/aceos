#ifndef __THREAD_H
#define __THREAD_H
/* 	   Thread Specific Function Declaration - Ace OS Source Code
		Created by Sam ( 08-Aug-2003)
				samueldotj@gmail.com
*/
#include <TypeDefs.h>
#include <List.h>
#include <Heap.h>
#include <IPC.h>
//#include <ObjMan.h>

#define TASK_RUNNING            1
#define TASK_RUNNABLE           2
#define TASK_INTERRUPTIBLE      4
//#define TASK_UNINTERRUPTIBLE    
#define TASK_STOPPED            8

#ifdef __cplusplus
    extern "C" {
#endif
        
#define DEFAULT_THREAD_MESSAGES 100

/*this enum is used to find the index of a specified register resides in the 
system stack( used by timer ).

This stack setup will do by the CreateThread for the first time
*/
enum STACK_POS
{
    SP_P_SS=1,
    SP_P_ESP,
    SP_EFLAGS,
    SP_CS,
    SP_EIP,
    SP_EAX,
    SP_ECX,
    SP_EDX,
    SP_EBX,
    SP_ESP,
    SP_EBP,
    SP_ESI,
    SP_EDI,
    SP_DS,
    SP_ES,
    SP_FS,
    SP_GS,
};

#define STACK_MIN_SIZE SP_GS

        
/* The objects of the following structures are for system use and it should be inaccessible for all except kernel*/
struct TSS
{
    UINT32 PREV_LINK;
    UINT32 ESP0;
    UINT32 SS0;
    UINT32 ESP1;
    UINT32 SS1;
    UINT32 ESP2;
    UINT32 SS2;
    UINT32 CR3;
    UINT32 EIP;
    UINT32 EFLAGS;
    UINT32 EAX;
    UINT32 ECX;
    UINT32 EDX;
    UINT32 EBX;
    UINT32 ESP;
    UINT32 EBP;
    UINT32 ESI;
    UINT32 EDI;
    UINT32 ES;
    UINT32 CS;
    UINT32 SS;
    UINT32 DS;
    UINT32 FS;
    UINT32 GS;
    UINT32 LDT_SEL;
    UINT16 TRAP;
    UINT16 IOBASE;
}__attribute__ ((packed));
struct ThreadStackInfo
{
    UINT32 SS0, SS1, SS2, SS3;
    UINT32 ESP0, ESP1, ESP2, ESP3;
}__attribute__ ((packed));

struct ThreadInfo
{
//    LPOBJECT lpObject;
    
    BYTE * IOBASE;      
    
    UINT32 * lpStack;
    
    UINT32 dwRunState;
    
    UINT32 dwPriority;
    UINT32 dwWaitingTime;
    struct ThreadStackInfo pStackInfo;
    
    LPMESSAGEBOX lpMessageBox;
    //HMESSBOX hMessageBox;
}__attribute__ ((packed));

typedef struct ThreadInfo THREAD;
typedef THREAD * LPTHREAD;

extern LPTHREAD sys_CurThread;
extern struct ThreadStackInfo * sysCurThreadStackInfo;
extern LPTHREAD sys_IdleThread;    

LPTHREAD GetCurrentThread();
LPTHREAD CreateThread( void(*lpThreadAddress)(),  UINT16 wStackSize, UINT16 wCodeSelector, UINT16 wDataSelector, UINT32 dwEFlag, UINT32 dwRunState, UINT32 dwPriority, BOOLEAN IsV86Thread, BOOLEAN bAddToSysThreadList);
UINT32 RemoveThread(UINT32 TaskID, LPTHREAD lpThread);
LPTHREAD GetThreadInfo(UINT32 TaskID, LPTHREAD lpThread);
#ifdef __cplusplus
    }
#endif

#endif

