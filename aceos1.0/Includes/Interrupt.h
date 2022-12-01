#ifndef __INTERRUPT_H
#define __INTERRUPT_H
/* 	   Timer and time related functions
		Created by Sam ( 22-Aug-2002)
				samuelhard@yahoo.com
*/
#include <TypeDefs.h>
#ifdef __cplusplus
	extern "C" {
#endif

#define DisableNMI() asm("cli")
#define EnableNMI() asm("sti")


#ifdef __cplusplus
	}
#endif

#endif
