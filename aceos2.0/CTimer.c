/*              Timer Routines
                      Developed by Sam(samueldotj@gmail.com) on 19-June-2002
                      
*/
#include <TypeDefs.h>
#include <Memory.h>
#include <malloc.h>
#include <IO.h>
#include <Processor.h>
#include <DeviceDriver.h>
#include <Timer.h>
#include <IPC.h>
#include <Signals.h>
#include <Messages.h>
#include <SysInfo.h>
#include <Util.h>

#define MAX_TIMER_MESSAGES 50

#define TMR_CONTROL  0x43
#define TMR_COUNTER0 0x40
#define TMR_COUNTER1 0x41
#define TMR_COUNTER2 0x42

LPMESSAGEBOX sysTimerMB;
UINT32 dwTimerInterval=0;
UINT32 dwTimerValue=0;
UINT32 dwTaskSwitchRequest=0;

#define _DEBUG_

void TimerInterruptHandler();
void TimerHand();

LPLISTNODE sysTimerQueue = NULL;

/*This function will return the total number of ticks since the OS Boot*/
UINT32 GetTimerTick()
{
    return dwTimerValue;
}
/*This function will waits for the given number of ticks and returns*/
void MDelay(UINT32 dwDelay)
{
    UINT32 dwTValue = dwTimerValue;
    while( dwDelay )
    {
        if ( dwTValue != dwTimerValue )
        {
            dwDelay--;
            dwTValue = dwTimerValue;
        }
    }
}

/*Initialize the timer to given Hz speed*/
BYTE InitTimer(int wHz)
{
    unsigned short foo = (3579545L / 3) / wHz;

    /* Reprogram the 8253 timer chip to run at 'HZ', instead of the default 18 Hz */
    OutPortByte(TMR_CONTROL, 0x36);             /* channel 0, LSB/MSB, mode 3, binary */
    OutPortByte(TMR_COUNTER0, foo & 0xFF);      /* LSB */
    OutPortByte(TMR_COUNTER0, foo >> 8);        /* MSB */
	
    SetIDTGate(32, 0x8, TimerInterruptHandler); //set interrupt handler
    
    return 1;
}
void TimerHand()
{
    LPTIMER lpTimer;
    struct ListNode * tmpListNode;
    //enumerate through each node
    FOREACH( tmpListNode, sysTimerQueue )
    {
        lpTimer = tmpListNode->ptrData;
        if ( lpTimer->bEnabled )
        {
            lpTimer->wCount++;		//increase the count
            if ( lpTimer->wCount >= lpTimer->wInterval )// if interval reached call the function
            {
                lpTimer->wCount=0;
                //here you should post message and set signal state - should not call back
                //lpTimer->CallBackFn();
                DEBUG_PRINT_OBJECT2("Signaling timer %d %d",lpTimer->wCount, lpTimer->wInterval);
                lpTimer->lpObject->dwSignals |= SIG_TIMER;
                IPC_PostMessage( lpTimer->lpObject->lpThread->lpMessageBox, MESS_TIMER, lpTimer, lpTimer->dwParameter );
            }
        }
    }
}
LPTIMER CreateTimer(UINT16 wInterval, UINT32 dwParameter )
{
    LPTIMER lpTimer;
    ALLOC_AND_CHECK(lpTimer, LPTIMER, sizeof(TIMER), NULL);
    
    lpTimer->lpObject = CreateObject("Timer", lpTimer);
    if ( lpTimer->lpObject == NULL ) //allocation failed
    {
        //here the lasterror will be set by CreateObject()
        free( lpTimer );
        DEBUG_PRINT_INFO("Timer object creation failed.");
        return NULL;
    }
    
    lpTimer->wCount = 0;
    lpTimer->wInterval = wInterval;
    //lpTimer->lpCallBackFn = lpCallBackFn;
    lpTimer->dwParameter = dwParameter;
    lpTimer->bEnabled = 1;		
    lpTimer->lpNext=NULL;
    
    if ( List_Add( &sysTimerQueue, "", lpTimer ) )
    {
        //here the lasterror should be set by the List_Add()
        free(lpTimer->lpObject);
        free(lpTimer);
        DEBUG_PRINT_INFO("List add failed");
        return NULL;
    }
    
    SetLastError( ERROR_SUCCESS );
    return lpTimer;
}
UINT32 DropTimer(LPTIMER lpTimer)
{   
    LPLISTNODE tmpNode;
    int i=0;
    FOREACH(tmpNode, sysTimerQueue)     
    {
        if ( tmpNode->ptrData == lpTimer )
        {
            List_Delete( &sysTimerQueue, "", i);
            free( lpTimer );
            SetLastError( ERROR_SUCCESS );
            return 0;
        }
    }
    SetLastError( ERROR_INVALID_PARAMETER );
    return 1;
}
