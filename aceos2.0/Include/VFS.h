#ifndef __VFS_H
#define __VFS_H
/* 	   Installable File System for Ace OS
		Created by Sam ( 06-June-2003)
				samueldotj@gmail.com
*/
#include <TypeDefs.h>
#include <GFS.h>
#include <SysInfo.h>

#define STDIN  (HANDLE)1
#define STDOUT (HANDLE)2
#define STDERR (HANDLE)3

#ifdef __cplusplus
extern "C" {
#endif

HANDLE GetStdHandle(DWORD STDHANDLE);

HANDLE VFS_CreateFile(LPFSHANDLE lpFS, LPCTSTR lpFilePath,DWORD dwOpenMode, DWORD dwShareMode, DWORD dwCreationDisposition,DWORD dwFlags );
UINT32 VFS_ReadFile(LPFSHANDLE lpFS, HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer);
UINT32 VFS_WriteFile(LPFSHANDLE lpFS, HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer);
    
int VFS_Init();   

#ifdef __cplusplus
}
#endif
#endif
