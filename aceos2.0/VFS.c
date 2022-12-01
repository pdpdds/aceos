/*      
    Virtual File System (VFS)
    It contains data structures and functions to access the 
    certain opened files like STDIN, STDOUT etc
    
    Created by Sam (samueldotj@gmail.com) on 28-May-2003
*/
#define _DEBUG_

#include <TypeDefs.h>
#include <GFS.h>
#include <VFS.h>
#include <VGAText.h>
#include <string.h>
#include <malloc.h>
#include <Keyboard.h>
#include <Util.h>

HANDLE hSTDIN,hSTDOUT,hSTDERR;
HANDLE GetStdHandle(DWORD STDHANDLE)
{
    switch (STDHANDLE)
    {
        case (DWORD)STDIN:
            return hSTDIN;
        case (DWORD)STDOUT:
            return hSTDOUT;
        case (DWORD)STDERR:
            return hSTDERR;
        default:
            return 0;
    }
}

HANDLE VFS_CreateFile(LPFSHANDLE lpFS, LPCTSTR lpFilePath, DWORD dwOpenMode, DWORD dwShareMode, DWORD dwCreationDisposition,DWORD dwFlags )
{
    if ( stricmp(lpFilePath,"STDIN")==0 )
        return STDIN;
    if ( stricmp(lpFilePath,"STDERR")==0 )
        return STDERR;
    if ( stricmp(lpFilePath,"STDOUT")==0 )
        return STDOUT;
    return 0;
}
UINT32 VFS_ReadFile(LPFSHANDLE lpFS, HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer)
{
    if ( hFile == STDIN )
    {
        int i;
        for(i=0;i<dwNoOfBytes;i++)
        {
            ((int *)lpBuffer)[i] = KB_ConvertScanCode(KB_Read(), 0);
        }
        return i;
    }
    else
        return 0;
}
UINT32 VFS_WriteFile(LPFSHANDLE lpFS, HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer)
{
    if ( hFile==STDOUT || hFile==STDERR )
    {
        int i;
        for(i=0;i<dwNoOfBytes;i++)
            VGA_PutCh(((char *)lpBuffer)[i]);
        return i;
    }
    else
        return 0;
}
int VFS_Init()
{
    LPFSHANDLE lpVFS;
    
    ALLOC_AND_CHECK( lpVFS, LPFSHANDLE, sizeof(FSHANDLE), 0);
    
    lpVFS->GetVolumeCapacity=NULL;
    lpVFS->GetVolumeFree=NULL;
    lpVFS->GetVolumeBad=NULL;

    lpVFS->CreateDirectory=NULL;
    lpVFS->RemoveDirectory=NULL;
    
    lpVFS->GetCurrentDirectory=NULL;
    lpVFS->SetCurrentDirectory=NULL;
    
    lpVFS->CreateFile=VFS_CreateFile;
    lpVFS->CloseFile=NULL;
    lpVFS->SetFilePointer=NULL;
    lpVFS->GetFilePointer=NULL;
    lpVFS->ReadFile=VFS_ReadFile;
    lpVFS->WriteFile=VFS_WriteFile;

    lpVFS->DeleteFile=NULL;
    lpVFS->GetFileAttributes=NULL;
    
    lpVFS->FindFirstFile=NULL;
    lpVFS->FindNextFile=NULL;
    lpVFS->FindClose=NULL;

    if ( GFS_RegisterForDevicePath( "VFS\\IORedirection", lpVFS ) ==FALSE )
    {
        DEBUG_PRINT_INFO("GFS_RegisterForDevicePath() failed");
        return 0;
    }
    if ( GFS_Register("VFS", lpVFS) == FALSE )
    {
        DEBUG_PRINT_INFO("GFS_Register() failed");
        return 0;
    }
    
    //open special files
    hSTDIN = GFS_CreateFile( (BYTE *)"STDIN", GENERIC_READ, 0, OPEN_EXISTING, 0);
    hSTDOUT = GFS_CreateFile( (BYTE *)"STDOUT", GENERIC_WRITE, 0, OPEN_EXISTING, 0);
    hSTDERR = GFS_CreateFile( (BYTE *)"STDERR", GENERIC_WRITE, 0, OPEN_EXISTING, 0);
    
    DEBUG_PRINT_OBJECT3("STDIN %X STDOUT %X STDERR %X", hSTDIN, hSTDOUT, hSTDERR);
    return 1;
}

