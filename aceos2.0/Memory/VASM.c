/*
        ------------------------------------------------------
                   Virtual Address Space Manager for Ace Os
                          Created by Sam 13-June-2003
                          Modified on 14-Mar-2004 (GRUB)
        ------------------------------------------------------
    13-Jun-2003:
        The main problem with this memory manager is it does not take any initial arguments and assumes that 
        the kernel code/data is 256 pages and kernel initial memory is 1024 pages this causes 6MB of physical memory for kernel statically.
    14-Mar-2004:
        Now it takes multiboot lpMultiBootInfo as input. Total Physical Memory is queried from BIOS instead of direct probing.
*/
#include <TypeDefs.h>
#include <TaskMan.h>
#include <multiboot.h>
#include <printf.h>
#include <Memory.h>

//#define _DEBUG_

//This macro is used for setting kernel page directory entries (PDE) and Kernel's Page Bitmap
//note :- this will not set the Physical Page Bitmap
#define INIT_MAP_PAGES(kp_START, kp_TOTAL, kp_ADDRESS, Protection ) \
            for(i=(kp_START),lpHAddress=(UINT32 *)(kp_ADDRESS) ;i<(kp_START)+(kp_TOTAL);i++,lpHAddress+=VASM_PAGE_SIZE  ) \
                INIT_SetPageEntry(i, lpHAddress, Protection ); \
            mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpKernelPageBitmap, PAGE_USED, kp_START , kp_TOTAL) ;
            
//#define KERNEL_WORKING__PHY_MEM_START ((void *) 0x200000)

#define KERNEL_TMP_VARIABLE_HOLD        (0x19000)               //here the the InitKernelMem structure is placed temporarily

void * mm_GDT=NULL;                             
void * mm_IDT=NULL;                             //Interrupt Vector Table of Kernel
void * mm_GDTBitmap=0;                          //Global Descriptor Table's Bitmap, initialized during Phase I of InitMem

void * mm_KernelPageBitmap=0;                   //Kernel's Page Bitmap intalized during Phase I of InitMem

void * mm_KernelPageDirectory;                  //Kernel Page Directory intalized during Phase I of InitMem
//void * mm_KernelTPT;                          //Kernel Translation Page Table intalized during Phase I of InitMem
//void * mm_KernelPointerToTPT;                 //Kernel Translation Pointer for Translation Page Table intalized during Phase I of InitMem

/*Physical memory management variables*/
extern void * sbss, * g_code;
extern UINT32 mm_PhysicalTotal;
extern UINT32 mm_PhysicalAvailable;
extern UINT16 mm_PhysicalBitmapSize;
extern void * mm_PhysicalBitmap;

/*
The following structure just used only in InitMem Phase I & Phase II.
The purpose of this structure is to gather the correct values of kernel variables after
initialzing paging.
*/
struct InitKernelMem
{
    UINT32 dwKernelSize;
    void * lpKernelWorkingMemory;                   

    void * lpGDT;
    void * lpIDT;
    void * lpGDTBitmap;

    void * lpKernelPageBitmap;
        
    void * lpKernelPageDirectory;
    void * lpKernelPageTables;

    UINT32 dwPhysicalTotal;
    UINT32 dwPhysicalAvailable;

