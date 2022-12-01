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
#include <Interrupt.h>
//RTC Command registers
#define RTC_INDEX_REG 0x70
#define RTC_VALUE_REG 0x71

//RTC Data register index
#define RTC_SECOND 0
#define RTC_MINUTE 2
#define RTC_HOUR   4

#define RTC_DAY_OF_WEEK  6

#define RTC_DAY   7
#define RTC_MONTH 8
#define RTC_YEAR  9

#define RTC_STATUS_A 0x0A
#define RTC_STATUS_B 0x0B
#define RTC_STATUS_C 0x0C
#define RTC_STATUS_D 0x0D



/* This function will return the current system date in the passed
   pointer parameters. It will return DayOfWeek on success, 0 on failue.

   Note : The year is only 2 digits and the RTC is Real Time Clock
   and donot confuse it with the Y2K problem.

   The Day of week problem is still unsolved
*/
BYTE GetSystemDate(BYTE * Day, BYTE * Month, BYTE * Year)
	{
	/* Checking whether we can read the time now or not
	according to some documentation the MSB in Status A will
	remain 1 (invalid time) only a millisecond*/
	OutPortByte(RTC_INDEX_REG,RTC_STATUS_B);
	OutPortByte(RTC_VALUE_REG,4);


	OutPortByte(RTC_INDEX_REG,RTC_STATUS_A);
	while ( InPortByte(RTC_VALUE_REG) & 0x80 );

	DisableNMI();

	OutPortByte(RTC_INDEX_REG,RTC_DAY);
	*Day=InPortByte(RTC_VALUE_REG);
	OutPortByte(RTC_INDEX_REG,RTC_MONTH);
	*Month=InPortByte(RTC_VALUE_REG);
	OutPortByte(RTC_INDEX_REG,RTC_YEAR);
	*Year=InPortByte(RTC_VALUE_REG);

	OutPortByte(RTC_INDEX_REG,RTC_DAY_OF_WEEK);
	BYTE DOW=InPortByte(RTC_VALUE_REG);

	EnableNMI();
	return DOW;
	}
/* Returns 1 on success and 0 on failue */
BYTE SetSystemDate(BYTE Day, BYTE Month, BYTE Year)
	{
	/* Checking whether we can read the time now or not
	according to some documentation the MSB in Status A will
	remain 1 (invalid time) only a millisecond*/

	OutPortByte(RTC_INDEX_REG,RTC_STATUS_A);
	while ( InPortByte(RTC_VALUE_REG) & 0x80 );

	DisableNMI();

	OutPortByte(RTC_INDEX_REG,RTC_DAY);
	OutPortByte(RTC_VALUE_REG,Day);
	OutPortByte(RTC_INDEX_REG,RTC_MONTH);
	OutPortByte(RTC_VALUE_REG,Month);
	OutPortByte(RTC_INDEX_REG,RTC_YEAR);
	OutPortByte(RTC_VALUE_REG,Year);

	EnableNMI();
	return 1;
	}


/* This function will return the current system time in the passed
   pointer parameters. It will return 1 on success, 0 on failue.

   Note : - The following two functions take the arguments as
   second , minute, hour which is not common for human writings
   so take care when passing and getting.

   returns some related value but not exact what we expect
*/

BYTE GetSystemTime(BYTE * Second, BYTE * Minute, BYTE * Hour)
	{
	//checking read access
	OutPortByte(RTC_INDEX_REG,RTC_STATUS_A);
	if ( InPortByte(RTC_VALUE_REG) & 0x80 )
		return 0;

	OutPortByte(RTC_INDEX_REG,RTC_STATUS_B);
	OutPortByte(RTC_VALUE_REG,6);

	OutPortByte(RTC_INDEX_REG,RTC_SECOND);
	*Second=InPortByte(RTC_VALUE_REG);
	OutPortByte(RTC_INDEX_REG,RTC_MINUTE);
	*Minute=InPortByte(RTC_VALUE_REG);
	OutPortByte(RTC_INDEX_REG,RTC_HOUR);
	*Hour=InPortByte(RTC_VALUE_REG);

	OutPortByte(RTC_INDEX_REG,RTC_STATUS_B);
	OutPortByte(RTC_VALUE_REG,2);

	EnableNMI();
	return 1;
	}

BYTE SetSystemTime(BYTE Second, BYTE Minute, BYTE Hour)
	{
	//checking read access
	OutPortByte(RTC_INDEX_REG,RTC_STATUS_A);
	if ( InPortByte(RTC_VALUE_REG) & 0x80 )
		return 0;

	OutPortByte(RTC_INDEX_REG,RTC_SECOND);
	OutPortByte(RTC_VALUE_REG,Second);
	OutPortByte(RTC_INDEX_REG,RTC_MINUTE);
	OutPortByte(RTC_VALUE_REG,Minute);
	OutPortByte(RTC_INDEX_REG,RTC_HOUR);
	OutPortByte(RTC_VALUE_REG,Hour);

	EnableNMI();
	return 1;
	}

#endif
