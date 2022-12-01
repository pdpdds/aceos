#ifndef __TIMER_H
#define __TIMER_H
/* 	   Timer and time related functions
		Created by Sam ( 22-Aug-2002)
				samuelhard@yahoo.com
*/
#include <TypeDefs.h>
#include <ObjMan.h>
#ifdef __cplusplus
    extern "C" {
#endif
        
typedef struct __TimerInfo TIMER;
typedef TIMER* LPTIMER;

extern DWORD dwTaskSwitchRequest;

extern struct ListNode * sysTimerQueue;

struct __TimerInfo
{
    LPOBJECT lpObject;
    
    UINT16 wInterval;
    UINT16 wCount;
    BOOLEAN bEnabled;
    //void (*lpCallBackFn)();
    UINT32 dwParameter;
    
    LPTIMER lpNext;
};

BYTE InitTimer(int wHz);

UINT32 GetTimerTick(void);
void MDelay(UINT32 DelayValue);

BYTE GetTimerTickStatus();
BYTE SetTimerTickStatus(BYTE Enabled);

LPTIMER CreateTimer(UINT16 wInterval, UINT32 dwParameter );
UINT32 DropTimer(LPTIMER lpTimer);

void TimerHand();

#ifdef __cplusplus
    }
#endif
#endif
