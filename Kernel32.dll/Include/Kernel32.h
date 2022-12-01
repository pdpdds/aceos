/*      Kernel System Call Interface for Drivers and User Applications
                            Created by Samuel on 31-May-2004 10:30PM                            
                            samueldotj@gamil.com
*/
#ifndef __KERNEL_32_H
#define __KERNEL_32_H

#include "TypeDefs.h"

#define VASM_MEM_USER       0x00000004
#define PAGE_READWRITE      0x00000002

#define VASM_PAGE_SIZE  4096

#define PAGE_ALIGN(addr)    ((addr) & -VASM_PAGE_SIZE)
#define PAGE_ALIGN_UP(addr)    (((addr) + VASM_PAGE_SIZE - 1) & -VASM_PAGE_SIZE)

#define MEM_COMMIT      1
#define MEM_RESERVE     2
#define MEM_RESET       4
#define MEM_TOP_DOWN    8
#define MEM_DECOMMIT    16
#define MEM_RELEASE     32

#define STDIN  (DWORD)1
#define STDOUT (DWORD)2
#define STDERR (DWORD)3

#define STD_INPUT_HANDLE  STDIN
#define STD_OUTPUT_HANDLE STDOUT
#define STD_ERROR_HANDLE  STDOUT

#define _MAX_PATH   260 /* max. length of full pathname */
#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */


#define GENERIC_READ            0x1
#define GENERIC_WRITE           0x2
#define FILE_SHARE_READ         0x4
#define FILE_SHARE_WRITE        0x8

#define CREATE_NEW              0x1
#define CREATE_ALWAYS           0x2
#define OPEN_EXISTING           0x4
#define OPEN_ALWAYS             0x8
#define TRUNCATE_EXISTING       0x10

#define FILE_ATTRIBUTE_READONLY 0x01
#define FILE_ATTRIBUTE_HIDDEN   0x02
#define FILE_ATTRIBUTE_SYSTEM   0x04
#define FILE_ATTRIBUTE_VOL_ID   0x08
#define FILE_ATTRIBUTE_DIR      0x10
#define FILE_ATTRIBUTE_ARCHIVE  0x20
#define FILE_ATTRIBUTE_LONG_NAME ( FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_VOL_ID )

#define INVALID_HANDLE_VALUE (-1)

#ifndef _SYSTEM_INFO_DEFINED
    #define _SYSTEM_INFO_DEFINED
   
    struct _SYSTEM_INFO 
    {
        char   CPU_VendorID[33]; 
        UINT32 CPU_Signature; 
        UINT32 CPU_FeatureFlags[3]; 
        
        UINT16 MEM_PhysicalAvailable;
        UINT16 MEM_PhysicalTotal;
        UINT16 MEM_PageSize;
        UINT32 MEM_AllocationGranularity;
    }__attribute__ ((packed)); 
    typedef struct _SYSTEM_INFO SYSTEM_INFO;
    typedef SYSTEM_INFO * LPSYSTEM_INFO;
#endif

#ifndef _SYSTIME_DEFINED
    #define _SYSTIME_DEFINED
    struct _SYSTEMTIME
    {
        WORD wYear; 
        WORD wMonth; 
        WORD wDayOfWeek; 
        WORD wDay; 
        WORD wHour; 
        WORD wMinute; 
        WORD wSecond; 
        WORD wMilliseconds; 
    }; 
    typedef struct _SYSTEMTIME SYSTEMTIME;
    typedef SYSTEMTIME * LPSYSTEMTIME;
#endif

#ifndef _HHEAPINFO_DEFINED
    #define _HHEAPINFO_DEFINED
    struct Heap
    {
        UINT32 dwSize;             //size of the memory segement
        BOOLEAN IsUsed;          //Free or Occupied  
        struct Heap * Next;      //pointer to next memory segment
        struct Heap * Previous;  //pointer to next memory segment
    }__attribute__ ((packed));
    typedef struct Heap HEAP;
    typedef HEAP * LPHEAP;
    
    struct HeapInfo
    {
        LPHEAP hHeapRoot;
        UINT32 dwOptions;
        UINT32 dwMaximumSize;
        UINT32 dwCurrentSize;
    }__attribute__ ((packed));
    typedef struct HeapInfo HEAPINFO;
    typedef HEAPINFO * LPHEAPINFO;
#endif


#ifndef _FILEINFO_DEFINED
    #define _FILEINFO_DEFINED
    typedef struct _FILETIME {
    DWORD dwLowDateTime;   /* low 32 bits  */
    DWORD dwHighDateTime;  /* high 32 bits */
    } FILETIME, *PFILETIME, *LPFILETIME;

    struct GFS_FileInfo
    {
        DWORD       dwFileAttributes; 
        FILETIME    ftCreationTime; 
        FILETIME    ftLastAccessTime; 
        FILETIME    ftLastWriteTime; 
        DWORD       nFileSizeHigh; 
        DWORD       nFileSizeLow; 
        DWORD       dwReserved0; 
        DWORD       dwReserved1; 
        CHAR        cFileName[ _MAX_PATH ]; 
        CHAR        cAlternateFileName[ 14 ]; 
        BYTE        Reserved[20];
    };
    typedef struct GFS_FileInfo FILEINFO;
    typedef FILEINFO * LPFILEINFO;
