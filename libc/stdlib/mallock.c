/* $Id$ */

#include <malloc.h>
#include <os/Kernel32.h>

//static lmutex_t lmux_malloc;

#warning ****check this
int __malloc_lock(int lock_unlock, unsigned long *lock)
{
	if (lock_unlock)
		*lock = 1 ;
	else
		*lock = 0;
	return 0;
}
