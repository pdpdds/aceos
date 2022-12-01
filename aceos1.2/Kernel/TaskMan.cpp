/*
  Name: 		Task Manager
  Author: 			- Samuel
  Description: 	This file contains neccessary routines  to manage tasks
  Date: 28-Oct-02 19:47
*/
#include "TypeDefs.h"
#include "Collect.h"
#include "Processor.h"
#include "MemMan.h"
#include "Heap.h"
#include "TaskMan.h"

#define TRS_NOT_STARTED 0
#define	TRS_RUNNING 1
#define	TRS_PAUSED 2
#define TRS_SLEEPING 3
#define TRS_QUEUED 4

#define TP_LOW 0 
#define TP_NORMAL 16
#define TP_HIGH 32

#define TSS_PREV_LINK 	0
#define TSS_ESP0	4
#define TSS_SS0		8
#define TSS_ESP1	12
#define TSS_SS1		16
#define TSS_ESP2	20
#define TSS_SS2		24
#define TSS_CR3		28
#define TSS_EIP		32
#define TSS_EFLAGS	36
#define TSS_EAX		40
#define TSS_ECX		44
#define TSS_EDX		48
#define TSS_EBX		52
#define TSS_ESP		56
#define TSS_EBP		60
#define TSS_ESI		64
#define TSS_EDI		68
#define TSS_ES		72
#define TSS_CS		76
#define TSS_SS		80
#define TSS_DS		84
#define TSS_FS		88
#define TSS_GS		92
#define TSS_LDT_SEL	96
#define TSS_TRAP	100
#define TSS_IOBASE	102
extern "C" void TEST();
struct TaskInfo
	{
	UINT32 TaskID;
	UINT16 RunState;
	UINT16 Priority;

	UINT32 * m_PageDirectoryAddress;	// you can get this value from TSS also. but this is more convenient
	Collection <UINT32 *> m_PageBitmapAddresses;

	BYTE * p_TSS;		// address of task state segment ( TSS is should be in one page)
	UINT16 p_TaskSelector;	// Task selector
	UINT16 p_TaskGate;	// Task Gate for the Task Selector
	};
Collection <struct TaskInfo *>RunningTasks;
UINT32 CurTaskID;

BYTE * KernelTSS;
UINT16 KernelTSSSelector;
/* This function Initialize the Task Manager.
you must call this function before any TaskManagement Calls*/
void InitTaskManager()
	{UINT32 TSSD_FirstDWORD, TSSD_SecondDWORD;
	RunningTasks.Initialize();
	CurTaskID=1;

	//creating kernel Task State Segment
	KernelTSS = (BYTE *)AllocateKernelPages(1);
	if ( KernelTSS == 0 )
		{Printf("\n\r TASKMAN :: Cannot Allocate Kernel memory for Kernel TSS");
		return;
		}
	//filling the Kernel TSS with zero
	for(int i=0;i<4096;i++)
		KernelTSS[i]=0;

	//loading the Kernel Page Directory Address
	UINT32 *tmpPtr=(UINT32 *)(KernelTSS+TSS_CR3);
	*tmpPtr=mmKernelPDAddress;

	TSSD_FirstDWORD = (UINT32)KernelTSS;	//limit - (104-1) and lower base-0
	TSSD_FirstDWORD <<=16;
	TSSD_FirstDWORD |= 0x0067;
	TSSD_SecondDWORD = (UINT32)KernelTSS;
	TSSD_SecondDWORD &= 0xFF000000;
	TSSD_SecondDWORD |= 0x0090E900;
	TSSD_SecondDWORD |= (BYTE)((UINT32)KernelTSS>>16) ;
	KernelTSSSelector = CreateGDTEntry(TSSD_SecondDWORD,TSSD_FirstDWORD);

	LoadKernelTSS(); //just loads the Kernel TSS into memory **ltr instruction
	}
