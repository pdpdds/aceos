/*                      Ace OS ( PELoader )  
                            - Created by samueldotj@gmail.com
                            - Created on 23-Jul-2003 
                            - Modified on 3-Jul-2005 split long functions into smaller ones
*/

#define _DEBUG_

#include <GFS.H>
#include <PE.H>
#include <Taskman.h>
#include <Memory.H>
#include <malloc.H>
#include <SysInfo.H>
#include <Util.H>

#define PE_INITIALIZE_STRUCTURES() \
    lpPI->lpImage=0;\
    lpPI->dwSizeOfImage=0;\
    lpPI->lpPEFile=NULL;\
    lpPI->lpRelocationSection=NULL;\
    lpPI->lpExportDirTable=NULL;\
    lpPI->lpImportDirTable=NULL;\
    lpPI->lpPreferedLoadAddress=lpPI->lpLoadedAddress=0;\
    lpPI->AddressOfEntryPoint = NULL;\
    lpDOSHeader=NULL;\
    lpCOFFHeader=NULL;\
    lpOptionalHeaderSTD=NULL;\
    lpOptionalHeaderNT=NULL;\
    
#define PE_LOADFILEINTOMEMORY_CLEANUP_AND_RETURN( ErrorCode )\
    free(lpPEFile);\
    GFS_CloseFile(hPEFile);\
    if ( ErrorCode != 0 )\
        SetLastError( ErrorCode );\
    return NULL;
    
#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGH 1
#define IMAGE_REL_BASED_LOW 2
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_HIGHADJ 4
#define IMAGE_REL_BASED_MIPS_JMPADDR 5
#define IMAGE_REL_BASED_SECTION 6
#define IMAGE_REL_BASED_REL32 7
#define IMAGE_REL_BASED_MIPS_JMPADDR16 9
#define IMAGE_REL_BASED_DIR64 10
#define IMAGE_REL_BASED_HIGH3ADJ 11

