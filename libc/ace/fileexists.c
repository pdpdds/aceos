#include <os/kernel32.h>
int __file_exists(const char *fn)
{
	FILEINFO FINFO;
	return GFS_GetFileAttributes(fn, &FINFO);
}