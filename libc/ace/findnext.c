#include <os/kernel32.h>
#include <dir.h>
extern int find_first_result;
int findnext(struct ffblk *ffblk)
{
	FILEINFO finfo;
	UINT32 result = GFS_FindNextFile(find_first_result, &finfo);
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