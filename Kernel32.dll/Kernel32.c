/*
  Name: 		Kernel Function Interface
  Author: 			- Samuel
  Created Date: 31-May-2004 10:19PM
*/
#include <SysCall.h>
#include <Kernel32.h>

DLLEXPORT UINT32 DllMain()
{
    //todo some initialization here
}
DLLEXPORT UINT32 GetLastError()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GETLASTERROR),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 SetLastError(UINT32 dwErrorCode)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(SETLASTERROR),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GetSystemInfo(struct _SYSTEM_INFO * SysInfo)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GETSYSTEMINFO),"i"(SYSCALL_INT));
}
DLLEXPORT void GetLocalTime(SYSTEMTIME * lpSystemTime)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GETLOCALTIME),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE SetLocalTime(SYSTEMTIME * lpSystemTime)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(SETLOCALTIME),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GetTimerTick(void)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GETTIMERTICK),"i"(SYSCALL_INT));
}
DLLEXPORT void Sound(UINT16 Frequency)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(SOUND),"i"(SYSCALL_INT));
}
DLLEXPORT void NoSound()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(NOSOUND),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 VirtualAlloc(UINT32 StartingAddress, UINT32 Size, UINT16 AllocationType, UINT16 ProtectionType )
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VIRTUALALLOC),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 VirtualFree(UINT32 StartingAddress, UINT32 Size, DWORD FreeType)
{    
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VIRTUALFREE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 VASM_GetPageSize()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
            int %1"::"i"(VASM_GETPAGESIZE),"i"(SYSCALL_INT));
}
DLLEXPORT LPHEAPINFO HeapCreate(UINT32 dwOptions, UINT32 dwInitialSize, UINT32 dwMaximumSize)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(HEAPCREATE),"i"(SYSCALL_INT));
}
DLLEXPORT void * HeapAlloc(LPHEAPINFO hHeapInfo, UINT32 dwSize)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(HEAPALLOC),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE HeapFree(LPHEAPINFO hHeapInfo, void * lpAddress)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(HEAPFREE),"i"(SYSCALL_INT));
}
DLLEXPORT void * GlobalAlloc(UINT32 dwSize)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GLOBALALLOC),"i"(SYSCALL_INT));
}
DLLEXPORT void * GlobalRealloc(void * lpAddress, UINT32 dwSize)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GLOBALREALLOC),"i"(SYSCALL_INT));
}
DLLEXPORT void * GlobalFree(void * lpAddress)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GLOBALFREE),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE GFS_GetRegisteredFSTypes(BYTE (*fnFeedBack)(const BYTE bFSType,const char * szDescription))
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_GETREGISTEREDFSTYPES),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_GetVolumeCapacity(LPCSTR szVolumePath)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_GETVOLUMECAPACITY),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_GetVolumeFree (LPCSTR szVolumePath)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_GETVOLUMEFREE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_GetVolumeBad  (LPCSTR szVolumePath)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_GETVOLUMEBAD),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_CreateDirectory(LPCSTR lpPath)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_CREATEDIRECTORY),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_RemoveDirectory(LPCSTR lpPath)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_REMOVEDIRECTORY),"i"(SYSCALL_INT));
}
DLLEXPORT HANDLE GFS_CreateFile(LPCTSTR lpFilePath,DWORD dwOpenMode, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlags )
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_CREATEFILE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_CloseFile(HANDLE hFile)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_CLOSEFILE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_SetFilePointer(HANDLE hFile, DWORD dwNewPos, DWORD dwMoveMethod)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_SETFILEPOINTER),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_GetFilePointer(HANDLE hFile)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_GETFILEPOINTER),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_ReadFile(HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_READFILE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_WriteFile(HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_WRITEFILE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_DeleteFile(LPCTSTR lpFileName)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_DELETEFILE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_GetFileAttributes(LPCTSTR lpFileName, LPFILEINFO lpFINFO)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_GETFILEATTRIBUTES),"i"(SYSCALL_INT));
}
DLLEXPORT HANDLE GFS_FindFirstFile(LPCTSTR lpFileName, LPFILEINFO lpFINFO)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_FINDFIRSTFILE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_FindNextFile(HANDLE hFile, LPFILEINFO lpFINFO )
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_FINDNEXTFILE),"i"(SYSCALL_INT));
}
DLLEXPORT UINT32 GFS_FindClose(HANDLE hFile)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GFS_FINDCLOSE),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE KB_GetLastKey()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(KB_GETLASTKEY),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE KB_KeysAvailable()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(KB_KEYSAVAILABLE),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE KB_Read()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(KB_READ),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE KB_ConvertScanCode(BYTE ScanCode, BYTE Modifier)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(KB_CONVERTSCANCODE),"i"(SYSCALL_INT));
}
/*DLLEXPORT void KB_Unread(BYTE KeyCode)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(DD_ALLOCATERESOURCE),"i"(SYSCALL_INT));
}*/
DLLEXPORT BYTE KB_PressStatus()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(KB_PRESSSTATUS),"i"(SYSCALL_INT));
}
DLLEXPORT void VGA_SetTextAttribute(BYTE Attribute)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(KB_PRESSSTATUS),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE VGA_GetTextAttribute()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_GETTEXTATTRIBUTE),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE VGA_GetCurCol()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_GETCURCOL),"i"(SYSCALL_INT));
}
DLLEXPORT BYTE VGA_GetCurRow()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_GETCURROW),"i"(SYSCALL_INT));
}            
DLLEXPORT void VGA_PrintCharacter(BYTE Row,BYTE Col, BYTE Character)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_PRINTCHARACTER),"i"(SYSCALL_INT));
}
DLLEXPORT void VGA_PutCh(BYTE Character)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_PUTCH),"i"(SYSCALL_INT));
}
DLLEXPORT int  VGA_Print(char * String)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_PRINT),"i"(SYSCALL_INT));
}
DLLEXPORT void VGA_Fill(BYTE RowStart, BYTE ColStart,BYTE RowEnd , BYTE ColEnd, BYTE Ch_Att, BYTE Type)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_FILL),"i"(SYSCALL_INT));
}
DLLEXPORT void VGA_InsertRow()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_INSERTROW),"i"(SYSCALL_INT));
}
DLLEXPORT void VGA_SetCursorType(BYTE Start, BYTE End)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_SETCURSORTYPE),"i"(SYSCALL_INT));
}
DLLEXPORT void VGA_MoveCursor(BYTE Row,BYTE Col)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(VGA_MOVECURSOR),"i"(SYSCALL_INT));
}
DLLEXPORT HANDLE GetStdHandle(DWORD STDHANDLE)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GETSTDHANDLE),"i"(SYSCALL_INT));
}
DLLEXPORT void ExitProcess(DWORD dwExitCode)
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(EXITPROCESS),"i"(SYSCALL_INT));
}
DLLEXPORT LPTSTR GetCommandLine()
{
    __asm__ __volatile__ ("movl %0, %%eax;\
                int %1"::"i"(GETCOMMANDLINE),"i"(SYSCALL_INT));
}
