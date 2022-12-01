#include <libc/stubs.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <io.h>
#include <sys/fsext.h>


int
_read(int handle, void* buffer, size_t count)
{
	return GFS_ReadFile( handle, count, buffer );
}
