#include <os/kernel32.h>
int
_creatnew(const char* filename, int attrib, int flags)
{
	return GFS_CreateFile(filename, CREATE_NEW, FILE_SHARE_WRITE, 0, attrib);
}