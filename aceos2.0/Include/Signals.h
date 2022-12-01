#ifndef __SIGNALS_H
#define __SIGNALS_H
/* 	   
    SIGNAL Definition
		Created by Sam ( 07-Oct-2004)
				samueldotj@yahoo.co.in
*/
#include <TypeDefs.h>
#ifdef __cplusplus
    extern "C" {
#endif

#define SIG_TIMER       1
#define SIG_ABORT       2
#define SIG_DMA         4
#define SIG_READ        8
#define SIG_WRITE       16
#define SIG_OPEN        32
#define SIG_CLOSE       64
#define SIG_DELETE      128
        
#ifdef __cplusplus
    }
#endif
#endif
    