    void * lpPhysicalBitmap;
    UINT16 wPhysicalBitmapSize;
};
/*This function searchs in a BYTE Bitmap from the StartIndex for the Bits of Size with set/cleared
Input:
    lpBitmap            -       Byte Bitmap
    FindWhat            -       0 - Free  1 - Used
    StartIndex          -       Index where to start searching
    Size                -       Size
    Limit               -       Limit
Returns:
    0           -       If no contious bit sequence of size is found
    NonZero     -       Index of the bit found
*/
UINT32 mm_FindInBitmap(BYTE * lpBitmap, BYTE FindWhat, UINT32 StartIndex, UINT32 Size, UINT32 Limit)
{
    UINT32 Result,Result2=0,Start=StartIndex/BITS_IN_UINT32;
    while( !Result2 )
    {
        //find the first free/used bit
        Result=mm_FindFirstInBitmap((UINT32 *)lpBitmap, FindWhat, Start , Limit/BITS_IN_UINT32 );
        //convert the uint32 index to bit index
        Start=Result+1;//still we have to search, so prepare the index
        Result*=BITS_IN_UINT32;
        //search for continous bits of specified size
        Result2=mm_FindContinousInBitmap(lpBitmap , FindWhat, Result , Size , Result+Size+32 );
        if ( Result2 ) //if a page is found return result
            return Result2;
    }
    return 0;
}
/*
This functions searches for a element in a 4byte unsigned int array 
containing atleast one bit free

Input:
    lpBitmap            -       Byte Bitmap
    FindWhat            -       0 - Free  1 - Used
    StartIndex          -       Index where to start searching
    Limit               -       Limit
Returns:
    0           -       If no contious bit sequence of size is found
    NonZero     -       Index of the bit found
*/
UINT32 mm_FindFirstInBitmap(UINT32 * Bitmap, BYTE FindWhat, UINT32 StartIndex, UINT32 Limit )
{
    UINT32 i=StartIndex;
    if ( FindWhat == PAGE_FREE )
        while( Bitmap[i] == 0xFFFFFFFF && i<Limit )
            i++;
    else
        while( Bitmap[i] == 0 && i<Limit)
            i++;
    
    return (i<Limit)?i:0;       //return 0 if i>=limit    
}
/* This function Initializes the Memory Manager.
This is phase I of the Memory Initialization, this will setup the necessary PDE and PTE
Note :-
    Corresponding Page Tables should intialized before intializing Pages.

After calling this function the Kernel sets the Page Bit in CR0.    
*/
void * InitMemManPhaseI(multiboot_header_t *lpMultiBootHeader, multiboot_info_t * lpMultiBootInfo)
{
    UINT32 dwTotalPhysicalMemory;
    struct InitKernelMem * lpInitKMem;
    UINT32 i;
    void * lpWorkingMemoryTop;                  //this memory contains next free memory during intialization
    void * lpHAddress;                          //this variable is used only in the macro and carries only temporary values
    UINT32 dwKernelSize;
    void * lpKernelWorkingMemory;               //Should not modify the content of this variable
    /*struct _GDTReg * tmpGDT;
    struct _GDTReg * tmpIDT;*/
        /*this initial values are copied into the memory by InitMemPhaseI(), after that this array is unused*/
    struct _GDTEntry InitGDTValues[INITIAL_GDT_ENTRIES]=
    {
        {0, 0, 0, 0, 0, 0},                       //null
        {0xFFFF, 0, 0, ACS_CODE, 0xDF, 0},        //kernel code
        {0xFFFF, 0, 0, ACS_DATA, 0xDF, 0},        //kernel data
        {0xFFFF, 0, 0, 0xBA, 0xDF, 0},
        {0xFFFF, 0, 0, 0xB2, 0xDF, 0}
        
        //{0xFFFF, 0, 0, ACS_CODE, 0xF, 0},       //v86 code
        //{0xFFFF, 0, 0, ACS_DATA, 0xF, 0}        //v86 data
        
    };

    
    
    lpInitKMem=(struct InitKernelMem *)KERNEL_TMP_VARIABLE_HOLD;                           //this address should be same after initializing paging
    
    dwTotalPhysicalMemory = 1 + lpMultiBootInfo->mem_upper/1024;                           //Storing Amount of Physical Memory in MB
    dwKernelSize = lpMultiBootHeader->bss_end_addr - lpMultiBootHeader->load_addr ;                                                           //Size of Kernel Code, Data, BSS sections
    
    lpInitKMem->dwKernelSize = dwKernelSize;
    
    lpInitKMem->dwPhysicalAvailable = lpInitKMem->dwPhysicalTotal = dwTotalPhysicalMemory; //Total memory availale to the system
    
    
    //FIND TOP OF MEMORY
    /*1MB + KernelSize/PageSize 
    + ( 1 Page :: only if required )
    + Page Fault Page + Stack Pages
    + IDT Page + 15 GDT Pages
    + Page Fault
    */
        
    lpKernelWorkingMemory = (void *) 0x100000 +  ( dwKernelSize / VASM_PAGE_SIZE ) * VASM_PAGE_SIZE \
                            + ( ( dwKernelSize % VASM_PAGE_SIZE ) ? VASM_PAGE_SIZE : 0 ) \
                            + VASM_PAGE_SIZE + ( kp_KERNEL_STACK_TOTAL_PAGES * VASM_PAGE_SIZE ) \
                            + VASM_PAGE_SIZE + ( VASM_PAGE_SIZE *  15 ) + VASM_PAGE_SIZE ;
    lpInitKMem->lpKernelWorkingMemory = lpKernelWorkingMemory;

    lpInitKMem->lpPhysicalBitmap =  lpKernelWorkingMemory;                              //Physical Bitmap Start Address
    lpInitKMem->wPhysicalBitmapSize = ( dwTotalPhysicalMemory / 128 )+1;                //pages required for managing physical free bitmap
    //setting all the bits in the Physical Bitmap to 0
    for(i=0;i<lpInitKMem->wPhysicalBitmapSize*VASM_PAGE_SIZE/4;i++)
        ((UINT32 *)lpInitKMem->lpPhysicalBitmap)[i]=0;                                  //All are free
    
    //kernel's virtual space usage bitmap
    lpInitKMem->lpKernelPageBitmap = lpInitKMem->lpPhysicalBitmap + (lpInitKMem->wPhysicalBitmapSize * VASM_PAGE_SIZE) ;
    for(i=0;i<(VASM_PAGE_BITMAP_SIZE*VASM_PAGE_SIZE/(2*4) );i++)                        //0-2GB 
        ((UINT32 *)lpInitKMem->lpKernelPageBitmap)[i]=0xFFFFFFFF;                       //Mark as Reserved
    for(;i<(VASM_PAGE_BITMAP_SIZE*VASM_PAGE_SIZE/4);i++)                                //2-4 GB
        ((UINT32 *)lpInitKMem->lpKernelPageBitmap)[i]=0;                                //Mark as Free
    
    //Creating Page Directory Address & intial page tables
    lpInitKMem->lpKernelPageDirectory = lpInitKMem->lpKernelPageBitmap + (VASM_PAGE_BITMAP_SIZE * VASM_PAGE_SIZE);
    lpInitKMem->lpKernelPageTables = lpInitKMem->lpKernelPageDirectory + VASM_PAGE_SIZE;
    //setting Page Directory Entries
    for( i=0; i<(VASM_PAGE_SIZE/4); i++)                                                                 //setting all page table as invalid
        ((UINT32 *) lpInitKMem->lpKernelPageDirectory )[i] = 0; 
    
    //create GDT
    lpWorkingMemoryTop = lpInitKMem->lpKernelPageTables + VASM_PAGE_SIZE;
    lpInitKMem->lpGDT = lpWorkingMemoryTop;
    //Initialize GDT with NULL
    for( i=0; i<(GDT_TOTAL_PAGES * VASM_PAGE_SIZE)/4; i++ )
        ((UINT32 *) lpInitKMem->lpGDT )[i] = 0;
    //copying the initial GDT entries
    
    for(i=0;i< (sizeof(struct _GDTReg)/2 )*  INITIAL_GDT_ENTRIES;i++)
        ((UINT32 *) lpInitKMem->lpGDT )[i] = ((UINT32 *)InitGDTValues)[i];
    
    //create IDT
    lpWorkingMemoryTop += GDT_TOTAL_PAGES * VASM_PAGE_SIZE;
    lpInitKMem->lpIDT = lpWorkingMemoryTop;
    //Initialize IDT with NULL
    for( i=0; i<(IDT_TOTAL_PAGES * VASM_PAGE_SIZE)/4; i++ )
        ((UINT32 *) lpInitKMem->lpIDT )[i] = 0; 
    lpWorkingMemoryTop += IDT_TOTAL_PAGES * VASM_PAGE_SIZE;
    
    /*tmpGDT = lpWorkingMemoryTop;
    //load GDT value into system GDT register
    tmpGDT->dwBase = lpInitKMem->lpGDT;
    tmpGDT->wLimit = (INITIAL_GDT_ENTRIES*8)-1 ;
    asm volatile ("lgdt %0" : :"m"(*tmpGDT));    
    
    tmpIDT = tmpGDT + sizeof(struct _GDTEntry);
    //load IDT value into system IDT register
    tmpIDT->dwBase = lpInitKMem->lpIDT;
    tmpIDT->wLimit = 0;
    asm volatile ("lidt %0" : :"m"(*tmpIDT));*/
    
    //create GDT and IDT Bitmap
    lpWorkingMemoryTop+=VASM_PAGE_SIZE;
    lpInitKMem->lpGDTBitmap = lpWorkingMemoryTop;
    //setting all GDT entries as free        
    for(i=1;i<VASM_PAGE_SIZE/4;i++)
        ((UINT32 *)lpInitKMem->lpGDTBitmap)[i]=0;                                       //All are free
    ((BYTE *)lpInitKMem->lpGDTBitmap)[0]=0x3F;                                          //First six entries are occupied
    
    //initializing Page Tables
    //Oth Page Table for 1st Reserved 1MB
    lpWorkingMemoryTop+=VASM_PAGE_SIZE;
    INIT_SetPageDirectoryEntry(0, lpInitKMem->lpKernelPageDirectory , lpWorkingMemoryTop , 7);
    lpWorkingMemoryTop+=VASM_PAGE_SIZE;
    for(i=kp_KERNEL_START_PAGE/1024;i<=(kp_KERNEL_START_PAGE+kp_KERNEL_TOTAL_PAGES)/1024 ;i++,lpWorkingMemoryTop+=VASM_PAGE_SIZE)  //setting page tables for kernel code
        INIT_SetPageDirectoryEntry(i, lpInitKMem->lpKernelPageDirectory , lpWorkingMemoryTop + ( i*VASM_PAGE_SIZE ), 3);
    //for(i=kp_KERNEL_MEM_START/1024;i<=(kp_KERNEL_MEM_START+kp_KERNEL_MEM_TOTAL_PAGES)/1024 ;i++,lpWorkingMemoryTop+=VASM_PAGE_SIZE)  //setting page tables for kernel working memory
    //    INIT_SetPageDirectoryEntry(i, lpInitKMem->lpKernelPageDirectory , lpWorkingMemoryTop + ( (i+1)*VASM_PAGE_SIZE ), 3);
    
    lpWorkingMemoryTop+=VASM_PAGE_SIZE;
    //creating PointerToTPT & Translation Page Table PageTable and pointing them in PDE
    INIT_SetPageDirectoryEntry( kp_POINTER_TPT_START/1024, lpInitKMem->lpKernelPageDirectory, lpWorkingMemoryTop , 3 );
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpPhysicalBitmap, PAGE_USED, (UINT32)lpWorkingMemoryTop/VASM_PAGE_SIZE , 1 ) ;
    lpWorkingMemoryTop+=VASM_PAGE_SIZE;
    INIT_SetPageDirectoryEntry(kp_TRANSLATION_PT_START/1024, lpInitKMem->lpKernelPageDirectory, lpWorkingMemoryTop , 3 );
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpPhysicalBitmap, PAGE_USED, (UINT32)lpWorkingMemoryTop/VASM_PAGE_SIZE , 1 ) ;
    
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpKernelPageBitmap, PAGE_USED, kp_TRANSLATION_PT_START , kp_TRANSLATION_PT_TOTAL) ;
        
    //pointing the Translation Page Table
    for(i=kp_POINTER_TPT_START;i<kp_POINTER_TPT_START+kp_POINTER_TPT_TOTAL;i++,lpWorkingMemoryTop+=VASM_PAGE_SIZE  ) 
        INIT_SetPageEntry(i, lpWorkingMemoryTop, 3  ); 
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpKernelPageBitmap, PAGE_USED, kp_POINTER_TPT_START , kp_POINTER_TPT_TOTAL ) ;
    
    /* Hereafter lpWorkingMemoryTop should not be used, because accessing the memory after this will not be included in the paging table */
    
    // reserving first 1 MB in physical memory, so that no process can allocate it
    //mm_C_S_BitsInBitmap(lpInitKMem->lpPhysicalBitmap, PAGE_USED, 0 , (1024*1024)/4096 );
    for(i=0;i<8;i++)
        ((UINT32 *)lpInitKMem->lpPhysicalBitmap)[i]=0xFFFFFFFF;
    
    //Intiliazing Kernel Pages
    INIT_MAP_PAGES(  kp_RESERVED_START_PAGE, kp_RESERVED_TOTAL_PAGES, kp_RESERVED_ADDRESS, 7);
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpPhysicalBitmap, PAGE_USED, kp_RESERVED_START_PAGE , kp_RESERVED_TOTAL_PAGES ) ;

    INIT_MAP_PAGES(  kp_KERNEL_STACK_START_PAGE, kp_KERNEL_STACK_TOTAL_PAGES, kp_KERNEL_STACK_ADDRESS, 3 );
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpPhysicalBitmap, PAGE_USED, kp_KERNEL_STACK_START_PAGE, kp_KERNEL_STACK_TOTAL_PAGES ) ;
    
    INIT_MAP_PAGES(  kp_KERNEL_START_PAGE, kp_KERNEL_TOTAL_PAGES, kp_KERNEL_ADDRESS, 3 );
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpPhysicalBitmap, PAGE_USED, kp_KERNEL_ADDRESS/VASM_PAGE_SIZE , kp_KERNEL_TOTAL_PAGES ) ;
    
    lpWorkingMemoryTop+=VASM_PAGE_SIZE;
    int iTotalPages=(kp_KERNEL_MEM_START+kp_KERNEL_MEM_TOTAL_PAGES*2)/1024;
    for(i=(kp_KERNEL_MEM_START/1024)+1;i<=iTotalPages;i++,lpWorkingMemoryTop+=VASM_PAGE_SIZE)  //setting page tables for kernel working memory
        INIT_SetPageDirectoryEntry(i, lpInitKMem->lpKernelPageDirectory , lpWorkingMemoryTop + ( (i+1)*VASM_PAGE_SIZE ), 3);
    
    INIT_MAP_PAGES(  kp_KERNEL_MEM_START, kp_KERNEL_MEM_TOTAL_PAGES, kp_KERNEL_MEM_ADDRESS, 3 );
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpPhysicalBitmap, PAGE_USED, kp_KERNEL_MEM_ADDRESS/VASM_PAGE_SIZE, kp_KERNEL_MEM_TOTAL_PAGES ) ;
    
    //remember these entries should be removed after successful intialization of PAGING
    INIT_MAP_PAGES(  kp_K_TMP_MAP_StartPage, kp_K_TMP_MAP_TotalPages, kp_K_TMP_MAP_Address, 3  ); 
    mm_C_S_BitsInBitmap((BYTE *)lpInitKMem->lpPhysicalBitmap, PAGE_USED, kp_K_TMP_MAP_StartPage , kp_K_TMP_MAP_TotalPages ) ;
    
    asm volatile("movl %0, %%ebx" : :"r" ((kp_KERNEL_STACK_START_PAGE) * VASM_PAGE_SIZE) ); //copy new stack base value - it will be copied into esp
    return lpInitKMem->lpKernelPageDirectory;                                             //copy page directory address into eax
}
/*only after initalizing the Paging we can access the kernel variables
so during the Phase II the temporary variable valuse are moved to the kernel variables.
*/
void InitMemManPhaseII()
{
    struct InitKernelMem * lpInitKMem=(struct InitKernelMem *)KERNEL_TMP_VARIABLE_HOLD;
    int i;
    UINT32 dwKernelSize;
    void * lpKernelWorkingMemory;               
    
    dwKernelSize = lpInitKMem->dwKernelSize;
    lpKernelWorkingMemory = lpInitKMem->lpKernelWorkingMemory;
    
    //these are pointers to Linear Address Space which should be exists in a kernel memory area.
    //The physical address are unaffected and mapped during intialization.
    
    //load GDT value into system GDT register - 0x100000 + 0xC0000000;//
    sysGDT.dwBase = lpInitKMem->lpGDT - lpInitKMem->lpKernelWorkingMemory + kp_KERNEL_MEM_LINEAR_ADDRESS;
    sysGDT.wLimit = (INITIAL_GDT_ENTRIES*8)-1 ;
    asm volatile ("lgdt %0" : :"m"(sysGDT));    
    
    //load IDT value into system IDT register
    sysIDT.dwBase = lpInitKMem->lpIDT - lpInitKMem->lpKernelWorkingMemory + kp_KERNEL_MEM_LINEAR_ADDRESS;
    sysIDT.wLimit = (256*8) - 1;                //set all interrupt / falut controller - review code
    asm volatile ("lidt %0" : :"m"(sysIDT));
    
    mm_GDTBitmap = (void *)( lpInitKMem->lpGDTBitmap - lpInitKMem->lpKernelWorkingMemory + kp_KERNEL_MEM_LINEAR_ADDRESS);
    mm_GDT = (void *)( lpInitKMem->lpGDT - lpInitKMem->lpKernelWorkingMemory + kp_KERNEL_MEM_LINEAR_ADDRESS);
    mm_IDT = (void *)( lpInitKMem->lpIDT - lpInitKMem->lpKernelWorkingMemory + kp_KERNEL_MEM_LINEAR_ADDRESS);
    
    mm_PhysicalBitmap=(void *)(lpInitKMem->lpPhysicalBitmap - lpInitKMem->lpKernelWorkingMemory + kp_KERNEL_MEM_LINEAR_ADDRESS);
    
    mm_KernelPageBitmap=(void *)(lpInitKMem->lpKernelPageBitmap - lpInitKMem->lpKernelWorkingMemory + kp_KERNEL_MEM_LINEAR_ADDRESS);
    mm_KernelPageDirectory=(void *)(lpInitKMem->lpKernelPageDirectory - lpInitKMem->lpKernelWorkingMemory + kp_KERNEL_MEM_LINEAR_ADDRESS);
    
    
    //these are normal variable taking the value
    mm_PhysicalTotal=lpInitKMem->dwPhysicalTotal;
    mm_PhysicalAvailable=lpInitKMem->dwPhysicalAvailable;
    mm_PhysicalBitmapSize=lpInitKMem->wPhysicalBitmapSize;
    
    
    /*copying the PDE values into Translation Page Table
    so that we can access the Page Tables without Page Fault
    */
    for(i=0;i<1024;i++)
    {
        UINT32 PDEValue=GetPageDirectoryEntry(i, mm_KernelPageDirectory );
        ((UINT32 *)VASM_LPTPT)[i] = PDEValue;  
    }
    
    //initialize memory variables used by Physical Page Structures
    VASM_InitLowPhysicalPageStructures();
}
/* This function sets the specified page entry in a page table. 
Important :-
    It must called only during the Initialization Phase I.
    (ie before initalization of Paging).

Note:-****
 1) You have to give the page directory address not the page table address. 
    It will take the Page Table address from the Page Directory Entry
 2) You must fill the address of the empty Page Table in the Page Directory before calling this function
 3) This function will not set the page bitmap and physical free bitmap
Input : 
    PageNo              - Page No ( 0 - 1024*1024 )
    lpPhysicalAddress   - The Physical address to be associated with that page
    Protection          - Protection and Present value
Returns :
    0 - Success
    1 - The Page Table Entry is invalid or not present
*/
int INIT_SetPageEntry(UINT32 PageNo, void * lpPhysicalAddress, BYTE Protection)
{
    int PTNo=PageNo/1024;               //Page Directiry Entry No
    UINT32 PageTable, PTEValue;
    struct InitKernelMem * lpInitKMem=(struct InitKernelMem *)KERNEL_TMP_VARIABLE_HOLD; 
        
    PageNo = PageNo%1024;               //Actual Page No 
    PageTable=((UINT32 *)lpInitKMem->lpKernelPageDirectory)[PTNo];
    if ( !(PageTable & 1) ) 
    {
        DEBUG();
        return PTNo; //Page Table not present
    }
        
    PageTable&=0xFFFFF000;  //extract the page table address
    PTEValue=(UINT32)lpPhysicalAddress;                 //storing Physical Address
    PTEValue=(PTEValue & 0x0FFFFF000)|Protection;       //zeros all except the upper 20 bits and sets Supervisor and Present bits
    ((UINT32 *)PageTable)[PageNo] = PTEValue;           //Page Table Entry is 4 byte long
    
    return 0; //success    
}
/* This function sets the specified page entry in a page table. 
Donot call this function before intializing Paging.
Note:-****
 1) The given Page Directory is used only for checking whether the specified Page Table is
    present in the memory or not. But there may be a design change which may eliminate this
    parameter(lpPageDirectory). 
 2) You must fill the address of the empty Page Table in the Page Directory before calling this function
 3) It will not set the page bitmap and physical free bitmap
Input : 
    PageNo                      - Page No ( 0 - 1024*1024 )
    lpPageDirectory             - Address of the Page Directory ( pointer in the kernel's virtual address space)
    TranslationPageTable        - Translation Page Table
    lpPhysicalAddress           - Physical address of Page Table to set in PD
    Protection                  - Protection and Present value
Returns :
    0 - Success
    1 - The Page Table Entry is invalid or not present
*/
int SetPageEntry(UINT32 dwPageNo, void * lpPageDirectory, UINT32 * TranslationPageTable, void * lpPhysicalAddress, BYTE Protection)
{
    int PageNo = dwPageNo;
    int PTNo=PageNo/1024;       //Page Directiry Entry No
    UINT32 PageTable, PTEValue;
    #ifdef _DEBUG_
        dbgprintf("\n\t VASM :: SETPAGEENTRY() PageNo %d lpPageDirectory %X lpPhysicalAddress %X",PageNo, lpPageDirectory,lpPhysicalAddress  );
    #endif
    
    PageNo = PageNo%1024;                //Actual Page No 
    PageTable=((UINT32 *)lpPageDirectory)[PTNo];
    if ( !(PageTable & 1) ) 
    {
        #ifdef _DEBUG_
            dbgprintf("\n VASM :: SETPAGEENTRY() - Page Table Not Present %d", PTNo );
        #endif
        return 1; //Page Table not present
    }
    UINT32 * lpMappedPT = (UINT32 *)( (UINT32)TranslationPageTable + (PTNo*VASM_PAGE_SIZE )  ) ;
    #ifdef _DEBUG_
        dbgprintf("\n VASM :: SETPAGEENTRY() PTList at %X", lpMappedPT );
    #endif
    
    PTEValue=(UINT32)lpPhysicalAddress;          //storing physical Address
    PTEValue=(PTEValue & 0xFFFFFF000)|Protection;       //zeros all except the upper 20 bits and sets Supervisor and Present bits
    
    #ifdef _DEBUG_
        dbgprintf("\n VASM :: SETPAGEENTRY() Creating Entry in Page Table %X at %d value %X", lpMappedPT , PageNo, PTEValue);
    #endif
    ((UINT32 *)lpMappedPT)[PageNo] = PTEValue;           //Page Table Entry is 4 byte long
    __asm__ __volatile__ ("invlpg (%0)": :"r" ( PageNo * VASM_PAGE_SIZE ) );
    
    return 0; //success    
}
/*
this function extracts the PageEntry from the TPT
*/
UINT32 GetPageEntry(UINT32 PageNo, UINT32 * TranslationPageTable)
{
    int PTNo=PageNo/1024;       //Page Directiry Entry No
    #ifdef _DEBUG_
        dbgprintf("\n\t VASM :: GETPAGEENTRY() PageNo %d ",PageNo  );
    #endif
    
    PageNo = PageNo%1024;                //Actual Page No 
    
    UINT32 * lpMappedPT = (UINT32 *)( (UINT32)TranslationPageTable + (PTNo*VASM_PAGE_SIZE )  ) ;
    #ifdef _DEBUG_
        dbgprintf("\n VASM :: SETPAGEENTRY() PTList at %X", lpMappedPT );
    #endif
    return ((UINT32 *)lpMappedPT)[PageNo];        
}

