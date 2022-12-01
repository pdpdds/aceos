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

void InitTimer();

UINT32 GetTimerTick(void);
void MDelay(UINT32 DelayValue);


#ifdef __cplusplus
	}
#endif
#endif