/*This function creates a page directory and makes the first 256 entries 
in the PD to point the kernel code and data.
*/
void * CreateTaskPageDirectory(struct TaskInfo * NewTask)
	{//creating Page Directory
	UINT32 * PD=(UINT32*)AllocateKernelPages(1);
	if ( PD == 0 )
		{Printf("\n\r TASKMAN :: Allocation failed during Task Page Directory Creation");
		return 0;
		}
	NewTask->m_PageDirectoryAddress[0]= ((UINT32)PD & 0xFFFFF000) | 7 ;
	UINT32 * tmpPtr=(UINT32 *)(NewTask->p_TSS+TSS_CR3);
	*tmpPtr = ( GetRAMHardwareAddress((UINT32*)mmKernelPDAddress,PD) & 0xFFFFF000) | 7;

	//creating Page Table
	UINT32 * PT=(UINT32*)AllocateKernelPages(1);
	if ( PT == 0 )
		{Printf("\n\r TASKMAN :: Allocation failed during Task First Page Table Creation");
		return 0;
		}
	PD[0]=((UINT32) GetRAMHardwareAddress((UINT32*)mmKernelPDAddress,(UINT32*)PT) & 0xFFFFF000) | 7;

	tmpPtr=(UINT32 *)mmKernelPDAddress;
	UINT32 PDE = tmpPtr[0];	// first PDE - PageTable address
	tmpPtr = (UINT32 *)(PDE & 0xFFFFF000);		// extracting Page Table address from the PDE
	//copying Kernel first 256 Page entry
	for(int i=0;i<255;i++)
		PT[i]=tmpPtr[i];

	//testing --- creating 1st Page Table
	UINT32 * UserPage=(UINT32*)AllocateKernelPages(1);
	if ( UserPage == 0 )
		{Printf("\n\r TASKMAN :: Allocation failed during Task User Page Creation");
		return 0;
		}
	UserPage[0]=0xcf;
	UserPage[1]=0x90;
	UserPage[2]=0xcf;
	//point the PT in 256th PDEz	
	PT[255]=((UINT32) GetRAMHardwareAddress((UINT32*)mmKernelPDAddress,(UINT32*)UserPage) & 0xFFFFF000)|7;
	PT[256]=((UINT32) GetRAMHardwareAddress((UINT32*)mmKernelPDAddress,(UINT32*)UserPage) & 0xFFFFF000)|7;
	PT[257]=((UINT32) GetRAMHardwareAddress((UINT32*)mmKernelPDAddress,(UINT32*)UserPage) & 0xFFFFF000)|7;
	}