#endif

    
#ifndef _GSH_IO_PARAMETER_DEFINED
    #define _GSH_IO_PARAMETER_DEFINED
    struct _GSH_IO_Parameter
    {
        UINT32 dwCylinder;
        UINT32 dwHead;
        UINT32 dwSector;
    
        UINT32 dwLBASector;
    
        UINT32 dwSectorCount;
    
        BYTE   bMode;		//if bit 0 is set LBA mode
    
        UINT32 Reserved[20];
    }__attribute__ ((packed));
#endif
    
typedef struct _GSH_IO_Parameter GSHIOPARA;
typedef GSHIOPARA * LPGSHIOPARA;

#ifdef __cplusplus
    extern "C" {
#endif

UINT32 DllMain();
UINT32 GetLastError();
UINT32 SetLastError(UINT32 dwErrorCode);
UINT32 GetSystemInfo(struct _SYSTEM_INFO * SysInfo);
void GetLocalTime(SYSTEMTIME * lpSystemTime);
BYTE SetLocalTime(SYSTEMTIME * lpSystemTime);
UINT32 GetTimerTick(void);
void Sound(UINT16 Frequency);
void NoSound();
UINT32 VirtualAlloc(UINT32 StartingAddress, UINT32 Size, UINT16 AllocationType, UINT16 ProtectionType );
UINT32 VirtualAllocEx(UINT32 TaskID, UINT32 StartingAddress, UINT32 Size, UINT16 AllocationType, UINT16 ProtectionType );
UINT32 VirtualFree(UINT32 StartingAddress, UINT32 Size, DWORD FreeType);
UINT32 VirtualFreeEx(UINT32 TaskID, UINT32 StartingAddress, UINT32 Size, DWORD FreeType);
UINT32 VASM_GetPageSize();
LPHEAPINFO HeapCreate(UINT32 dwOptions, UINT32 dwInitialSize, UINT32 dwMaximumSize);
void * HeapAlloc(LPHEAPINFO hHeapInfo, UINT32 dwSize);
BYTE HeapFree(LPHEAPINFO hHeapInfo, void * lpAddress);
void * GlobalAlloc(UINT32 dwSize);
void * GlobalRealloc(void * lpAddress, UINT32 dwSize);
void * GlobalFree(void * lpAddress);
BYTE GFS_GetRegisteredFSTypes(BYTE (*fnFeedBack)(const BYTE bFSType,const char * szDescription));
UINT32 GFS_GetVolumeCapacity(LPCSTR szVolumePath);
UINT32 GFS_GetVolumeFree (LPCSTR szVolumePath);
UINT32 GFS_GetVolumeBad  (LPCSTR szVolumePath);
UINT32 GFS_CreateDirectory(LPCSTR lpPath);
UINT32 GFS_RemoveDirectory(LPCSTR lpPath);
HANDLE GFS_CreateFile(LPCTSTR lpFilePath,DWORD dwOpenMode, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlags );
UINT32 GFS_CloseFile(HANDLE hFile);
UINT32 GFS_SetFilePointer(HANDLE hFile, DWORD dwNewPos, DWORD dwMoveMethod);
UINT32 GFS_GetFilePointer(HANDLE hFile);
UINT32 GFS_ReadFile(HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer);
UINT32 GFS_WriteFile(HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer);
UINT32 GFS_DeleteFile(LPCTSTR lpFileName);
UINT32 GFS_GetFileAttributes(LPCTSTR lpFileName, LPFILEINFO lpFINFO);
HANDLE GFS_FindFirstFile(LPCTSTR lpFileName, LPFILEINFO lpFINFO);
UINT32 GFS_FindNextFile(HANDLE hFile, LPFILEINFO lpFINFO );
UINT32 GFS_FindClose(HANDLE hFile);
BYTE KB_GetLastKey();
BYTE KB_KeysAvailable();
BYTE KB_Read();
BYTE KB_ConvertScanCode(BYTE ScanCode, BYTE Modifier);
BYTE KB_PressStatus();
void VGA_SetTextAttribute(BYTE Attribute);
BYTE VGA_GetTextAttribute();
BYTE VGA_GetCurCol();
BYTE VGA_GetCurRow();
void VGA_PrintCharacter(BYTE Row,BYTE Col, BYTE Character);
void VGA_PutCh(BYTE Character);
int  VGA_Print(char * String);
void VGA_Fill(BYTE RowStart, BYTE ColStart,BYTE RowEnd , BYTE ColEnd, BYTE Ch_Att, BYTE Type);
void VGA_InsertRow();
void VGA_SetCursorType(BYTE Start, BYTE End);
void VGA_MoveCursor(BYTE Row,BYTE Col);
HANDLE GetStdHandle(DWORD STDHANDLE);
void ExitProcess(DWORD dwExitCode);
LPTSTR GetCommandLine();

HANDLE GetStdHandle(DWORD STDHANDLE);

#ifdef __cplusplus
    }
#endif
    
   
#endif


    