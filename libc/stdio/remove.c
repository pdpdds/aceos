#include <stdio.h>
#include <errno.h>

int remove(const char *fn)
{
	errno = ENOTIMPL;
	return -1;
}
