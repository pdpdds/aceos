/*
	Real Time Clock (Motorola MC146818)

	This program is writen by Sam on 4-Feb-2002
	You can get the latest version from web
			www.geocities.com/samuelhard
			samuelhard@yahoo.com

	Note :
		When intializing the kernel you must set 0x6 to the Status B
		register so that the RTC will not generate any interrupt and
		operate on Binary data mode and 24hrs settings.
*/
#ifndef __RTC__H
#define __RTC__H

#include <TypeDefs.h>
#include <IO.h>

#ifdef __cplusplus
	extern "C" {
#endif
BYTE GetSystemDate(BYTE * Day, BYTE * Month, BYTE * Year);
BYTE SetSystemDate(BYTE Day, BYTE Month, BYTE Year);

BYTE GetSystemTime(BYTE * Second, BYTE * Minute, BYTE * Hour);
BYTE SetSystemTime(BYTE Second, BYTE Minute, BYTE Hour);
#ifdef __cplusplus
	}
#endif

#endif
