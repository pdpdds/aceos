/* 		Device Driver functions ACE OS kernel		
			-created by Samuel (04-Sep-2002)	
			samuelhard@yahoo.com
*/
#include <DeviceDriver.h>
#include <malloc.h>
#include <string.h>
#include <List.h>
#include <TaskMan.h>
#include <PE.h>
#include <Thread.h>
#include <Memory.h>
UINT32 DD_LoadDriver(char * szDriverFile)
{
    struct PE_IMAGE_LOAD_INFO PI;
    struct PEExportInfo * PEExpoInfo;
    void (*DriverEntry)();
    
    //load PE file and read COFF header
    PI.lpExportDirTableList=NULL;
    if ( PE_LoadFile( szDriverFile, &PI) == NULL )
    {
        dbgprintf("\n\r DD_LoadDriver :: PE_LoadFile() failed.");
        return 0;
    }
        
    PI.lpLoadedAddress=PI.lpImage;
    PEExpoInfo = (struct PEExportInfo *) malloc(sizeof( struct PEExportInfo ));
    PEExpoInfo->lpExportDirTab=PI.lpExportDirTable;
    PEExpoInfo->lpImage=PI.lpImage;
    
    //exporting symbols to kernel
    //List_Add( &sys_KernelTask.PEExportList, PI.lpExportDirTable->NameRVA + (UINT32)PI.lpImage , PEExpoInfo );

    //importing symbols from kernel  
    //PE_ResolveImports(PI.lpImage, 1, PI.lpImportDirTable, _KernelImageInfo.lpImage, sys_KernelTask.PEExportList );
    
    INT32 Delta=(UINT32)PI.lpLoadedAddress - (UINT32) PI.lpPreferedLoadAddress;
    PE_RelocateImage(Delta, PI.lpPEFile, PI.lpImage, PI.lpRelocationSection);
    
    DriverEntry=PE_GetExportSymbolByName(PI.lpImage, PI.lpExportDirTable,"DriverEntry" );
    free(PI.lpPEFile); //free the file
    
    if ( DriverEntry == NULL )
    {
        printf("\n\r DD :: LoadDriver Failed. Symbol _DriverEntry not found");
        return 0;
    }
    CreateThread(DriverEntry,10000,TP_NORMAL); //attach the thread
    return 1;
}
UINT32 DD_AllocateResource(UINT16 wResourceType, void * lpStarting, UINT32 Size)
{
    HTHREAD hThread=GetCurrentThread();
    
    if ( hThread == NULL )
        return 0;
    switch ( wResourceType )
    {
        case DD_R_MEMORY:       //allocate a linear memory and then map it to the given physical address range
            dbgprintf("\n-----allocate mem(%X,%ld)",lpStarting,Size);
            void * x=KernelMap(0, Size, lpStarting );
            dbgprintf("\n %X",x);
            dbgprintf("\n %X",GetPageEntry((UINT32)x/VASM_PAGE_SIZE,     VASM_TPT ));
            return (UINT32)x;
        case DD_R_IO:
            if ( hThread->IOBASE == NULL )
                return 0;
            mm_C_S_BitsInBitmap(hThread->IOBASE, 0, (UINT32)lpStarting, Size);
            return 1;
        default:
            return 0;
    }
}
