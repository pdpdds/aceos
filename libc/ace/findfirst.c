#include <os/kernel32.h>
#include <dir.h>
int find_first_result=0;
int findfirst(const char *pathname, struct ffblk *ffblk, int attrib)
{
	FILEINFO finfo;
	UINT32 result = GFS_FindFirstFile(pathname, &finfo);
	if ( result )
	{
		find_first_result = result;
		
		ffblk->ff_fsize = finfo.nFileSizeLow;
		strcpy(ffblk->ff_name, finfo.cAlternateFileName);
		ffblk->ff_ftime = finfo.ftLastAccessTime.dwLowDateTime;
		ffblk->ff_fdate = finfo.ftLastAccessTime.dwHighDateTime;
		
		return 0;
	}
	else
		return -1;
}