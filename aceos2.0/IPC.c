/*                            Inter Process Commmunication - Messaging
                                                        - Sam (04-Sep-2002) samueldotj@gmail.com
                                                Modified on 07-Aug-2003
*/

#define _DEBUG_

#include <TypeDefs.h>
#include <Beep.h>
#include <IPC.h>
#include <SysInfo.h>
#include <Util.h>

/*
This function will create a message box.
Input : wMaxMessages - Maximum messages that can be stored in the message box
Output : MessageBox
*/
LPMESSAGEBOX IPC_CreateMessageBox(UINT16 wMaxMessages)
{
    //creating array of messages
    LPMESSAGE lpMessageArray=(LPMESSAGE)malloc( sizeof(MESSAGE) * wMaxMessages ) ;
    if ( lpMessageArray == NULL )
    {
        SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        return NULL;
    }
    //creating MessageBox;
    LPMESSAGEBOX lpMessBox=(LPMESSAGEBOX)malloc(sizeof(MESSAGEBOX));
    if ( lpMessBox == NULL )
    {
        SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        free(lpMessageArray);
        return NULL;
    }
    lpMessBox->lpMessageArray=lpMessageArray;
    lpMessBox->wFirstMessage=lpMessBox->wTotalMessages=0;
    lpMessBox->wMaxMessages=wMaxMessages;
    
    SetLastError( ERROR_SUCCESS );
    return lpMessBox;
}
/*
This function frees the allocated Message Box
Input : MessageBox
*/
void IPC_DestroyMessageBox(LPMESSAGEBOX lpMessageBox)
{
    if ( lpMessageBox )
    {
        free(lpMessageBox->lpMessageArray);
        free(lpMessageBox);
        SetLastError( ERROR_SUCCESS );
    }
}
UINT16 IPC_PostMessage(LPMESSAGEBOX lpMessageBox, UINT32 dwMessageID, UINT32 dwPara1, UINT32 dwPara2)
{
    UINT16 PostAt;
    if ( lpMessageBox->wTotalMessages > lpMessageBox->wMaxMessages )
    {
        //Beep();
        DEBUG_PRINT_INFO("Overflow - message ignored");
        return 0;
    }
    PostAt = lpMessageBox->wFirstMessage + lpMessageBox->wTotalMessages;
    lpMessageBox->wTotalMessages++;
    lpMessageBox->wTotalMessages=1;
    if ( lpMessageBox->wFirstMessage > lpMessageBox->wMaxMessages )
        lpMessageBox->wFirstMessage=0;
    if ( PostAt > lpMessageBox->wMaxMessages )
        PostAt -= lpMessageBox->wMaxMessages;
    
    lpMessageBox->lpMessageArray[PostAt].dwMessageID = dwMessageID;
    lpMessageBox->lpMessageArray[PostAt].dwPara1 = dwPara1;
    lpMessageBox->lpMessageArray[PostAt].dwPara2 = dwPara2;
    return 1;
}
LPMESSAGE IPC_ReceiveMessage(LPMESSAGEBOX lpMessageBox)
{
    if ( lpMessageBox->wFirstMessage > lpMessageBox->wMaxMessages )
        lpMessageBox->wFirstMessage=0;
    if ( lpMessageBox->wTotalMessages > 0 )
    {
        lpMessageBox->wTotalMessages--;
        SetLastError( ERROR_SUCCESS );
        return &lpMessageBox->lpMessageArray[lpMessageBox->wFirstMessage++];
    }
    SetLastError( ERROR_INVALID_ACCESS );
    return NULL;
}

