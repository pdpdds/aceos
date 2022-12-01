#ifndef __PRINTF_H
#define __PRINTF_H

#include <TypeDefs.h>
#ifdef __cplusplus
	extern "C" {
#endif

    int printf(const char *fmt, ...);
    int dbgprintf(const char *fmt, ...);
#ifdef __cplusplus
	}
#endif
#endif
