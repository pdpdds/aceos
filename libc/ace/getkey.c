#include <os/kernel32.h>
int getkey()
{
	return KB_GetLastKey();
}