#ifndef __MEMORY_H
#define __MEMORY_H

#include <TypeDefs.h>

#define VASM_PAGE_SIZE          4096
#define VASM_PAGE_BITMAP_SIZE   32      //32 KB for 4GB virtual Address Space

#define PAGE_ALIGN(addr)    ((addr) & -VASM_PAGE_SIZE)
#define PAGE_ALIGN_UP(addr)    (((addr) + VASM_PAGE_SIZE - 1) & -VASM_PAGE_SIZE)

#define PAGE_USED 1
#define PAGE_FREE 0

#warning check the following and change it in the Kernel32.dll
#define VASM_MEM_USER       0x00000004
#define PAGE_READWRITE      0x00000002


#define MEM_COMMIT      1
#define MEM_RESERVE     2
#define MEM_RESET       4
#define MEM_TOP_DOWN    8
#define MEM_DECOMMIT    16
#define MEM_RELEASE     32


#define BITS_IN_UINT32          (32)

#define PAGE_TABLE_SIZE         1024*4
#define PAGE_DIRECTORY_SIZE     1024*4

/*
These are macros that used during the initialization of memory (Phase I).
*/
#define kp_RESERVED_START_PAGE          0
#define kp_RESERVED_TOTAL_PAGES         ( 0x100000 / VASM_PAGE_SIZE )
#define kp_RESERVED_ADDRESS             0                       // These are reserved 1 MB BIOS Memory

//////------------REMOVE THIS ONE-------
#define kp_K_TMP_MAP_StartPage 	        256			
#define kp_K_TMP_MAP_TotalPages 	2
#define kp_K_TMP_MAP_Address 		0x100000                //Used in Phase II Initialization
/////------------------------------------

#define kp_KERNEL_START_PAGE 	        0xC0000 		//Kernel Code, data - (3*256*1024)
#define kp_KERNEL_TOTAL_PAGES 	        ( (dwKernelSize / VASM_PAGE_SIZE ) + 1 )                   
#define kp_KERNEL_ADDRESS 	        0x100000

#define kp_KERNEL_STACK_START_PAGE 	( kp_KERNEL_START_PAGE+kp_KERNEL_TOTAL_PAGES ) 	        //Kernel stack
#define kp_KERNEL_STACK_TOTAL_PAGES 	5                      // 5 Pages (4096*5 enough ring 0 stack space)
#define kp_KERNEL_STACK_ADDRESS 	(kp_KERNEL_ADDRESS + (kp_KERNEL_TOTAL_PAGES * VASM_PAGE_SIZE)) //0x140000                 // predetermined stack address

#define kp_KERNEL_MEM_START 	        (kp_KERNEL_STACK_START_PAGE+kp_KERNEL_STACK_TOTAL_PAGES)	
#define kp_KERNEL_MEM_TOTAL_PAGES	( ( lpWorkingMemoryTop - lpKernelWorkingMemory ) / VASM_PAGE_SIZE  + 1)
#define kp_KERNEL_MEM_ADDRESS           ((UINT32)lpKernelWorkingMemory)                         //physical address
#define kp_KERNEL_MEM_LINEAR_ADDRESS    ((UINT32)kp_KERNEL_MEM_START * VASM_PAGE_SIZE )              //Linear address

#define kp_TRANSLATION_PT_START 	((1024*1024)-1024)      //(kp_Kernel_Code_StartPage-1024)
#define kp_TRANSLATION_PT_TOTAL 	1024                    // 1 page table

#define kp_POINTER_TPT_START     	(kp_TRANSLATION_PT_START-1)
#define kp_POINTER_TPT_TOTAL     	1                       // 1 page 

#define VASM_TPT    ((void *)((UINT32)kp_TRANSLATION_PT_START*VASM_PAGE_SIZE))
#define VASM_LPTPT  ((void *)((UINT32)(kp_POINTER_TPT_START)*VASM_PAGE_SIZE))


