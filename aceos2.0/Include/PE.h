/*
	PE.h (Portable Executable file related operations)
            Created on 6-Jun-2003
            Modified on 3-Jul-2005 - No logic change, jist splitted large functions into smaller ones
        samueldotj@gmail.com

*/
#ifndef __PE__H
#define __PE__H

#include "TypeDefs.h"
#ifdef __cplusplus
	extern "C" {
#endif
            
#define PE_ERR_FILE_OPEN        1
#define PE_ERR_FILE_READ        2
#define PE_ERR_MEMORY           3
#define PE_ERR_SIGNATURE        4

typedef struct                 // DOS .EXE header
{      
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    UINT32  e_lfanew;                    // File address of new exe header
}__attribute__ ((packed)) _IMAGE_DOS_HEADER;

typedef _IMAGE_DOS_HEADER * LP_IMAGE_DOS_HEADER;

#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
//#define IMAGE_OS2_SIGNATURE                 0x4E45      // NE
//#define IMAGE_OS2_SIGNATURE_LE              0x4C45      // LE
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00

typedef struct 
{
    UINT16 Machine;
    UINT16 NoOfSections;
    UINT32 TimeStamp;
    UINT32 PointerToSymbolTable;
    UINT32 NoOfSymbols;
    UINT16 SizeOfOptionalHeader;
    UINT16 Characterstics;
}__attribute__ ((packed)) COFF_FILE_HEADER;

typedef COFF_FILE_HEADER * LPCOFF_FILE_HEADER;

#define IMAGE_FILE_MACHINE_UNKNOWN 0x0 //Contents assumed to be applicable to anymachine type.
#define IMAGE_FILE_MACHINE_ALPHA 0x184 //Alpha AXP™.
#define IMAGE_FILE_MACHINE_ARM 0x1c0
#define IMAGE_FILE_MACHINE_ALPHA64 0x284 //Alpha AXP™ 64-bit.
#define IMAGE_FILE_MACHINE_I386 0x14c //Intel 386 or later, and compatible processors.
#define IMAGE_FILE_MACHINE_IA64 0x200 //Intel IA64™
#define IMAGE_FILE_MACHINE_M68K 0x268 //Motorola 68000 series.
#define IMAGE_FILE_MACHINE_MIPS16 0x266
#define IMAGE_FILE_MACHINE_MIPSFPU 0x366 //MIPS with FPU
#define IMAGE_FILE_MACHINE_MIPSFPU16 0x466 //MIPS16 with FPU
#define IMAGE_FILE_MACHINE_POWERPC 0x1f0 //Power PC, little endian.
#define IMAGE_FILE_MACHINE_R3000 0x162
#define IMAGE_FILE_MACHINE_R4000 0x166 //MIPS® little endian.
#define IMAGE_FILE_MACHINE_R10000 0x168
#define IMAGE_FILE_MACHINE_SH3 0x1a2 //Hitachi SH3
#define IMAGE_FILE_MACHINE_SH4 0x1a6 //Hitachi SH4
#define IMAGE_FILE_MACHINE_THUMB 0x1c2
	
	
#define IMAGE_FILE_RELOCS_STRIPPED 0x0001 
#define IMAGE_FILE_EXECUTABLE_IMAGE 0x0002 
#define IMAGE_FILE_LINE_NUMS_STRIPPED 0x0004 
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED 0x0008 
#define IMAGE_FILE_AGGRESSIVE_WS_TRIM 0x0010
#define IMAGE_FILE_LARGE_ADDRESS_AWARE 0x0020
#define IMAGE_FILE_16BIT_MACHINE 0x0040 
#define IMAGE_FILE_BYTES_REVERSED_LO 0x0080 
#define IMAGE_FILE_32BIT_MACHINE 0x0100
#define IMAGE_FILE_DEBUG_STRIPPED 0x0200
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP 0x0400 
#define IMAGE_FILE_SYSTEM 0x1000
#define IMAGE_FILE_DLL 0x2000
#define IMAGE_FILE_UP_SYSTEM_ONLY 0x4000 
#define IMAGE_FILE_BYTES_REVERSED_HI 0x8000

typedef struct 
{
	UINT16 Magic;
	BYTE MajorLinkerVersion;
	BYTE MinorLinkerVersion;
	UINT32 SizeOfCode;
	UINT32 SizeOfInitializedData;
	UINT32 SizeOfUnInitializedData;
	UINT32 AddressOfEntryPoint;
	UINT32 BaseOfCode;
	UINT32 BaseOfData;
}__attribute__ ((packed)) PE_OPTIONAL_HEADER_STD;

typedef PE_OPTIONAL_HEADER_STD * LPPE_OPTIONAL_HEADER_STD;

#define PE32_MAGIC 0x10B
#define PE32_PLUS_MAGIC 0x20B

typedef struct 
{
	UINT32 ImageBase;
	UINT32 SectionAlignment;
	UINT32 FileAlignment;
	UINT16 MajorOSVersion;
	UINT16 MinorOSVersion;
	UINT16 MajorImageVersion;
	UINT16 MinorImageVersion;
	UINT16 MajorSubsystemVersion;
	UINT16 MinorSubsystemVersion;
	UINT32 Reserved;
	UINT32 SizeOfImage;
	UINT32 SizeOfHeaders;
	UINT32 CheckSum;
	UINT16 Subsystem;
	UINT16 DLLCharacterstics;
	UINT32 SizeOfStackReserve;
	UINT32 SizeOfStackCommit;
	UINT32 SizeOfHeapReserve;
	UINT32 SizeOfHeapCommit;
	UINT32 LoaderFlags;
	UINT32 NumberOfRVAandSizes;
}__attribute__ ((packed)) PE_OPTIONAL_HEADER_NT_PE32;

typedef PE_OPTIONAL_HEADER_NT_PE32 * LPPE_OPTIONAL_HEADER_NT_PE32;

typedef struct 
{
	UINT64 ImageBase;
	UINT32 SectionAlignment;
	UINT32 FileAlignment;
	UINT16 MajorOSVersion;
	UINT16 MinorOSVersion;
	UINT16 MajorImageVersion;
	UINT16 MinorImageVersion;
	UINT16 MajorSubsystemVersion;
	UINT16 MinorSubsystemVersion;
	UINT32 Reserved;
	UINT32 SizeOfImage;
	UINT32 SizeOfHeaders;
	UINT32 CheckSum;
	UINT16 Subsystem;
	UINT16 DLLCharacterstics;
	UINT64 SizeOfStackReserve;
	UINT64 SizeOfStackCommit;
	UINT64 SizeOfHeapReserve;
	UINT64 SizeOfHeapCommit;
	UINT32 LoaderFlags;
	UINT32 NumberOfRVAandSizes;
}__attribute__ ((packed)) PE_OPTIONAL_HEADER_NT_PE32_PLUS;

typedef PE_OPTIONAL_HEADER_NT_PE32_PLUS * LPPE_OPTIONAL_HEADER_NT_PE32_PLUS;

typedef struct 
{
    DWORD RVA;
    DWORD Size;
}__attribute__ ((packed)) _IMAGE_DATA_DIRECTORY;

typedef _IMAGE_DATA_DIRECTORY * LP_IMAGE_DATA_DIRECTORY;



typedef struct 
{
    char Name[8];
    UINT32 VirtualSize;
    UINT32 VirtualAddress;
    UINT32 SizeOfRawData;
    UINT32 PointerToRawData;
    UINT32 PointerToRelocations;
    UINT32 PointerToLineNumbers;
    UINT16 NumberOfRelocations;
    UINT16 NumberOfLineNumbers;
    UINT32 Characteristics;
}__attribute__ ((packed)) PE_SECTION_TABLE;

typedef PE_SECTION_TABLE * LPPE_SECTION_TABLE;

typedef struct 
{
    UINT32 ExportFlags;
    UINT32 TimeStamp;
    UINT16 MajorVersion;
    UINT16 MinorVersion;
    UINT32 NameRVA;
    UINT32 OrdinalBase;
    UINT32 AddressTableEntries;
    UINT32 NumberOfNamePointers;
    UINT32 ExportAddressTableRVA;
    UINT32 NamePointerRVA;
    UINT32 OrdinalTableRVA;
}__attribute__ ((packed)) PE_EXPORT_DIR_TABLE;

typedef PE_EXPORT_DIR_TABLE * LPPE_EXPORT_DIR_TABLE;

typedef struct 
{
    UINT32 ExportRVA;
    UINT32 ForwarderRVA;
}__attribute__ ((packed)) PE_EXPORT_ADDRESS_TABLE;

typedef PE_EXPORT_ADDRESS_TABLE * LPPE_EXPORT_ADDRESS_TABLE;

typedef struct 
{
    UINT32 ImportLookUpRVA;
    UINT32 TimeStamp;
    UINT32 ForwarderChain;
    UINT32 NameRVA;
    UINT32 ImportAddressTableRVA;
}__attribute__ ((packed)) PE_IMPORT_DIR_TABLE;

typedef PE_IMPORT_DIR_TABLE * LPPE_IMPORT_DIR_TABLE;

#define IMAGE_SCN_TYPE_REG 0x00000000 //Reserved for future use.
#define IMAGE_SCN_TYPE_DSECT 0x00000001 //Reserved for future use.
#define IMAGE_SCN_TYPE_NOLOAD 0x00000002 //Reserved for future use.
#define IMAGE_SCN_TYPE_GROUP 0x00000004 //Reserved for future use.
#define IMAGE_SCN_TYPE_NO_PAD 0x00000008 //Section should not be padded to next boundary. This is obsolete andreplaced by IMAGE_SCN_ALIGN_1BYTES. Thisis valid for object files only.
#define IMAGE_SCN_TYPE_COPY 0x00000010 //Reserved for future use.
#define IMAGE_SCN_CNT_CODE 0x00000020 //Section contains executable code.
#define IMAGE_SCN_CNT_INITIALIZED_DATA 0x00000040 //Section contains initialized data.
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA 0x00000080 //Section contains uninitialized data.
#define IMAGE_SCN_LNK_OTHER 0x00000100 //Reserved for future use.
#define IMAGE_SCN_LNK_INFO 0x00000200 //Section contains comments or other information. The .drectve section has this type. This is valid for object files only. 
#define IMAGE_SCN_TYPE_OVER 0x00000400 //Reserved for future use.
#define IMAGE_SCN_LNK_REMOVE 0x00000800 //Section will not become part of the image. This is valid for object files only.
#define IMAGE_SCN_LNK_COMDAT 0x00001000 //Section contains COMDAT data. 
#define IMAGE_SCN_MEM_FARDATA 0x00008000 //Reserved for future use.
#define IMAGE_SCN_MEM_PURGEABLE 0x00020000 //Reserved for future use.
#define IMAGE_SCN_MEM_16BIT 0x00020000 //Reserved for future use.
#define IMAGE_SCN_MEM_LOCKED 0x00040000 //Reserved for future use.
#define IMAGE_SCN_MEM_PRELOAD 0x00080000 //Reserved for future use.
#define IMAGE_SCN_ALIGN_1BYTES 0x00100000 //Align data on a 1-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_2BYTES 0x00200000 //Align data on a 2-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_4BYTES 0x00300000 //Align data on a 4-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_8BYTES 0x00400000 //Align data on a 8-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_16BYTES 0x00500000 //Align data on a 16-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_32BYTES 0x00600000 //Align data on a 32-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_64BYTES 0x00700000 //Align data on a 64-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_128BYTES 0x00800000 //Align data on a 128-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_256BYTES 0x00900000 //Align data on a 256-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_512BYTES 0x00A00000 //Align data on a 512-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_1024BYTES 0x00B00000 //Align data on a 1024-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_2048BYTES 0x00C00000 //Align data on a 2048-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_4096BYTES 0x00D00000 //Align data on a 4096-byte boundary. This is valid for object files only.
#define IMAGE_SCN_ALIGN_8192BYTES 0x00E00000 //Align data on a 8192-byte boundary. This is valid for object files only.
#define IMAGE_SCN_LNK_NRELOC_OVFL 0x01000000 //Section contains extended relocations.
#define IMAGE_SCN_MEM_DISCARDABLE 0x02000000 //Section can be discarded as needed.
#define IMAGE_SCN_MEM_NOT_CACHED 0x04000000 //Section cannot be cached.
#define IMAGE_SCN_MEM_NOT_PAGED 0x08000000 //Section is not pageable.
#define IMAGE_SCN_MEM_SHARED 0x10000000 //Section can be shared in memory.
#define IMAGE_SCN_MEM_EXECUTE 0x20000000 //Section can be executed as code.
#define IMAGE_SCN_MEM_READ 0x40000000 //Section can be read.
#define IMAGE_SCN_MEM_WRITE 0x80000000 //Section can be written to.
typedef struct 
{
    UINT32 PageRVA;
    UINT32 BlockSize;
}__attribute__ ((packed)) PE_RELOC_FIXUP_BLOCK;
typedef PE_RELOC_FIXUP_BLOCK * LPPE_RELOC_FIXUP_BLOCK;


typedef struct 
{
    UINT32 VirtualAddress;
    UINT32 SymbolTableIndex;
    UINT16 Type;
}__attribute__ ((packed)) COFF_RELOCATIONS;
typedef COFF_RELOCATIONS * LPCOFF_RELOCATIONS;
#define IMAGE_REL_I386_ABSOLUTE 0x0000 //This relocation is ignored.
#define IMAGE_REL_I386_DIR16 0x0001 //Not supported.
#define IMAGE_REL_I386_REL16 0x0002 //Not supported.
#define IMAGE_REL_I386_DIR32 0x0006 //The target’s 32-bit virtual address.
#define IMAGE_REL_I386_DIR32NB 0x0007 //The target’s 32-bit relative virtual address.
#define IMAGE_REL_I386_SEG12 0x0009 //Not supported.
#define IMAGE_REL_I386_SECTION 0x000A //The 16-bit-section index of the section containing the target. This is used to support debugging information.
#define IMAGE_REL_I386_SECREL 0x000B //The 32-bit offset of the target from the beginning of its section. This is used to support debugging information as well as static thread local storage.
#define IMAGE_REL_I386_REL32 0x0014 //The 32-bit relative displacement to the target. This supports the x86 relative branch and call instructions.

typedef struct 
{
    UINT32 Type;
    UINT16 LineNumber;
    union 
    {
        UINT32 SymbolTableIndex; //Used when Linenumber is 0: index to symbol table entry for a function. This format is used to indicate the function that a group of line-number records refer to.
        UINT32 VirtualAddress; //Used when Linenumber is non-zero: relative virtual address of the executable code that corresponds to the source line indicated. In an object file, this contains the virtual address within the section.
    };
}__attribute__ ((packed)) COFF_LINE_NUMBERS;
typedef COFF_LINE_NUMBERS * LPCOFF_LINE_NUMBERS;


typedef struct 
{
    void * lpPEFile;            
    void * lpImage;             
    
    UINT32 dwSizeOfImage;         
    
    LPPE_SECTION_TABLE lpRelocationSection; 
    LPPE_EXPORT_DIR_TABLE lpExportDirTable;
    LPPE_IMPORT_DIR_TABLE lpImportDirTable;
    
    LPLISTNODE lpExportDirTableList;     //this should be initalized outside the PE_LoadFile()
    
    void * lpPreferedLoadAddress;       
    void * lpLoadedAddress;
    
    UINT32 AddressOfEntryPoint;
}__attribute__ ((packed)) PE_IMAGE_LOAD_INFO;
typedef PE_IMAGE_LOAD_INFO * LPPE_IMAGE_LOAD_INFO;


typedef struct 
{
    LPPE_EXPORT_DIR_TABLE lpExportDirTab;
    void * lpImage;
}PE_EXPORTINFO;

typedef PE_EXPORTINFO * LPPE_EXPORTINFO;

//internal functions
void * PE_LoadFileIntoMemory(LPCSTR szFilePath, LP_IMAGE_DOS_HEADER *lipDOSHeader, LPCOFF_FILE_HEADER *lipCOFFHeader, LPPE_OPTIONAL_HEADER_STD *lipOptionalHeaderSTD, void **lipOptionalHeaderNT);
void PE_ProcessExportSection(LPCSTR szName, LPPE_IMAGE_LOAD_INFO lpPI, LPPE_SECTION_TABLE lpSectionTable);
void PE_ProcessImportSection(LPCSTR szFilePath, LPPE_IMAGE_LOAD_INFO lpPI, LPPE_SECTION_TABLE lpSectionTable);
void * PE_GetExportSymbolByOrdinal(void *lpImage, LPPE_EXPORT_DIR_TABLE  lpExportDirTab, UINT16 wOrdinal);
void * PE_GetExportSymbolByName(void *lpImage, LPPE_EXPORT_DIR_TABLE lpExportDirTab, char * szSymbolName);
        

//exported functions
LPPE_IMAGE_LOAD_INFO PE_LoadFile(LPCSTR szFilePath, LPCSTR szFileName, LPPE_IMAGE_LOAD_INFO lpPI);
void PE_ResolveImports(void * lpImage, BYTE PE32, LPPE_IMPORT_DIR_TABLE lpImportDirTab, void * lpEImage, LPLISTNODE lpPEExportDirTableList);
void PE_RelocateImage(INT32 Delta, void * lpPEFile, void * lpImage, LPPE_SECTION_TABLE lpRelocationSection);

#ifdef __cplusplus
    }
#endif

#endif
