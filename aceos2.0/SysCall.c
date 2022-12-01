/*
  Name: 		System Call Dispatcher - Interrupt 50 Handler
  Author: 			- Samuel
  Created Date: 20-Mar-2004 11:18PM
*/
#include <SysCall.h>
#include <SysInfo.h>
#include <VGAText.h>
#include <Timer.h>
#include <RTC.h>
#include <Beep.h>
#include <DeviceDriver.h>
#include <FAT.h>
#include <GFS.h>
#include <GSH.h>
#include <Memory.h>
#include <Heap.h>
#include <Keyboard.h>
#include <VFS.h>
#include <TaskMan.h>
#include <Environment.h>

SYSCALLINFO _SysCallDispatchTable[SYSCALL_END];
/*
Be carefull about these static table initialization, 
if you forget to initialize them correctly, they will become a hell for you
*/
void InitSysCallTable()
{
    int i;
    for(i=0;i<SYSCALL_END;i++)
    {
        _SysCallDispatchTable[i].lpSysCall = NULL;         //just initialize every function pointer to null
        _SysCallDispatchTable[i].dwCopyStackSize = 0;      //no parameters
    }
    
    _SysCallDispatchTable[GETLASTERROR].lpSysCall = (UINT32 (*)())GetLastError;
    _SysCallDispatchTable[SETLASTERROR].lpSysCall = (UINT32 (*)())SetLastError;
    _SysCallDispatchTable[SETLASTERROR].dwCopyStackSize = 1;
    
    _SysCallDispatchTable[GETSYSTEMINFO].lpSysCall = (UINT32 (*)())GetSystemInfo;
    _SysCallDispatchTable[GETSYSTEMINFO].dwCopyStackSize = 1;
    
    _SysCallDispatchTable[GETLOCALTIME].lpSysCall = (UINT32 (*)())GetLocalTime;
    _SysCallDispatchTable[GETLOCALTIME].dwCopyStackSize = 1;
    _SysCallDispatchTable[SETLOCALTIME].lpSysCall = (UINT32 (*)())SetLocalTime;
    _SysCallDispatchTable[SETLOCALTIME].dwCopyStackSize = 1;
    
    _SysCallDispatchTable[GETTIMERTICK].lpSysCall = (UINT32 (*)())GetTimerTick ;
    
    _SysCallDispatchTable[SOUND].lpSysCall = (UINT32 (*)())Sound;                 
    _SysCallDispatchTable[SOUND].dwCopyStackSize = 1;                 //UINT16 Frequency
    _SysCallDispatchTable[NOSOUND].lpSysCall = (UINT32 (*)())NoSound;
    
    _SysCallDispatchTable[VIRTUALALLOC].lpSysCall = (UINT32 (*)())VirtualAlloc;
    _SysCallDispatchTable[VIRTUALALLOC].dwCopyStackSize = 5;      /// UINT32 TaskID, UINT32 StartingAddress, UINT32 Size, UINT16 AllocationType, UINT16 ProtectionType 
    _SysCallDispatchTable[VIRTUALFREE].lpSysCall = (UINT32 (*)())VirtualFree;
    _SysCallDispatchTable[VIRTUALFREE].dwCopyStackSize = 3;   //UINT32 TaskID, UINT32 StartingAddress, UINT32 Size
    _SysCallDispatchTable[VASM_GETPAGESIZE].lpSysCall = (UINT32 (*)())VASM_GetPageSize;
    
    _SysCallDispatchTable[HEAPALLOC].lpSysCall = (UINT32 (*)())HeapAlloc;
    _SysCallDispatchTable[HEAPALLOC].dwCopyStackSize = 2;     //HHEAPINFO hHeapInfo, UINT32 dwSize
    _SysCallDispatchTable[HEAPCREATE].lpSysCall = (UINT32 (*)())HeapCreate;
    _SysCallDispatchTable[HEAPCREATE].dwCopyStackSize = 3;     //UINT32 dwOptions, UINT32 dwInitialSize, UINT32 dwMaximumSize
    _SysCallDispatchTable[HEAPFREE].lpSysCall = (UINT32 (*)())HeapFree;
    _SysCallDispatchTable[HEAPFREE].dwCopyStackSize = 2;        //HHEAPINFO hHeapInfo, void * lpAddress
    
    _SysCallDispatchTable[GLOBALALLOC].lpSysCall = (UINT32 (*)())GlobalAlloc;
    _SysCallDispatchTable[GLOBALALLOC].dwCopyStackSize = 1;        //(UINT32 dwSize);
    _SysCallDispatchTable[GLOBALREALLOC].lpSysCall = (UINT32 (*)())GlobalRealloc;
    _SysCallDispatchTable[GLOBALREALLOC].dwCopyStackSize = 2;        //(void * lpAddress, UINT32 dwSize)
    _SysCallDispatchTable[GLOBALFREE].lpSysCall = (UINT32 (*)())GlobalFree;
    _SysCallDispatchTable[GLOBALFREE].dwCopyStackSize = 1;        //void * lpAddress
    
    _SysCallDispatchTable[GFS_CLOSEFILE].lpSysCall = (UINT32 (*)())GFS_CloseFile;
    _SysCallDispatchTable[GFS_CLOSEFILE].dwCopyStackSize = 1;   //HANDLE hFile
    _SysCallDispatchTable[GFS_CREATEDIRECTORY].lpSysCall = (UINT32 (*)())GFS_CreateDirectory;
    _SysCallDispatchTable[GFS_CREATEDIRECTORY].dwCopyStackSize = 1;     //LPCSTR lpPath
    _SysCallDispatchTable[GFS_CREATEFILE].lpSysCall = (UINT32 (*)())GFS_CreateFile;
    _SysCallDispatchTable[GFS_CREATEFILE].dwCopyStackSize = 5; //LPCTSTR lpFilePath,DWORD dwOpenMode, DWORD dwShareMode, DWORD dwCreationDisposition,DWORD dwFlags
    _SysCallDispatchTable[GFS_DELETEFILE].lpSysCall = (UINT32 (*)())GFS_DeleteFile;
    _SysCallDispatchTable[GFS_DELETEFILE].dwCopyStackSize = 1; //LPCTSTR lpFileName
    _SysCallDispatchTable[GFS_FINDCLOSE].lpSysCall = (UINT32 (*)())GFS_FindClose;
    _SysCallDispatchTable[GFS_FINDCLOSE].dwCopyStackSize = 1; //HANDLE hFile
    _SysCallDispatchTable[GFS_FINDFIRSTFILE].lpSysCall = (UINT32 (*)())GFS_FindFirstFile;
    _SysCallDispatchTable[GFS_FINDFIRSTFILE].dwCopyStackSize = 2;  //LPCTSTR lpFileName, LPFILEINFO lpFINFO
    _SysCallDispatchTable[GFS_FINDNEXTFILE].lpSysCall = (UINT32 (*)())GFS_FindNextFile;
    _SysCallDispatchTable[GFS_FINDNEXTFILE].dwCopyStackSize = 2;      //LPCTSTR lpFileName, LPFILEINFO lpFINFO
    _SysCallDispatchTable[GFS_GETFILEATTRIBUTES].lpSysCall = (UINT32 (*)())GFS_GetFileAttributes;
    _SysCallDispatchTable[GFS_GETFILEATTRIBUTES].dwCopyStackSize = 2;
    _SysCallDispatchTable[GFS_GETFILEPOINTER].lpSysCall = (UINT32 (*)())GFS_GetFilePointer;
    _SysCallDispatchTable[GFS_GETFILEPOINTER].dwCopyStackSize = 1; //HANDLE hFile
    //_SysCallDispatchTable[GFS_GETNOOFVOLUMES].lpSysCall = GFS_NoOfVolumes;
    //_SysCallDispatchTable[GFS_GETREGISTEREDFSTYPES].lpSysCall = (UINT32 (*)())GFS_GetRegisteredFSTypes;
    //_SysCallDispatchTable[GFS_GETREGISTEREDFSTYPES].dwCopyStackSize = 1; //BYTE (*fnFeedBack)(const BYTE bFSType,const char * szDescription)
    _SysCallDispatchTable[GFS_GETVOLUMEBAD].lpSysCall = (UINT32 (*)())GFS_GetVolumeBad;
    _SysCallDispatchTable[GFS_GETVOLUMEBAD].dwCopyStackSize = 1; //LPCSTR szVolumePath
    _SysCallDispatchTable[GFS_GETVOLUMECAPACITY].lpSysCall = (UINT32 (*)())GFS_GetVolumeCapacity;
    _SysCallDispatchTable[GFS_GETVOLUMECAPACITY].dwCopyStackSize = 1;   //LPCSTR szVolumePath
    _SysCallDispatchTable[GFS_GETVOLUMEFREE].lpSysCall = (UINT32 (*)())GFS_GetVolumeFree;
    _SysCallDispatchTable[GFS_GETVOLUMEFREE].dwCopyStackSize = 1; //LPCSTR szVolumePath
    _SysCallDispatchTable[GFS_READFILE].lpSysCall = (UINT32 (*)())GFS_ReadFile;
    _SysCallDispatchTable[GFS_READFILE].dwCopyStackSize = 3;             //HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer
    _SysCallDispatchTable[GFS_REMOVEDIRECTORY].lpSysCall = (UINT32 (*)())GFS_RemoveDirectory;
    _SysCallDispatchTable[GFS_REMOVEDIRECTORY].dwCopyStackSize = 1;      //LPCSTR lpPath
    _SysCallDispatchTable[GFS_SETFILEPOINTER].lpSysCall = (UINT32 (*)())GFS_SetFilePointer;
    _SysCallDispatchTable[GFS_SETFILEPOINTER].dwCopyStackSize = 3;       //HANDLE hFile, DWORD dwNewPos, DWORD dwMoveMethod
    _SysCallDispatchTable[GFS_WRITEFILE].lpSysCall = (UINT32 (*)())GFS_WriteFile;
    _SysCallDispatchTable[GFS_WRITEFILE].dwCopyStackSize = 3;            //HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer
    
    _SysCallDispatchTable[GETSTDHANDLE].lpSysCall = (UINT32 (*)())GetStdHandle;
    _SysCallDispatchTable[GETSTDHANDLE].dwCopyStackSize = 1; //DWORD STDHANDLE
    
    _SysCallDispatchTable[KB_CONVERTSCANCODE].lpSysCall = (UINT32 (*)())KB_ConvertScanCode;
    _SysCallDispatchTable[KB_CONVERTSCANCODE].dwCopyStackSize = 2;              //BYTE ScanCode, BYTE Modifier
    _SysCallDispatchTable[KB_GETLASTKEY].lpSysCall = (UINT32 (*)())KB_GetLastKey;
    _SysCallDispatchTable[KB_GETRATE].lpSysCall = (UINT32 (*)())KB_GetRate;
    _SysCallDispatchTable[KB_GETRATE].dwCopyStackSize = 2;      //UINT16 Rate,BYTE Delay
    _SysCallDispatchTable[KB_KEYSAVAILABLE].lpSysCall = (UINT32 (*)())KB_KeysAvailable;
    _SysCallDispatchTable[KB_PRESSSTATUS].lpSysCall = (UINT32 (*)())KB_PressStatus;
    _SysCallDispatchTable[KB_READ].lpSysCall = (UINT32 (*)())KB_Read;
    _SysCallDispatchTable[KB_SETRATE].lpSysCall = (UINT32 (*)())KB_SetRate;
    _SysCallDispatchTable[KB_SETRATE].dwCopyStackSize = 2;      //UINT16 Rate,BYTE Delay
    
    _SysCallDispatchTable[VGA_FILL].lpSysCall = (UINT32 (*)())VGA_Fill;
    _SysCallDispatchTable[VGA_FILL].dwCopyStackSize = 6;         //BYTE RowStart, BYTE ColStart,BYTE RowEnd , BYTE ColEnd, BYTE Ch_Att, BYTE Type
    _SysCallDispatchTable[VGA_GETCURCOL].lpSysCall = (UINT32 (*)())VGA_GetCurCol;
    _SysCallDispatchTable[VGA_GETCURROW].lpSysCall = (UINT32 (*)())VGA_GetCurRow;
    _SysCallDispatchTable[VGA_GETTEXTATTRIBUTE].lpSysCall = (UINT32 (*)())VGA_GetTextAttribute;
    _SysCallDispatchTable[VGA_INSERTROW].lpSysCall = (UINT32 (*)())VGA_InsertRow;
    _SysCallDispatchTable[VGA_MOVECURSOR].lpSysCall = (UINT32 (*)())VGA_MoveCursor;
    _SysCallDispatchTable[VGA_MOVECURSOR].dwCopyStackSize = 2;   //BYTE Row,BYTE Col
    _SysCallDispatchTable[VGA_PRINT].lpSysCall = (UINT32 (*)())VGA_Print;
    _SysCallDispatchTable[VGA_PRINT].dwCopyStackSize = 1;       //char * String
    _SysCallDispatchTable[VGA_PRINTCHARACTER].lpSysCall = (UINT32 (*)())VGA_PrintCharacter;
    _SysCallDispatchTable[VGA_PRINTCHARACTER].dwCopyStackSize = 3; //BYTE Row,BYTE Col, BYTE Character
    _SysCallDispatchTable[VGA_PUTCH].lpSysCall = (UINT32 (*)())VGA_PutCh;
    _SysCallDispatchTable[VGA_PUTCH].dwCopyStackSize = 1;       //BYTE Character
    _SysCallDispatchTable[VGA_SETCURSORTYPE].lpSysCall = (UINT32 (*)())VGA_SetCursorType;
    _SysCallDispatchTable[VGA_SETCURSORTYPE].dwCopyStackSize = 2;       //BYTE Bottom, BYTE Top
    _SysCallDispatchTable[VGA_SETTEXTATTRIBUTE].lpSysCall = (UINT32 (*)())VGA_SetTextAttribute;
    _SysCallDispatchTable[VGA_SETTEXTATTRIBUTE].dwCopyStackSize = 1;    //BYTE Attribute
    //_SysCallDispatchTable[].lpSysCall = ;
    _SysCallDispatchTable[CREATETASK].lpSysCall = (UINT32 (*)())CreateTask;
    _SysCallDispatchTable[CREATETASK].dwCopyStackSize = 2;    //LPCSTR szFilePath, LPCSTR szFileName
    _SysCallDispatchTable[REMOVETASK].lpSysCall = (UINT32 (*)())RemoveTask;
    _SysCallDispatchTable[REMOVETASK].dwCopyStackSize = 1;    //UINT32 TaskID;
    _SysCallDispatchTable[EXITPROCESS].lpSysCall = (UINT32 (*)())ExitProcess;
    _SysCallDispatchTable[EXITPROCESS].dwCopyStackSize = 1;    //(DWORD dwExitCode)
    
    _SysCallDispatchTable[GETCOMMANDLINE].lpSysCall = (UINT32 (*)())GetCommandLine;
    _SysCallDispatchTable[GETCOMMANDLINE].dwCopyStackSize = 0;    //(DWORD dwExitCode)
    
    //printf("\n\r _SysCallDispatchTable %X", _SysCallDispatchTable);
    
}
