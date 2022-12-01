//Date 20-July-2002
#ifndef __NUMCONVERT_H
#define __NUMCONVERT_H
#include "TypeDefs.h"
#ifdef __cplusplus
	extern "C" {
#endif

char * UIntToString(unsigned int Value, char * Str, char Base);
char * ULongToString(UINT32 Value, char * Str, char Base);

#ifdef __cplusplus
	}
#endif

#endif