#ifdef __cplusplus
    extern "C" {
#endif
//extern void * mm_IDTAddress;
extern void * mm_GDTBitmap;

extern void * mm_KernelPageDirectory;
extern void * mm_KernelPageBitmap;

extern UINT32 mm_PhysicalTotal;
extern UINT32 mm_PhysicalAvailable;
extern UINT16 mm_PhysicalBitmapSize;
extern void * mm_PhysicalBitmap;

UINT32 VASM_GetPageSize();
        
UINT32 DetectMemory();

void * InitMemManPhaseI();
void InitMemManPhaseII();

void INIT_SetPageDirectoryEntry(int PDENo, void * lpPageDirectory, void * lpPTAddress, BYTE Protection );
void SetPageDirectoryEntry(int PDENo, void * lpPageDirectory, void * lpPageTableList, void * lpPTAddress, BYTE Protection );
       
UINT32 GetPageDirectoryEntry(int PDENo, void * lpPageDirectory);

int INIT_SetPageEntry(UINT32 PageNo, void * lpHardwareAddress, BYTE Protection);        
int SetPageEntry(UINT32 PageNo, void * lpPageDirectory, UINT32 * TranslationPageTableList, void * lpHardwareAddress, BYTE Protection);
UINT32 GetPageEntry(UINT32 PageNo, UINT32 * TranslationPageTableList);

UINT32 VirtualAlloc(UINT32 StartingAddress, UINT32 Size, UINT16 AllocationType, UINT16 ProtectionType );
UINT32 VirtualAllocEx(UINT32 TaskID, UINT32 StartingAddress, UINT32 Size, UINT16 AllocationType, UINT16 ProtectionType );
UINT32 VirtualFree(UINT32 StartingAddress, UINT32 Size, DWORD FreeType);
UINT32 VirtualFreeEx(UINT32 TaskID, UINT32 StartingAddress, UINT32 Size, DWORD FreeType);

void * KernelMap(void * lpStartingAddress, UINT32 Size, void * lpHardwareAddress);
void * KernelUnMap(void * lpStartingAddress, UINT32 Size);

UINT32 mm_FindInBitmap(BYTE * lpBitmap, BYTE FindWhat, UINT32 StartIndex, UINT32 Size, UINT32 Limit);
UINT32 mm_FindFirstInBitmap(UINT32 * Bitmap, BYTE FindWhat, UINT32 StartIndex, UINT32 Limit );
UINT32 mm_FindContinousInBitmap(BYTE * lpBitmap, BYTE FindWhat, UINT32 StartIndex, UINT32 Size, UINT32 Limit);
void mm_C_S_BitsInBitmap(BYTE * Bitmap, BYTE ClearOrSet, UINT32 StartIndex, UINT32 Size);

void VASM_InitLowPhysicalPageStructures();
UINT32 VASM_AllocateLowPhysicalPages(UINT32 dwStartPage, UINT32 dwLimitPage, UINT32 dwSize);
UINT32 VASM_GetFirstLowPhysicalPage(UINT32 dwStartPage, UINT32 dwLimitPage);
UINT32 VASM_FreeLowPhysicalPages(UINT32 dwStartPage, UINT32 dwSize);


//------------------------------------Descriptor Management-----------------------
struct _GDTReg
{
    UINT16 wLimit;
    UINT32 dwBase;
}__attribute__ ((packed));

struct _GDTEntry
{
    UINT16 wLimit;
    UINT16 wBaseLow;
    BYTE bBaseMed;
    BYTE bAccess;
    BYTE bAttribs;
    BYTE bBaseHigh;
}__attribute__ ((packed));


#define ACS_CODE  0X9A  // (PRESENT | CSEG | READ)
#define ACS_DATA  0X92  // (PRESENT | DSEG | WRITE)
#define ACS_STACK 0X96  // (PRESENT | DSEG | WRITE | EXPAND DOWN )

#define INITIAL_GDT_ENTRIES     5       //if you change this also change the BITMAP setting operation
#define GDT_TOTAL_PAGES         17
#define IDT_TOTAL_PAGES         1

extern void * mm_GDT;
extern void * mm_IDT;

extern struct _GDTReg sysGDT;
extern struct _GDTReg sysIDT;
extern struct _GDTEntry InitGDTValues[INITIAL_GDT_ENTRIES];

UINT16 CreateGDTEntry(void * BaseAddress, UINT32 Limit, BYTE G_DB, BYTE Type_DPL);
UINT16 SetGDTEntry(UINT16 GDTNo, void * BaseAddress, UINT32 Limit, BYTE G_DB, BYTE Type_DPL);
void GDT_GetEntryRaw(UINT16 GDTNo, UINT32 * FirstDWord, UINT32 * SecondDWord);
UINT16 GDT_SetEntryRaw(UINT16 GDTNo, UINT32 FirstDWord, UINT32 SecondDWord);

void SetIDTEntry(BYTE InterruptNo, void (* IntHandler)());
void SetIDTGate(BYTE InterruptNo, UINT16 CodeSelector, void (* IntHandler)());
void SetIDTTask(BYTE InterruptNo, UINT16 TSSSelector);
void GetIDTEntry(BYTE InterruptNo, UINT32 * FirstDWord, UINT32 * SecondDWord);
#ifdef __cplusplus
    }
#endif
    
#endif

