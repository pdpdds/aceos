#include <time.h>
clock_t clock(void)
{
	return (clock_t)GetTimerTick();
}