/*
Call this function during Phase I of the InitMem to set the PDE
Input :
    PDENo               -       Page Directory Entry No to set
    lpPageDirectory     -       Page Directory Physical Address
    lpPTAddress         -       Physical address of Page Table
    Protection          -       Protection ( 3 - for Supervisor and Present )
*/
void INIT_SetPageDirectoryEntry(int PDENo, void * lpPageDirectory, void * lpPTAddress, BYTE Protection )
{
    if ( PDENo >= 1024 || PDENo < 0 )
        return;
    UINT32 PDEValue=(UINT32)lpPTAddress;         //PageTable Address
    PDEValue=(PDEValue & 0x0FFFFF000) | Protection;
    
    ((UINT32 *)lpPageDirectory)[PDENo] = PDEValue; //Page Directory Entry is 4 byte long
}
/*The following function sets the corresponding Page Directory Entry in the Page Directory
Call this function during Phase II of the InitMem to set the PDE
Input :
    PDENo               -       Page Directory Entry No to set
    lpPageDirectory     -       Page Directory linear Address
    lpPageTableList     -       Pointer to TPT
    Protection          -       Protection ( 3 - for Supervisor and Present )
*/
void SetPageDirectoryEntry(int PDENo, void * lpPageDirectory, void * lpPageTableList, void * lpPTAddress, BYTE Protection )
{
    if ( PDENo >= 1024 || PDENo < 0 )
        return;
    UINT32 PDEValue=(UINT32)lpPTAddress;         //PageTable Address
    PDEValue=(PDEValue & 0x0FFFFF000) | Protection;
    #ifdef _DEBUG_
        dbgprintf("\n\t VASM :: SetPageDirectoryEntry(%d) lpPageTableList %X lpPageDirectory %X, PDEValue %X ", PDENo, lpPageTableList, lpPageDirectory, PDEValue);
    #endif
    ((UINT32 *)lpPageTableList)[PDENo] = PDEValue; 
    ((UINT32 *)lpPageDirectory)[PDENo] = PDEValue; //Page Directory Entry is 4 byte long
}
/*Returns the content of the given page directory entry*/
UINT32 GetPageDirectoryEntry(int PDENo, void * lpPageDirectory)
{
    return ((UINT32 *)lpPageDirectory)[PDENo];
}