/*Internal function - To separate file related operations from the main logic.
lip - Long Indirect Pointer :) 
1) It should take care the pointer returned.
*/
void * PE_LoadFileIntoMemory(LPCSTR szFilePath, LP_IMAGE_DOS_HEADER *lipDOSHeader, LPCOFF_FILE_HEADER *lipCOFFHeader, LPPE_OPTIONAL_HEADER_STD *lipOptionalHeaderSTD, void **lipOptionalHeaderNT)
{
    FILEINFO FINFO;
    UINT32 dwFileSize;
    HANDLE hPEFile;
    BYTE * lpPEFile;
    
    GFS_GetFileAttributes( szFilePath, &FINFO);
    dwFileSize = FINFO.nFileSizeLow;
    
    hPEFile=GFS_CreateFile( szFilePath, GENERIC_READ, 0, OPEN_EXISTING, 0);
    
    if ( hPEFile == 0 )
    {
        DEBUG_PRINT_OBJECT1("Unable to open the file %s", szFilePath);
        //last error will set by GFS
        return NULL;
    }
    lpPEFile=(BYTE *)malloc(dwFileSize);
    if ( lpPEFile == NULL )
    {
        DEBUG_PRINT_OBJECT1("Unable to allocate memory (%d)", dwFileSize);
        PE_LOADFILEINTOMEMORY_CLEANUP_AND_RETURN( ERROR_NOT_ENOUGH_MEMORY );
    }
    if ( GFS_ReadFile(hPEFile, dwFileSize, lpPEFile) == 0 )
    {
        DEBUG_PRINT_OBJECT1("Unable to read the file %s", szFilePath);
        PE_LOADFILEINTOMEMORY_CLEANUP_AND_RETURN( ERROR_READ_FAULT );
    }
    *lipDOSHeader = (LP_IMAGE_DOS_HEADER)lpPEFile;
    if ( (*lipDOSHeader)->e_magic != IMAGE_DOS_SIGNATURE )
    {
        DEBUG_PRINT_OBJECT2("DOS Signature mismatch %X!=%X", (*lipDOSHeader)->e_magic, IMAGE_DOS_SIGNATURE );
        PE_LOADFILEINTOMEMORY_CLEANUP_AND_RETURN( ERROR_INVALID_EXE_SIGNATURE );
    }
    if ( ((UINT32 *)( lpPEFile + (*lipDOSHeader)->e_lfanew) )[0] != IMAGE_NT_SIGNATURE  )
    {
        DEBUG_PRINT_OBJECT2("PE Signature mismatch %X!=%X", ((UINT32 *)(lpPEFile+ (*lipDOSHeader)->e_lfanew))[0],  IMAGE_NT_SIGNATURE );
        PE_LOADFILEINTOMEMORY_CLEANUP_AND_RETURN( 0 );
    }
    *lipCOFFHeader = (LPCOFF_FILE_HEADER ) (lpPEFile+ (*lipDOSHeader)->e_lfanew+4);
    if ( (*lipCOFFHeader)->Machine != IMAGE_FILE_MACHINE_I386 )
    {
        DEBUG_PRINT_OBJECT1("Machine Type %X Not Supported", (*lipCOFFHeader)->Machine);
        PE_LOADFILEINTOMEMORY_CLEANUP_AND_RETURN( ERROR_NOT_SUPPORTED );
    }
    if ( (*lipCOFFHeader)->Characterstics & IMAGE_FILE_RELOCS_STRIPPED )
    {
        DEBUG_PRINT_OBJECT1("Relocations Stripped", (*lipCOFFHeader)->Machine);
        PE_LOADFILEINTOMEMORY_CLEANUP_AND_RETURN( ERROR_NOT_SUPPORTED  );
    }
    
    *lipOptionalHeaderSTD = (LPPE_OPTIONAL_HEADER_STD) ( (UINT32)(*lipCOFFHeader) + sizeof(COFF_FILE_HEADER) );
    *lipOptionalHeaderNT = (void *) ( (UINT32)(*lipOptionalHeaderSTD) + sizeof(PE_OPTIONAL_HEADER_STD) );
    
    GFS_CloseFile( hPEFile );
    
    DEBUG_PRINT_OBJECT1("returns %X", lpPEFile);
    return lpPEFile;
}
/* This function loads the specified PE file into memory and returns the memory address*/
LPPE_IMAGE_LOAD_INFO PE_LoadFile(LPCSTR szFilePath, LPCSTR szFileName, LPPE_IMAGE_LOAD_INFO lpPI)
{
    DEBUG_PRINT_OBJECT2("( %s %s) ",szFilePath, szFileName);
    BYTE * lpPEFile, *lpImage;
    int i;
    char szFilePathName[_MAX_PATH];
    LP_IMAGE_DOS_HEADER lpDOSHeader;
    LPCOFF_FILE_HEADER lpCOFFHeader;
    LPPE_OPTIONAL_HEADER_STD lpOptionalHeaderSTD;
    void *lpOptionalHeaderNT;
    
    LP_IMAGE_DATA_DIRECTORY lpDataDirectory;
    LPPE_SECTION_TABLE lpSectionTable;
    UINT32 dwSizeOfImage, dwNumberOfRVAandSizes;
    
    PE_INITIALIZE_STRUCTURES();
    
    
    if ( szFilePath == NULL || strlen(szFilePath)<=1)
        return NULL;
    strcpy( szFilePathName, szFilePath );
    if ( szFilePath[strlen(szFilePath)] != '\\' )
        strcat( szFilePathName, "\\");
    strcat( szFilePathName, szFileName );
    
    lpPEFile = PE_LoadFileIntoMemory(szFilePathName, &lpDOSHeader, &lpCOFFHeader, &lpOptionalHeaderSTD, &lpOptionalHeaderNT);
    if ( lpPEFile == NULL )
        return NULL;

    DEBUG_PRINT_OBJECT2("Base of Code %X Size %ld", lpOptionalHeaderSTD->BaseOfCode, lpOptionalHeaderSTD->SizeOfCode );
    DEBUG_PRINT_OBJECT2("Base of Data %X Address of Entry %X", lpOptionalHeaderSTD->BaseOfData, lpOptionalHeaderSTD->AddressOfEntryPoint);
    
    if ( lpOptionalHeaderSTD->Magic == PE32_MAGIC )
    {
        dwSizeOfImage = ((LPPE_OPTIONAL_HEADER_NT_PE32)lpOptionalHeaderNT)->SizeOfImage;
        dwNumberOfRVAandSizes=((LPPE_OPTIONAL_HEADER_NT_PE32)lpOptionalHeaderNT)->NumberOfRVAandSizes;
        lpDataDirectory = (LP_IMAGE_DATA_DIRECTORY) ((UINT32)lpOptionalHeaderNT + sizeof(PE_OPTIONAL_HEADER_NT_PE32));
    }
    else
    {
        dwSizeOfImage = ((LPPE_OPTIONAL_HEADER_NT_PE32_PLUS)lpOptionalHeaderNT)->SizeOfImage;
        dwNumberOfRVAandSizes=((LPPE_OPTIONAL_HEADER_NT_PE32_PLUS)lpOptionalHeaderNT)->NumberOfRVAandSizes;
        lpDataDirectory = (LP_IMAGE_DATA_DIRECTORY) ((UINT32)lpOptionalHeaderNT + sizeof(PE_OPTIONAL_HEADER_NT_PE32_PLUS));
    }
    lpPI->lpPreferedLoadAddress=(void *)((LPPE_OPTIONAL_HEADER_NT_PE32)lpOptionalHeaderNT)->ImageBase;
    
    DEBUG_PRINT_OBJECT2("Image Base %X Size Of Image %X", lpPI->lpPreferedLoadAddress, ((LPPE_OPTIONAL_HEADER_NT_PE32)lpOptionalHeaderNT)->SizeOfImage );
    
    lpImage = (BYTE *) malloc( dwSizeOfImage );
    if ( lpImage == NULL )
    {
        DEBUG_PRINT_INFO("Memory allocation failed for image creation");
        free( lpImage );
        SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        return NULL;
    }
    
    lpPI->lpPEFile = lpPEFile;
    lpPI->lpImage = lpImage;
    lpPI->dwSizeOfImage = dwSizeOfImage;
    if ( lpOptionalHeaderSTD )
        lpPI->AddressOfEntryPoint = lpOptionalHeaderSTD->AddressOfEntryPoint;
    
    for(i=0;i<dwNumberOfRVAandSizes;i++)
    {
        DEBUG_PRINT_OBJECT3("%X RVA %X  Size %X",lpDataDirectory, lpDataDirectory->RVA, lpDataDirectory->Size );
        lpDataDirectory = (LP_IMAGE_DATA_DIRECTORY ) ((UINT32)lpDataDirectory + sizeof(LP_IMAGE_DATA_DIRECTORY));
    }
    lpSectionTable = (LPPE_SECTION_TABLE) ((UINT32)lpCOFFHeader+ sizeof(COFF_FILE_HEADER)+ lpCOFFHeader->SizeOfOptionalHeader);
    for (i=0;i<lpCOFFHeader->NoOfSections;i++)
    {
        char Name[9];
        strncpy(Name,lpSectionTable->Name,8);
        Name[9]=0;
        #ifdef _DEBUG_
            dbgprintf("\n [%s] %X  %ld  %ld  %X  %X",Name, lpSectionTable->VirtualAddress, lpSectionTable->VirtualSize, lpSectionTable->SizeOfRawData, lpSectionTable->PointerToRelocations, lpSectionTable->Characteristics);
        #endif
        
        //copy the section table to image address and set 0 if it is smaller
        memcpy( (void *)((UINT32)lpImage + lpSectionTable->VirtualAddress) , (void *)((UINT32)lpPEFile + lpSectionTable->PointerToRawData) , lpSectionTable->VirtualSize );
        if ( lpSectionTable->VirtualSize > lpSectionTable->SizeOfRawData )
            memset( (void *)((UINT32)lpImage + lpSectionTable->VirtualAddress + lpSectionTable->SizeOfRawData), 0, lpSectionTable->VirtualSize-lpSectionTable->SizeOfRawData);
        
        if ( lpSectionTable->PointerToRelocations )
            DEBUG_PRINT_INFO("WARNING - Section Relocations Found. But No action taken");
        
        if ( strcmp(Name, ".edata") == 0 )
            PE_ProcessExportSection(szFileName, lpPI, lpSectionTable );
        
        if ( strcmp(Name, ".idata") == 0 )
            PE_ProcessImportSection(szFilePath, lpPI, lpSectionTable);
        
        if ( strcmp(Name, ".reloc") == 0 )
            lpPI->lpRelocationSection=lpSectionTable;
        
        //next section table
        lpSectionTable = (LPPE_SECTION_TABLE) ((UINT32)lpSectionTable + sizeof(PE_SECTION_TABLE));
    }
    
    DEBUG_PRINT_OBJECT1("Returns %x", lpPI);
    return lpPI;
}
void PE_ProcessImportSection(LPCSTR szFilePath, LPPE_IMAGE_LOAD_INFO lpPI, LPPE_SECTION_TABLE lpSectionTable)
{
    LPPE_IMAGE_LOAD_INFO pImportDLLImage;
    char szFilePathName[_MAX_PATH];
    
    
    ALLOC_AND_CHECK(pImportDLLImage, LPPE_IMAGE_LOAD_INFO, sizeof(PE_IMAGE_LOAD_INFO), );
    
    lpPI->lpImportDirTable=(LPPE_IMPORT_DIR_TABLE) ((UINT32)lpPI->lpPEFile + lpSectionTable->PointerToRawData);
    DEBUG_PRINT_OBJECT3("(UINT32)lpImage+lpPI->lpImportDirTable->NameRVA %X %X %X",(UINT32)lpPI->lpImage+lpPI->lpImportDirTable->NameRVA,(UINT32)lpPI->lpImage, lpPI->lpImportDirTable->NameRVA);
    if ( lpPI->lpImportDirTable->NameRVA != 0 )
    {
        pImportDLLImage->lpExportDirTableList=lpPI->lpExportDirTableList;
        DEBUG_PRINT_OBJECT1("pImportDLLImage->lpExportDirTableList %X", pImportDLLImage->lpExportDirTableList);
        
        PE_LoadFile(szFilePath, (char * )lpPI->lpImage+lpPI->lpImportDirTable->NameRVA,  pImportDLLImage);       //recursive call
        PE_ResolveImports(lpPI->lpImage, 1, lpPI->lpImportDirTable, pImportDLLImage->lpImage,lpPI->lpExportDirTableList );
    }
    
}
void PE_ProcessExportSection(LPCSTR szName, LPPE_IMAGE_LOAD_INFO lpPI, LPPE_SECTION_TABLE lpSectionTable)
{
    LPPE_EXPORTINFO lpExportInfo;
    char szFileName[_MAX_DIR];
    strcpy(szFileName, szName);
    
    lpPI->lpExportDirTable=(LPPE_EXPORT_DIR_TABLE) ((UINT32)lpPI->lpPEFile + lpSectionTable->PointerToRawData);
    
    DEBUG_PRINT_OBJECT2("Exporting symbols from [%s] to list %X", szFileName, lpPI->lpExportDirTableList);
    ALLOC_AND_CHECK(lpExportInfo, LPPE_EXPORTINFO, sizeof(PE_EXPORTINFO), );
    
    lpExportInfo->lpExportDirTab = lpPI->lpExportDirTable;
    lpExportInfo->lpImage = lpPI->lpImage;
    DEBUG_PRINT_OBJECT1("Adding ExportInfo to list - lpPI->lpExportDirTableList %X", lpPI->lpExportDirTableList);
    List_Add( &lpPI->lpExportDirTableList, szFileName, lpExportInfo );
}
void PE_ResolveImports(void * lpImage, BYTE PE32, LPPE_IMPORT_DIR_TABLE pImportDirTab, void * lpEImage, LPLISTNODE lpPEExportDirTableList)
{
    void * pImportLookupTab; 
    void * pImportAddressTab;
    int i;
    DEBUG_PRINT_OBJECT2("Search for import from %s Export List %X",(UINT32)lpImage+pImportDirTab->NameRVA, lpPEExportDirTableList);
    LPPE_EXPORTINFO PEExpoInfo = (LPPE_EXPORTINFO) List_GetData(lpPEExportDirTableList, (void *)((UINT32)lpImage+pImportDirTab->NameRVA), -1 );
    if ( PEExpoInfo == NULL )
    {
        DEBUG_PRINT_INFO("PEExpoInfo - NULL");
        return;
    }
    LPPE_EXPORT_DIR_TABLE pExportDirTab = PEExpoInfo->lpExportDirTab ;
    if ( pExportDirTab == NULL )
    {
        DEBUG_PRINT_INFO("pExportDirTab  - NULL");
        return;
    }
    
    while (1)
    {
        if ( pImportDirTab->ImportLookUpRVA==0 && pImportDirTab->NameRVA==0 )
            return;
        pImportLookupTab = (void *) ((UINT32)lpImage+pImportDirTab->ImportLookUpRVA);
        pImportAddressTab =(void *) ((UINT32)lpImage+pImportDirTab->ImportAddressTableRVA );
        for(i=0;;i++)
        {
            if ( PE32 )
            {
                UINT32 pImportLTEntry = ((UINT32 *) pImportLookupTab)[i];
                DEBUG_PRINT_OBJECT2("LookupEntry %X Address %X",pImportLTEntry, ((UINT32 *)pImportAddressTab)[i] );
                if ( pImportLTEntry == 0 )
                    break;
                if ( pImportLTEntry & 0x80000000 )
                {
                    DEBUG_PRINT_OBJECT1("Import By Ordinal %X", pImportLTEntry & 0x7FFFFFFF);
                    void * lpAddress=PE_GetExportSymbolByOrdinal( lpEImage, pExportDirTab, pImportLTEntry & 0x7FFFFFFF );
                    if ( lpAddress )
                        ((UINT32 *)pImportAddressTab)[i]=(UINT32)lpAddress;
                }
                else
                {
                    //UINT16 Hint = *((UINT16 *)((UINT32)lpImage + (pImportLTEntry & 0x7FFFFFFF) ));
                    char * szImportFnName=(char *)((UINT32)lpImage + (pImportLTEntry & 0x7FFFFFFF)+ 2 );
                    void * lpAddress=PE_GetExportSymbolByName( lpEImage, pExportDirTab, szImportFnName );
                    if ( lpAddress )
                        ((UINT32 *)pImportAddressTab)[i]=(UINT32)lpAddress;
                }
            }
            else
            {
                DEBUG_PRINT_INFO("Error : PE32+ import not implemented");
            }
        }
        pImportDirTab= (LPPE_IMPORT_DIR_TABLE )((UINT32)pImportDirTab + sizeof(PE_IMPORT_DIR_TABLE));
    }
}
void * PE_GetExportSymbolByOrdinal(void *lpImage, LPPE_EXPORT_DIR_TABLE pExportDirTab, UINT16 wOrdinal)
{
    UINT32 * pExportAddressTable;
    DEBUG_PRINT_OBJECT2("%d %d ",wOrdinal, pExportDirTab->OrdinalBase );
    //wOrdinal-=pExportDirTab->OrdinalBase;
    if ( pExportDirTab == NULL )
        return NULL;
    if ( wOrdinal > pExportDirTab->AddressTableEntries )
        return NULL;
    
    pExportAddressTable = (UINT32 *)((UINT32)lpImage + pExportDirTab->ExportAddressTableRVA );
    
    //**note - Forwarder RVA is not checked 
    return (void *)((UINT32)lpImage + (UINT32)pExportAddressTable[wOrdinal]);
}
void * PE_GetExportSymbolByName(void *lpImage, LPPE_EXPORT_DIR_TABLE pExportDirTab, char * szSymbolName)
{
    int i;
    char * szNamePointer;
    UINT32 * pNamePointerTable;
    UINT16 * pOrdinalTable;
    
    if ( pExportDirTab == NULL )
    {
        DEBUG_PRINT_INFO("pExportDirTab is NULL");
        return NULL;
    }
    DEBUG_PRINT_OBJECT1("SymbolName Search for %s", szSymbolName );
    
    pNamePointerTable = (UINT32 *)((UINT32)lpImage + pExportDirTab->NamePointerRVA);
    pOrdinalTable=(UINT16 *)((UINT32)lpImage + pExportDirTab->OrdinalTableRVA );
    szNamePointer=(char *) ( (UINT32)lpImage + pNamePointerTable[0] );
    for(i=0;i<pExportDirTab->NumberOfNamePointers;i++,szNamePointer= (char *) ( (UINT32)lpImage + pNamePointerTable[i] ) )
    {
        if ( strcmp(szSymbolName,szNamePointer) == 0 )
        {
            UINT16 Ordinal = pOrdinalTable[i];
            return PE_GetExportSymbolByOrdinal(lpImage, pExportDirTab,Ordinal);
        }
    }
    return NULL;
}

