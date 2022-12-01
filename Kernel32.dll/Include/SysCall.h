/*
	Ace OS Source Code - System Calls
                Created on 24-May-2004 21:43 by Samuel (samueldotj@gmail.com)
*/
#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <TypeDefs.h>

#ifdef __cplusplus
    extern "C" {
#endif

#define SYSCALL_INT 50

struct _SysCallInfo
{
    UINT32 (*lpSysCall)();     //system function pointer        
    UINT32 dwCopyStackSize;     //stack size to copy in dwords
}__attribute__ ((packed));

typedef struct _SysCallInfo SYSCALLINFO;
typedef SYSCALLINFO * LPSYSCALLINFO;

enum _SysCallNo
{
    GETLASTERROR=0,
    SETLASTERROR,
    
    GETSYSTEMINFO,
    GETLOCALTIME,
    SETLOCALTIME,
    GETTIMERTICK,
    MDELAY,
    
    SOUND,
    NOSOUND,
        
    VIRTUALALLOC,
    VIRTUALFREE,
    VASM_GETPAGESIZE,
    HEAPALLOC,
    HEAPCREATE,
    HEAPFREE,
    GLOBALALLOC,
    GLOBALREALLOC,
    GLOBALFREE,
    
    GFS_CLOSEFILE,
    GFS_CREATEDIRECTORY,
    GFS_CREATEFILE,
    GFS_DELETEFILE,
    GFS_FINDCLOSE,
    GFS_FINDFIRSTFILE,
    GFS_FINDNEXTFILE,
    GFS_GETFILEATTRIBUTES,
    GFS_GETFILEPOINTER,
    GFS_GETNOOFVOLUMES,
    GFS_GETREGISTEREDFSTYPES,
    GFS_GETVOLUMEBAD,
    GFS_GETVOLUMECAPACITY,
    GFS_GETVOLUMEFREE,
    GFS_READFILE,
    GFS_REMOVEDIRECTORY,
    GFS_SETFILEPOINTER,
    GFS_WRITEFILE,
    
    GETSTDHANDLE,
    
    KB_CONVERTSCANCODE,
    KB_GETLASTKEY,
    KB_GETRATE,
    KB_KEYSAVAILABLE,
    KB_PRESSSTATUS,
    KB_READ,
    KB_SETRATE,

    VGA_FILL,
    VGA_GETCURCOL,
    VGA_GETCURROW,
    VGA_GETTEXTATTRIBUTE,
    VGA_INSERTROW,
    VGA_MOVECURSOR,
    VGA_PRINT,
    VGA_PRINTCHARACTER,
    VGA_PUTCH,
    VGA_SETCURSORTYPE,
    VGA_SETTEXTATTRIBUTE,
    
    CREATETASK,
    REMOVETASK,
    EXITPROCESS,
    
    GETCOMMANDLINE,
    
    SYSCALL_END
};
extern SYSCALLINFO _SysCallDispatchTable[SYSCALL_END];

void InitSysCallTable();

#ifdef __cplusplus
    }
#endif

#endif

