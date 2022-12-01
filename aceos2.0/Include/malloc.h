/*---------------------Ace OS source Code-----------------------
		Created by Sam on 08-Jun-2002(samuelhard@yahoo.com)
 --------------------------------------------------------------*/
#ifndef MALLOC_H
#define MALLOC_H

#include "TypeDefs.h"
#ifdef __cplusplus
	extern "C" {
#endif

void * malloc(size_t NoOfBytes);
BYTE free(void * Address);

#ifdef __cplusplus
	}
#endif

#endif