void PE_RelocateImage(INT32 Delta, void * lpPEFile, void * lpImage, LPPE_SECTION_TABLE lpRelocationSection)
{
    LPPE_RELOC_FIXUP_BLOCK pRelocFixupBlock;
    UINT32 i,j;
    DEBUG_PRINT_INFO("Called");
    if ( lpRelocationSection == NULL )
        return;
    pRelocFixupBlock =(LPPE_RELOC_FIXUP_BLOCK) ( (UINT32)lpPEFile + lpRelocationSection->PointerToRawData);
    if ( pRelocFixupBlock->BlockSize == 0 )
        return;
    DEBUG_PRINT_OBJECT2("Relocate %X %X",pRelocFixupBlock->PageRVA, pRelocFixupBlock->BlockSize);
    
    for(i=0;i< pRelocFixupBlock->BlockSize!=0 ;i++)
    {
        int dwREntries=pRelocFixupBlock->BlockSize-sizeof(PE_RELOC_FIXUP_BLOCK);
        j=0;
        
        while( dwREntries > 0)
        {
            UINT16 wREntry=((UINT16*)((UINT32)pRelocFixupBlock + sizeof(PE_RELOC_FIXUP_BLOCK ) ))[j];
            
            BYTE FixupType = wREntry>>12;
            UINT32 ImgOffset = pRelocFixupBlock->PageRVA + (wREntry&0x0FFF);
            
            switch ( FixupType )
            {
                case IMAGE_REL_BASED_HIGHLOW:
                    *( (UINT32 *)( (UINT32)lpImage + ImgOffset ) ) += Delta;
                    break;
                case IMAGE_REL_BASED_ABSOLUTE:
                    break;
                default:
                    DEBUG_PRINT_OBJECT1("Unsupported FixupType %d != IMAGE_REL_BASED_ABSOLUTE AND !=IMAGE_REL_BASED_HIGHLOW", FixupType);
            }
            
            dwREntries -= sizeof(UINT16);
            
            j++;
        }
        pRelocFixupBlock = (LPPE_RELOC_FIXUP_BLOCK )( (UINT32)pRelocFixupBlock + pRelocFixupBlock->BlockSize ); ;
        
    }
    DEBUG_PRINT_INFO("Return");
}
