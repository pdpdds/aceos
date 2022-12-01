/*		Messaging Subsystem 
                Ace OS Source Code
				- Sam (samuelhard@yahoo.com)
				Date 05-Sep-2002
                                Modified 07-Aug-2003
*/
#ifndef __IPC_H
#define __IPC_H
#include "TypeDefs.h"
#ifdef __cplusplus
    extern "C" {
#endif

typedef struct Message MESSAGE;
typedef MESSAGE * LPMESSAGE;

typedef struct MessageBox MESSAGEBOX;
typedef MESSAGEBOX * LPMESSAGEBOX;

struct Message
{
    UINT32 dwMessageID;		//Message ID
    UINT32 dwPara1;		//parameter 1
    UINT32 dwPara2;		//parameter 2
};

struct MessageBox
{
    UINT16 wMaxMessages;	//capacity of the message box
    UINT16 wTotalMessages;	//total messages currently posted in the message box
    UINT16 wFirstMessage;	//index of the first message 
    LPMESSAGE lpMessageArray;   //this is actually an array of message not linked list
};
   
LPMESSAGEBOX IPC_CreateMessageBox(UINT16 wMaxMessages);
void IPC_DestroyMessageBox(LPMESSAGEBOX lpMessageBox);
UINT16 IPC_PostMessage(LPMESSAGEBOX lpMessageBox, UINT32 dwMessageID, UINT32 dwPara1, UINT32 dwPara2);
LPMESSAGE IPC_ReceiveMessage(LPMESSAGEBOX lpMessageBox);

#ifdef __cplusplus
	}
#endif

#endif