/*---------This function creates a TaskInfo structure and returns the TaskID to the kernel
It also allocates memory for PD, PT and Code. It sets the page bitmap also. The kernel then
loads the file from the disk to the allocated area.
*/
UINT16 CreateTask(UINT16 Priority, UINT32 CodeSize)
	{struct TaskInfo * NewTask;
	UINT32 TSSD_FirstDWORD, TSSD_SecondDWORD=0;	
	UINT32 * tmpPtr;
 	UINT16 TSSSelector; 
	
	NewTask = (struct TaskInfo *)AllocHeap(sizeof(struct TaskInfo));
	if ( NewTask == 0 )	
		return 0;
	NewTask->RunState=TRS_NOT_STARTED;
	NewTask->Priority=Priority;

	NewTask->m_PageDirectoryAddress = (UINT32*)AllocateKernelPages(1);
	if ( NewTask->m_PageDirectoryAddress == 0 )
		{Printf("\n\r TASKMAN :: Cannot Allocate Kernel memory for PD");
		return 0;
		}
	for(int i=0;i<1024;i++)
		NewTask->m_PageDirectoryAddress[i]=0;	//zeroing all PDE

	NewTask->m_PageBitmapAddresses.Initialize();
	UINT32 *tmpAddress = (UINT32*)AllocateKernelPages(1);
	if ( tmpAddress == 0 )
		{Printf("\n\r TASKMAN :: Cannot Allocate Kernel memory for Page Bitmap");
		return 0;
		}
	for(int i=0;i<1024;i++)
		tmpAddress[i]=0xFFFFFFFF;	//all Pages are free
	NewTask->m_PageBitmapAddresses.Add( tmpAddress );

	NewTask->p_TSS = (BYTE *)AllocateKernelPages(1);
	if ( NewTask->p_TSS == 0 )
		{Printf("\n\r TASKMAN :: Cannot Allocate Kernel memory for TSS");
		return 0;
		}
	for(int i=0;i<4096;i++)
		NewTask->p_TSS[i]=0;
	
	//creating Data segement. it also used for stack segment
	TSSD_FirstDWORD = 0x0000FFFF;	//limit - 0xFFFF and lower base-0
	TSSD_SecondDWORD = 0xDFF300;
	TSSSelector = CreateGDTEntry(TSSD_SecondDWORD,TSSD_FirstDWORD);
	if ( TSSSelector == 0 )
		{Printf("\n\r TASKMAN :: CreateGDTEntry() failed during data segment creation");
		return 0;
		}
	NewTask->p_TSS[TSS_PREV_LINK]=KernelTSSSelector;
	NewTask->p_TSS[TSS_SS0]=0x18;
	tmpPtr=(UINT32 *)(NewTask->p_TSS+TSS_ESP1);
	*tmpPtr=0x17000;
	tmpPtr=(UINT32 *)(NewTask->p_TSS+TSS_ESP0);
	*tmpPtr=0xA000;
	NewTask->p_TSS[TSS_SS1]=TSSSelector|3;
	tmpPtr=(UINT32 *)(NewTask->p_TSS+TSS_ESP2);
	*tmpPtr=0xA000;
	NewTask->p_TSS[TSS_SS2]=TSSSelector|3;
	NewTask->p_TSS[TSS_EAX]=NewTask->p_TSS[TSS_ECX]=NewTask->p_TSS[TSS_EDX]=NewTask->p_TSS[TSS_EBX]=\
 	NewTask->p_TSS[TSS_EBP]=NewTask->p_TSS[TSS_ESI]=NewTask->p_TSS[TSS_EDI]=0;
	
	NewTask->p_TSS[TSS_SS]=TSSSelector|3;
	tmpPtr=(UINT32 *)(NewTask->p_TSS+TSS_ESP);
	*tmpPtr=0xA000;
	
	NewTask->p_TSS[TSS_ES]=NewTask->p_TSS[TSS_DS]=NewTask->p_TSS[TSS_GS]=NewTask->p_TSS[TSS_FS]=TSSSelector;
	NewTask->p_TSS[TSS_LDT_SEL]=0;
	NewTask->p_TSS[TSS_TRAP]=NewTask->p_TSS[TSS_IOBASE]=0;
	// -----------creating code segment
	TSSD_FirstDWORD = 0x0000FFFF;	//limit - 0xFFFF and lower base-0
	TSSD_SecondDWORD =  0xCFFA00; 
	TSSSelector = CreateGDTEntry(TSSD_SecondDWORD,TSSD_FirstDWORD);
	if ( TSSSelector == 0 )
		{Printf("\n\r TASKMAN :: CreateGDTEntry() failed during code segment creation");
		return 0;
		}
	NewTask->p_TSS[TSS_CS]=TSSSelector|3;
	tmpPtr=(UINT32 *)(NewTask->p_TSS+TSS_EIP);
	*tmpPtr=4096*256;

	tmpPtr=(UINT32 *)(NewTask->p_TSS+TSS_EFLAGS);
	*tmpPtr=0x202L;

	//----------creating Task State Segment descriptor  ; this descriptor is based on the kernels page directory
	TSSD_FirstDWORD = (UINT32)NewTask->p_TSS;	
	TSSD_FirstDWORD <<= 16;
	TSSD_FirstDWORD |= 0x0067;
	TSSD_SecondDWORD = (UINT32)NewTask->p_TSS;
	TSSD_SecondDWORD &= 0xFF000000;
	TSSD_SecondDWORD |= 0x0090E900;
	TSSD_SecondDWORD |= (BYTE)((UINT32)NewTask->p_TSS>>16) ;
	TSSSelector = CreateGDTEntry(TSSD_SecondDWORD,TSSD_FirstDWORD);
	if ( TSSSelector == 0 )
		{Printf("\n\r TASKMAN :: CreateGDTEntry() failed during TSS Descriptor creation");
		return 0;
		}
	NewTask->p_TaskSelector = TSSSelector|3;
	
	RunningTasks.Add(NewTask);
	NewTask->TaskID=++CurTaskID;
	//------------------Creating Task Gate
	TSSD_FirstDWORD = NewTask->p_TaskSelector;
	TSSD_FirstDWORD <<=16;
	TSSD_SecondDWORD = 0x0000E500;

	TSSSelector = CreateGDTEntry(TSSD_SecondDWORD,TSSD_FirstDWORD);
	if ( TSSSelector == 0 )
		{Printf("\n\r TASKMAN :: CreateGDTEntry() failed during TSS Descriptor creation");
		return 0;
		}
	NewTask->p_TaskGate = TSSSelector|3;
	CreateTaskPageDirectory(NewTask);

	return NewTask->TaskID;
	}
UINT16 GetTaskSelector(UINT32 TaskID)
	{UINT32 TotTasks=RunningTasks.Count();
	for(UINT32 i=0;i<TotTasks;i++)
		if( RunningTasks.Item(i)->TaskID == TaskID )
			return RunningTasks.Item(i)->p_TaskSelector;
	return 0;
	}
BYTE * GetTaskTSS(UINT32 TaskID)
	{UINT32 TotTasks=RunningTasks.Count();
	for(UINT32 i=0;i<TotTasks;i++)
		if( RunningTasks.Item(i)->TaskID == TaskID )
			return (BYTE *)RunningTasks.Item(i)->p_TSS;
	return 0;
	}