/* This function returns the size of a virtual page*/
UINT32 VASM_GetPageSize()
{
    return VASM_PAGE_SIZE;
}
UINT32 VirtualAlloc(UINT32 StartingAddress, UINT32 SizeRequired ,  UINT16 AllocationType, UINT16 ProtectionType)
{
    return VirtualAllocEx( GetTaskID(), StartingAddress, SizeRequired,  AllocationType, ProtectionType);
}
/*
This function allocates a range of linear address space in the given Task's Address Space
*/
UINT32 VirtualAllocEx(UINT32 TaskID, UINT32 StartingAddress, UINT32 SizeRequired ,  UINT16 AllocationType, UINT16 ProtectionType)
{
    #ifdef _DEBUG_
        dbgprintf("\n\r VASM :: VirtualAlloc() Size %d ",SizeRequired );
    #endif
    UINT32 lpVirtualPage=0,j;
    UINT32 lpPhysicalPage;
    
    LPTASK TI = GetTaskInfo( TaskID );
    if ( TI == NULL )
    {
        #ifdef _DEBUG_
            dbgprintf("\n VASM :: Task(%d) Not Found ",TaskID);
        #endif
        return 0;
    }
    //find the free page's starting address
    #ifdef _DEBUG_
        dbgprintf("\n VASM :: Search Bitmap %X, %X",TI->m_PageBitmapAddress, VASM_PAGE_BITMAP_SIZE * VASM_PAGE_SIZE * 8 );
    #endif
    if ( ! (AllocationType & (MEM_COMMIT | MEM_RESERVE )  ) ) //return if allocation type does not RESERVE or COMMIT
    {
        #ifdef _DEBUG_
            dbgprintf("\n VASM :: Invalid Allocation type - %X", AllocationType );
        #endif
        return 0;
    }
    if ( AllocationType & MEM_RESERVE )
    {
        lpVirtualPage = mm_FindInBitmap((BYTE *)TI->m_PageBitmapAddress, PAGE_FREE , StartingAddress, SizeRequired , VASM_PAGE_BITMAP_SIZE * VASM_PAGE_SIZE * 8);
        #ifdef _DEBUG_
            dbgprintf("\n VASM :: Virtual Free Page Found at %X in PD %X",lpVirtualPage, TI->m_PageDirectoryAddress );
        #endif
    }
    else
        lpVirtualPage = StartingAddress / VASM_PAGE_SIZE;
    
    if ( lpVirtualPage ) //if free page found
    {
        //mark the free pages as allocated
        mm_C_S_BitsInBitmap((BYTE *)TI->m_PageBitmapAddress, PAGE_USED, lpVirtualPage, SizeRequired );
        
        if ( ! (AllocationType & MEM_COMMIT) ) // if the page is not commited return
        {
            #ifdef _DEBUG_
                dbgprintf("\n VASM :: VirtualAlloc() Returns %X",lpVirtualPage*VASM_PAGE_SIZE );
            #endif
            return lpVirtualPage*VASM_PAGE_SIZE;
        }
        
        for(j=0;j<SizeRequired ;j++) //fill the task's page directory with correct information
        {
            
            //find a free page for the current virtual page
            //since virtual page should contigous and physical pages need not to be contigious
            lpPhysicalPage = mm_FindInBitmap( (BYTE *)mm_PhysicalBitmap, PAGE_FREE, 0, 1,  VASM_PAGE_BITMAP_SIZE * VASM_PAGE_SIZE );
            #ifdef _DEBUG_
                dbgprintf("\n VASM :: Physical Free Page Found at %X in %X",lpPhysicalPage, mm_PhysicalBitmap );
            #endif

            if ( lpPhysicalPage ) // if found
            {
                //mark the physical storage as allocated
                mm_C_S_BitsInBitmap((BYTE *)mm_PhysicalBitmap, PAGE_USED, lpPhysicalPage, 1 );
            
                #ifdef _DEBUG_
                    dbgprintf("\n VASM :: Setting Page Entry - %X in PD %X",lpVirtualPage+j,TI->m_PageDirectoryAddress);
                #endif
                if ( SetPageEntry(lpVirtualPage+j, TI->m_PageDirectoryAddress, VASM_TPT, (void *) (lpPhysicalPage * VASM_PAGE_SIZE), 3 ) ) //it will return 1 if page table is not fiiled
                {
                    //find a free page in physical free
                    #ifdef _DEBUG_
                        dbgprintf("\n VASM :: Page Table not found. Creating new one");
                    #endif
                    UINT32 i;
                    UINT32 lpNewPageTable=mm_FindInBitmap((BYTE *)mm_PhysicalBitmap , PAGE_FREE , 0 , 1 , VASM_PAGE_BITMAP_SIZE * VASM_PAGE_SIZE) * VASM_PAGE_SIZE ;
                    if ( lpNewPageTable == 0 )
                    {
                        #ifdef _DEBUG_
                            dbgprintf("\n VASM :: No physical memory to create page table.");
                        #endif
                        return 0;
                    }
                    #ifdef _DEBUG_
                        dbgprintf(" at physical address %X ", lpNewPageTable );
                        dbgprintf("\n VASM :: Setting Page Directory Entry %d, using lpTPT %X ", (lpVirtualPage+j)/1024, VASM_LPTPT );
                    #endif
                    SetPageDirectoryEntry( (lpVirtualPage+j)/1024 , TI->m_PageDirectoryAddress, VASM_LPTPT , (void *)lpNewPageTable , 3 );
                    mm_C_S_BitsInBitmap((BYTE *)mm_PhysicalBitmap, PAGE_USED, lpNewPageTable/VASM_PAGE_SIZE , 1 );
          
                    #ifdef _DEBUG_                    
                        dbgprintf("\n VASM :: Setup PageTable %d", ((lpVirtualPage+j)/1024));
                    #endif
                    //setup page table
                    for(i=0;i<VASM_PAGE_SIZE/4;i++)     //all entries are not present 
                        ((UINT32 *)VASM_TPT)[(((lpVirtualPage+j)/1024)*4096)/4+i]=0;
                    #ifdef _DEBUG_
                        dbgprintf("\n VASM :: Setting up Required Page " );
                    #endif
                    //setup the required page
                    if ( SetPageEntry(lpVirtualPage+j, TI->m_PageDirectoryAddress, VASM_TPT , (void *) (lpPhysicalPage * VASM_PAGE_SIZE) , 3) )
                    {
                        #ifdef _DEBUG_
                            dbgprintf("\n VASM :: Still unable to create Page Table " );
                        #endif
                        return 0;
                    }
                }
            }
            else
            {
                #ifdef _DEBUG_
                    dbgprintf("\n AllocHeap :: Commit Failed. No Free Physical Memory");
                #endif
            }
        
            #ifdef _DEBUG_
                dbgprintf("\n VASM :: Marking Virutial Page Bitmap ");
            #endif
            //mark the task's page bitmap as allocated
            //mm_C_S_BitsInBitmap(TI->m_PageBitmapAddress, PAGE_USED, lpVirtualPage , SizeRequired) ;
            if ( AllocationType & MEM_RESET )
            {
                //write code for zeroing the contents
            }
        }
        #ifdef _DEBUG_
            dbgprintf("\n VASM :: VirtualAlloc() Returns %X",lpVirtualPage*VASM_PAGE_SIZE );
        #endif
        return lpVirtualPage*VASM_PAGE_SIZE; //success return the allocated memory
    }
    
    #ifdef _DEBUG_
        dbgprintf("\n VASM :: No Free Page is Found for Task(%d)",TaskID);
    #endif

    return 0;
}
UINT32 VirtualFree(UINT32 StartingAddress, UINT32 Size, DWORD FreeType)
{
    return VirtualFreeEx( GetTaskID(), StartingAddress, Size, FreeType );
}
/*********************This function is not completed
currently it just sets the corresponding bit in the Page Bitmap as free,
but it neither modifies the Physicals bitmap nor Page Directory entry 
*/
UINT32 VirtualFreeEx(UINT32 TaskID, UINT32 StartingAddress, UINT32 Size, DWORD FreeType)
{
    UINT32 TotTasks=List_Count(sys_TaskList);
    UINT32 i;
    if ( FreeType & MEM_RELEASE )
    {
        if ( Size != 0 )
        {
            //SetLastError(ERROR_INVALID_PARAMETER);
            return 1;
        }
        for(i=0;i<TotTasks;i++)
        {
            struct TaskInfo * TI= (struct TaskInfo*)List_GetData(sys_TaskList,"",i);
            if( TI->TaskID == TaskID )
            {
                //Size/=VASM_PAGE_SIZE;        //divide the size in bytes by the Page size to get the required no of pages
                UINT32 UnReservedStart = mm_FindInBitmap((BYTE *)TI->m_PageBitmapAddress, PAGE_USED, StartingAddress, 1, VASM_PAGE_BITMAP_SIZE * VASM_PAGE_SIZE * 8);
                Size = UnReservedStart - StartingAddress;
                if ( Size <= 0 )
                    return 2;
                mm_C_S_BitsInBitmap((BYTE *)TI->m_PageBitmapAddress, PAGE_FREE, StartingAddress, Size);
                
                //todo - write to code to unallocate the physical mapping
                return Size;
            }
        }
    }
    return 0;
}
/*
This function is used to map a physical address to kernels linear address
*/
void * KernelMap(void * lpStartingAddress, UINT32 Size, void * lpPhysicalAddress)
{
    int iTotalPages=(int)(Size/VASM_PAGE_SIZE) + ((Size%VASM_PAGE_SIZE)?1:0);
    #ifdef _DEBUG_
        dbgprintf("\n VASM :: KernelMap() %X %X(%d Pages) %X",lpStartingAddress, Size, iTotalPages, lpPhysicalAddress );
    #endif
    
    UINT32 * lpMapped=(UINT32 *)VirtualAlloc(((UINT32)lpStartingAddress)/VASM_PAGE_SIZE , iTotalPages,  MEM_RESERVE , 0);
    int i;
    if ( lpMapped )
    {
        //it may cause page fault if the page table is not present so checkit
        
        for(i=0;i<iTotalPages;i++)
        {
            if ( SetPageEntry( ((UINT32)lpMapped/VASM_PAGE_SIZE)+i, mm_KernelPageDirectory, VASM_TPT , (void *)((UINT32)lpPhysicalAddress+ (i*VASM_PAGE_SIZE) ) , 3) )
            {
                KernelUnMap(lpMapped, i*VASM_PAGE_SIZE);
                return 0; //error
            }
        }
        #ifdef _DEBUG_
            dbgprintf("\n VASM :: KernelMap() returns %X", lpMapped);
        #endif
        return lpMapped;
    }
    else
    {
        dbgprintf("\n VASM :: KernelMap() failed");
        return 0;
    }
}
void * KernelUnMap(void * lpStartingAddress, UINT32 Size)
{
    #ifdef _DEBUG_
        dbgprintf("\n\r VASM :: Warning - KernelUnMap() not implemented");
    #endif
    return 0;
}

