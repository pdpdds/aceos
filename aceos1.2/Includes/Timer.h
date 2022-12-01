#ifndef __TIMER_H
#define __TIMER_H
/* 	   Timer and time related functions
		Created by Sam ( 22-Aug-2002)
				samuelhard@yahoo.com
*/
#include <TypeDefs.h>
#ifdef __cplusplus
	extern "C" {
#endif
BYTE InitTimer();
UINT32 GetTimerTick(void);
void MDelay(UINT32 DelayValue);
BYTE GetTimerTickStatus();
BYTE SetTimerTickStatus(BYTE Enabled);
UINT32 CreateTimer(UINT16 Interval, void (*CallBackFn)());
void DropTimer(UINT32 TimerID);
void ChangeTimerInfo(UINT32 TimerID, UINT16 Interval, void (*CallBackFn)(), BOOLEAN Enabled);
#ifdef __cplusplus
	}
#endif
#endif
