#include <stdio.h>
#include <errno.h>

int rename(const char *old, const char *new)
{
	errno = ENOTIMPL;
	return -1;
}