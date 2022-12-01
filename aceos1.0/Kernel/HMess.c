/*                            Hardware Messaging
                                                        - Sam (04-Sep-2002) samuelhard@yahoo.com
This file provide the functions needed by device drivers(Hardware interrupt handlers) to post
message to the Kernel.
*/
#include <TypeDefs.h>
#include <Beep.h>
#include <HMess.h>

#define MAX_MESS 100

BYTE TotalMessages=0;
BYTE FirstMessage=0;
struct HMessage HMails[MAX_MESS];
/*This function will post the given message in the hardware and returns 1 on sucess 
otherwise it returns 0
Device drivers uses this function to communicate with kernel. Kernel may post some
messages to the other applications.*/
BYTE PutHardwareMessage(BYTE HardwareID, UINT16 SubSystem, UINT32 Para1, UINT32 Para2 )
	{BYTE PostAt;
	if ( TotalMessages > MAX_MESS )
		{Beep();
		return 0;
		}
	PostAt = FirstMessage + TotalMessages;
	TotalMessages++;
	if ( FirstMessage > MAX_MESS )
		FirstMessage=0;
	if ( PostAt > MAX_MESS )
		PostAt -= MAX_MESS;
	HMails[PostAt].HardwareID = HardwareID;
	HMails[PostAt].SubSystem = SubSystem;
	HMails[PostAt].Para1 = Para1;
	HMails[PostAt].Para2 = Para2;
	return 1;
	}
/*The kernel uses this function read the hardware messages. The kernel
can sequentialy serve the post messages */
struct HMessage * GetNextHardwareMessage(void)
	{if ( TotalMessages > 0 )
		{TotalMessages--;
		return &HMails[FirstMessage++];
		}
	return 0;
	}
