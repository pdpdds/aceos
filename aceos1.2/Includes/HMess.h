/*		Hardware Messaging functions for Ace OS
				- Sam (samuelhard@yahoo.com)
				Date 05-Sep-2002*/
#ifndef __HMESS_H
#define __HMESS_H
#include "TypeDefs.h"
#ifdef __cplusplus
	extern "C" {
#endif

struct HMessage
	{BYTE HardwareID;
	UINT16 SubSystem;	
	UINT32 Para1;
	UINT32 Para2;
	BYTE Reserved[4];
	};
BYTE PutHardwareMessage(BYTE HardwareID, UINT16 SubSystem, UINT32 Para1, UINT32 Para2 );
struct HMessage * GetNextHardwareMessage(void);
#ifdef __cplusplus
	}
#endif

#endif
