#ifndef __TASK_MAN_H
#define __TASK_MAN_H
/* 	   Task Management - Ace OS Source Code
		Created by Sam ( 01-Nov-2002)
				samuelhard@yahoo.com
*/
#include <TypeDefs.h>
UINT16 GetTaskSelector(UINT32 TaskID);
BYTE * GetTaskTSS(UINT32 TaskID);
UINT16 CreateTask(UINT16 Priority, UINT32 CodeSize);
void InitTaskManager();
extern "C" void JumpToTask(UINT16 SegSelctor);
extern "C" void LoadKernelTSS();
#endif
