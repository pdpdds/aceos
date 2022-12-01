/*
                Task / Thread Scheduler
                   Ace OS Source Code
                       Created by Samuel on 16-Aug-2003 10:28pm
				samuelhard@yahoo.com
Credits :
        1) The Scheduling Algorithirm is borrowed from the VISOPSYS.
            Task Priority, Waiting Time, Priority Ratio
*/
#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <TypeDefs.h>
#include <Atomic.h>

#ifdef __cplusplus
    extern "C" {
#endif
extern ATOMIC AtomSync;
        
struct ThreadReg * Scheduler();

#ifdef __cplusplus
    }
#endif

#endif


