/*              Object Manager
                            - Ace OS Source Code 
        Created on 1-Oct-2004 by Samuel(samueldotj@yahoo.co.in)
*/
#include <TypeDefs.h>
#include <ObjMan.h>
#include <SysInfo.h>
#include <IPC.h>
#include <Thread.h>
#include <string.h>
#include <Timer.h>
#include <Scheduler.h>

struct ListNode * sysObjects = NULL;
struct ListNode * sysWaitForObjects = NULL;
LPOBJECT CreateObject(char * szName, PVOID lpData)
{
    LPOBJECT lpObj;
    LPMESSAGEBOX lpMessageBox;
    lpObj = (LPOBJECT) malloc( sizeof( OBJECT ) );
    
    if ( lpObj == NULL )
    {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return NULL;
    }
    /*#warning "Remove this comment"
    lpMessageBox = IPC_CreateMessageBox(OBJECT_MESSAGE_BOX_SIZE);
    if ( lpMessageBox == NULL )
    {
        //dont set error code here - it should be set in IPC_CreateMessageBox()
        free(lpObj);    //clean-up
        return NULL;
    }*/
    strncpy((char *)lpObj->szName, szName, 19);
    lpObj->szName[19] = 0;                //terminate the string -- this logic would not work if the string is smaller than 20
    lpObj->dwUses = 1;                    //first use
    lpObj->lpTask = (LPTASK) GetTaskID();           //Current Task
    lpObj->lpThread= GetCurrentThread();       //Current Thread
    lpObj->dwSignals = 0;                 //no signal received
    lpObj->lpIData = lpData;              //associate custom data
    
    //lpObj->lpMessageBox = lpMessageBox;   
    
    List_Add( &sysObjects, "", lpObj ); //add it to the sys objects
    
    SetLastError( ERROR_SUCCESS );
    return lpObj;
}
//why this function is created ?
PVOID GetObjectData(LPOBJECT lpObj)
{
    if ( lpObj )
    {
        SetLastError( ERROR_SUCCESS );
        return lpObj->lpIData;
    }
    SetLastError( ERROR_INVALID_PARAMETER );
    return NULL;
}
UINT32 DeleteObject(LPOBJECT lpObj)
{
    //SetLastError(ERROR_NOT_SUPPORTED);
    LPLISTNODE tmpNode;
    int i=0;
    FOREACH(tmpNode, sysObjects) 
    {
        i++;
        if ( tmpNode->ptrData == lpObj )
        {
            List_Delete( &sysObjects, "", i);
            free( lpObj );
            SetLastError( ERROR_SUCCESS );
            return 0;
        }
    }
    SetLastError( ERROR_INVALID_PARAMETER );
    return 1;
}
/* Add the object to WaitObject queue and set the thread state to interruptable and halt the thread*/
UINT32 WaitForSingleObject(LPOBJECT lpObj)
{
    LPWAITFOROBJECT lpWObj;
    
    lpWObj = (LPWAITFOROBJECT) malloc( sizeof( WAITFOROBJECT ) );
    if ( lpWObj )
    {
        lpWObj->lpWaitForObject = lpObj;
        lpWObj->lpTask = (LPTASK)GetTaskID();
        lpWObj->lpThread = GetCurrentThread();
        
        List_Add( &sysWaitForObjects, "", lpWObj );
        
        #warning "Here synchronizaation problem may arise here so add a spinlock here"
        ((LPTHREAD)GetCurrentThread())->dwRunState = TASK_INTERRUPTIBLE;  //set Task's state to not running but interruptable

        RunScheduler();
        
        SetLastError(ERROR_SUCCESS);
        
        return 0;
    }
    
    SetLastError(ERROR_INVALID_PARAMETER);
    return 1;
}
/*Here post the message to the appropriate thread so that they can wake up on the next scheduling,
*/
void HandleObjects()
{
    LPWAITFOROBJECT lpWObj;
    WORD wCount;
    int i;
    wCount=List_Count(sysWaitForObjects);
    for(i=0;i<wCount;i++)
    {
        lpWObj=(LPWAITFOROBJECT)List_GetData( sysWaitForObjects, "", i);
        if ( lpWObj )
            if ( lpWObj->lpWaitForObject->dwSignals ) //signaled?
            {
                if ( !(lpWObj->lpThread->dwRunState & TASK_STOPPED) )
                {
                    lpWObj->lpThread->dwRunState |= TASK_RUNNABLE;        //ready and not running
                    //#warning The objects must be deleted immediately 
                    List_Delete( &sysWaitForObjects, "" , i );
                    free( lpWObj );                    
                }
            }
    }

}
