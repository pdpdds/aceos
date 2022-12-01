#include <os/kernel32.h>
_exit(int status)
{
	__exit (status);
}
__exit(int status)
{
	ExitProcess(status);
}
