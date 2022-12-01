#include <os/kernel32.h>
int _open(const char* filename, int oflag)
{
	GFS_CreateFile(filename, oflag, 0, 0, 0);
}