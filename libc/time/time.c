/* $Id: time.c,v 1.1.1.1 2002/12/21 09:50:19 pavlovskii Exp $ */

#include <time.h>
#include <os/Kernel32.h>

time_t time(time_t *t)
{
    /*struct tm _t;
    SYSTEMTIME SystemTime;
    GetLocalTime(&SystemTime);
    if (t)
    {
        _t.tm_sec = SystemTime.wSecond;
        _t.tm_min = SystemTime.wMinute;
        _t.tm_hour = SystemTime.wHour;
        _t.tm_mday = SystemTime.wDay;
        _t.tm_mon = SystemTime.wMonth;
        _t.tm_year = SystemTime.wYear;
        _t.tm_wday = 0;
        _t.tm_yday = 0;
        _t.tm_isdst = 0;
        _t.__tm_zone = 0;
        _t.__tm_gmtoff = 0;
    }
    *t = _t;
    return t;*/
    return t;
}
