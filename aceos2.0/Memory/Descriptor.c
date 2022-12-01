/*
	Descriptor Management Code for Ace OS Kernel 
			- Created by Samuel on 20-July-2002 (samuelhard@yahoo.com)
        Only kernel should use the functions used in this file
    
    This file is converted from the existing asm code.
*/

#include "TypeDefs.h"
#include "Memory.h"
#include "TaskMan.h"

struct _GDTReg sysGDT __attribute__ ((aligned (32))) ;
struct _GDTReg sysIDT __attribute__ ((aligned (32))) ;

/* It will creates the sysGDT Entry and sets the Page Bitmap
Input : Base Address  - Base address of the section
        Limit         - Limit of the section
        G_DB          - Granularity and Default Operation Size (20-23 bits of second dword)
        Type_DPL      - Type & Descriptor Previlige Level of the Section(8-15 bits of second dword)
Returns :
    0 - Failure
    Non-Zero - sysGDT Selector
*/
UINT16 CreateGDTEntry(void * BaseAddress, UINT32 Limit, BYTE G_DB, BYTE Type_DPL)
{
    UINT32 FirstDWord, SecondDWord, tmpValue;
    UINT16 FreeGDTNo;
    
    //finding free sysGDT entry
    FreeGDTNo = mm_FindInBitmap( mm_GDTBitmap, PAGE_FREE, 0, 1, (GDT_TOTAL_PAGES-1) * VASM_PAGE_SIZE );
    if ( FreeGDTNo == 0 )
    {
        dbgprintf("\n CreateGDTEntry Failed :: No free sysGDT in Bitmap %X", mm_GDTBitmap);
        return 0;
    }
    asm volatile ("sgdt %0" : :"m"(sysGDT));
    //dbgprintf("\n Base Address %X Limit %X G_DB %X Type_DPL %X", BaseAddress, Limit, G_DB, Type_DPL );
    FirstDWord = (UINT32)BaseAddress;
    FirstDWord<<=16;
    tmpValue=Limit & 0x0000FFFF;
    FirstDWord|=tmpValue;
    
    SecondDWord = (UINT32)BaseAddress;          //setting bits 0-7
    SecondDWord>>=16;                   
    SecondDWord&=0x000000FF;            //filtering 8-15
    
    tmpValue=(UINT32)BaseAddress;               //setting 24-31
    tmpValue>>=24;                      //removing 0-23
    tmpValue<<=24;                      //moving to 23 position
    SecondDWord|=tmpValue;             
    
    tmpValue=Limit;                     
    tmpValue>>=16;
    tmpValue|=Type_DPL;
    tmpValue<<=8;
    SecondDWord|=tmpValue;
    
    tmpValue=G_DB;                    //setting 16-23
    tmpValue<<=16;
    SecondDWord|=tmpValue;
    
    
    //BYTE * lpGDT=(BYTE *)KernelMap((void *)0, VASM_PAGE_SIZE*2,(UINT32 *) (((UINT32)sysGDT.dwBase/VASM_PAGE_SIZE)*VASM_PAGE_SIZE) ); 
    //UINT32 PageGap=(UINT32)sysGDT.dwBase - ((UINT32)sysGDT.dwBase/VASM_PAGE_SIZE)*VASM_PAGE_SIZE;
    //UINT32 * lpGDTEntry=(UINT32 *)((UINT32)lpGDT+PageGap+((FreeGDTNo-1)*8));
    
    UINT32 dwPageGap = (UINT32)sysGDT.dwBase - ( (UINT32)sysGDT.dwBase/VASM_PAGE_SIZE ) * VASM_PAGE_SIZE;
    UINT32 * lpGDTEntry = (UINT32 *)( (UINT32)mm_GDT + dwPageGap + ( (FreeGDTNo-1)*8) );
    
    lpGDTEntry[0]=FirstDWord;
    lpGDTEntry[1]=SecondDWord;
    
    //increase sysGDT limit
    sysGDT.wLimit = (FreeGDTNo*8)-1 ;
    //load new sysGDT
    asm volatile ("lgdt %0" : :"m"(sysGDT));
    
    //setting the GDTEntry as used
    mm_C_S_BitsInBitmap((BYTE *)mm_GDTBitmap, PAGE_USED, FreeGDTNo, 1) ;    

    //dbgprintf("\n First %X %X", FirstDWord, SecondDWord);
    return (FreeGDTNo-1)*8;//return sysGDT Selector
}
/* 
    This function is used to modify the existing sysGDT Entry in the sysGDT.
It also sets the PageBitmap of sysGDT.
Input : 
        GDTNo         - sysGDT Entry No to modify
        Base Address  - Base address of the section
        Limit         - Limit of the section
        G_DB          - Granularity and Default Operation Size (20-23 bits of second dword)
        Type_DPL      - Type & Descriptor Previlige Level of the Section(8-15 bits of second dword)
Returns :
    0 - Failure
    Non-Zero - sysGDT Selector
Note :-
    you have to pass sysGDT Entry No not sysGDT Selector which is returned by this function and CreateGDTEntry
*/
UINT16 SetGDTEntry(UINT16 GDTNo, void * BaseAddress, UINT32 Limit, BYTE G_DB, BYTE Type_DPL)
{
    UINT32 FirstDWord, SecondDWord, tmpValue;
    
    asm volatile ("sgdt %0" : :"m"(sysGDT));
    if ( GDTNo*8 > sysGDT.wLimit )
        return 0;
    //dbgprintf("\n Base Address %X Limit %X G_DB %X Type_DPL %X", BaseAddress, Limit, G_DB, Type_DPL );
    FirstDWord = (UINT32)BaseAddress;
    FirstDWord<<=16;
    tmpValue=Limit & 0x0000FFFF;
    FirstDWord|=tmpValue;
    
    SecondDWord = (UINT32)BaseAddress;          //setting bits 0-7
    SecondDWord>>=16;                   
    SecondDWord&=0x000000FF;            //filtering 8-15
    
    tmpValue=(UINT32)BaseAddress;               //setting 24-31
    tmpValue>>=24;                      //removing 0-23
    tmpValue<<=24;                      //moving to 23 position
    SecondDWord|=tmpValue;             
    
    tmpValue=Limit;                     
    tmpValue>>=16;
    tmpValue|=Type_DPL;
    tmpValue<<=8;
    SecondDWord|=tmpValue;
    
    tmpValue=G_DB;                    //setting 16-23
    tmpValue<<=16;
    SecondDWord|=tmpValue;
    
    //BYTE * lpGDT=(BYTE *)KernelMap((void *)0, VASM_PAGE_SIZE*2, (UINT32*) (((UINT32)sysGDT.dwBase/VASM_PAGE_SIZE)*VASM_PAGE_SIZE) );
    //UINT32 PageGap=(UINT32)sysGDT.dwBase - ((UINT32)sysGDT.dwBase/VASM_PAGE_SIZE)*VASM_PAGE_SIZE;
    //UINT32 * lpGDTEntry=(UINT32 *)((UINT32)lpGDT+PageGap+(GDTNo*8));
    
    UINT32 dwPageGap = (UINT32)sysGDT.dwBase - ( (UINT32)sysGDT.dwBase/VASM_PAGE_SIZE ) * VASM_PAGE_SIZE;
    UINT32 * lpGDTEntry = (UINT32 *)( (UINT32)mm_GDT + dwPageGap + ( (GDTNo-1)*8) );
    
    lpGDTEntry[0]=FirstDWord;
    lpGDTEntry[1]=SecondDWord;

    //KernelUnMap( lpGDT, VASM_PAGE_SIZE*2 );
    //dbgprintf("\n First %X %X", FirstDWord, SecondDWord);
    
    //setting the GDTEntry as used
    mm_C_S_BitsInBitmap((BYTE *)mm_GDTBitmap, PAGE_USED, GDTNo, 1) ; 
    return GDTNo*8;//return sysGDT Selector
    
}
void GDT_GetEntryRaw(UINT16 GDTNo, UINT32 * FirstDWord, UINT32 * SecondDWord)
{
    *FirstDWord=0;
    *SecondDWord=0;
    asm volatile ("sgdt %0" : :"m"(sysGDT));
    if ( GDTNo*8 > sysGDT.wLimit )
        return;
    //dbgprintf("\n Base Address %X Limit %X G_DB %X Type_DPL %X", BaseAddress, Limit, G_DB, Type_DPL );
    
    BYTE * lpGDT=(BYTE *)KernelMap((void *)0, VASM_PAGE_SIZE*2, (UINT32*) (((UINT32)sysGDT.dwBase/VASM_PAGE_SIZE)*VASM_PAGE_SIZE) );
    UINT32 PageGap=(UINT32)sysGDT.dwBase - ((UINT32)sysGDT.dwBase/VASM_PAGE_SIZE)*VASM_PAGE_SIZE;
    UINT32 * lpGDTEntry=(UINT32 *)((UINT32)lpGDT+PageGap+(GDTNo*8));
    
    if ( lpGDT )
    {
        *FirstDWord=lpGDTEntry[0];
        *SecondDWord=lpGDTEntry[1];
    }
    else
    {
        dbgprintf("\n Unable to allocate page for tmp mapping");
        return;
    }
    KernelUnMap( lpGDT, VASM_PAGE_SIZE*2 );
    return;
}
UINT16 GDT_SetEntryRaw(UINT16 GDTNo, UINT32 FirstDWord, UINT32 SecondDWord)
{
    asm volatile ("sgdt %0" : :"m"(sysGDT));
    if ( GDTNo*8 > sysGDT.wLimit )
        return 0;
    //dbgprintf("\n Base Address %X Limit %X G_DB %X Type_DPL %X", BaseAddress, Limit, G_DB, Type_DPL );
    
    //BYTE * lpGDT=(BYTE *)KernelMap((void *)0, VASM_PAGE_SIZE*2, (UINT32*) (((UINT32)sysGDT.dwBase/VASM_PAGE_SIZE)*VASM_PAGE_SIZE) );
    //UINT32 PageGap=(UINT32)sysGDT.dwBase - ((UINT32)sysGDT.dwBase/VASM_PAGE_SIZE)*VASM_PAGE_SIZE;
    //UINT32 * lpGDTEntry=(UINT32 *)((UINT32)lpGDT+PageGap+(GDTNo*8));
    
    UINT32 dwPageGap = (UINT32)sysGDT.dwBase - ( (UINT32)sysGDT.dwBase/VASM_PAGE_SIZE ) * VASM_PAGE_SIZE;
    UINT32 * lpGDTEntry = (UINT32 *)( (UINT32)mm_GDT + dwPageGap + ( (GDTNo-1)*8) );
        
    lpGDTEntry[0]=FirstDWord;
    lpGDTEntry[1]=SecondDWord;

    return GDTNo*8;
}

/* This function sets the IDT Entry*/
void SetIDTEntry(BYTE InterruptNo, void (* IntHandler)())
{
    UINT32 * lpIDTEntry;
    UINT32 FirstDWord, SecondDWord,tmpValue;
    
    lpIDTEntry=(UINT32 *)(mm_IDT+(InterruptNo*8));
    FirstDWord = SecondDWord =(UINT32) IntHandler;
        
    tmpValue = CODE_SELECTOR;                   //Code Selector = 0x8
    tmpValue <<= 16;                            //Move it to upper byte
    FirstDWord &= 0x0000FFFF;                   //
    FirstDWord |= tmpValue;                     //
    
    tmpValue=0xEE00;                            //Interrupt Gate - 32 bit DPL - 11
    SecondDWord &= 0xFFFF0000;                  //
    SecondDWord |= tmpValue;
    
    ((UINT32 *)lpIDTEntry)[0]=FirstDWord;       //Copy the first dword
    ((UINT32 *)lpIDTEntry)[1]=SecondDWord;      //copy the second dword
}
void SetIDTGate(BYTE InterruptNo, UINT16 CodeSelector, void (* IntHandler)())
{
    UINT32 * lpIDTEntry;
    UINT32 FirstDWord, SecondDWord;
    UINT32 tmpValue;
    
    lpIDTEntry = (UINT32 *)(mm_IDT+(InterruptNo*8));
    FirstDWord = CodeSelector;
    FirstDWord <<= 16;
    tmpValue = (UINT32) IntHandler;
    tmpValue &= 0x0000FFFF;
    FirstDWord |= tmpValue;
    
    SecondDWord = 0xEE00; //Interrupt Gate - DPL 11
    tmpValue = (UINT32) IntHandler;
    tmpValue &= 0xFFFF0000;
    SecondDWord|=tmpValue;
    
    ((UINT32 *)lpIDTEntry)[0] = FirstDWord;       //Copy the first dword
    ((UINT32 *)lpIDTEntry)[1] = SecondDWord;      //copy the second dword
}
void SetIDTTask(BYTE InterruptNo, UINT16 TSSSelector)
{
    UINT32 * lpIDTEntry;
    UINT32 FirstDWord, SecondDWord;
    
    lpIDTEntry=(UINT32 *)(mm_IDT+(InterruptNo*8));
    FirstDWord=TSSSelector;
    FirstDWord<<=16;
    
    SecondDWord=0xE500; //Task Gate - DPL 11
    ((UINT32 *)lpIDTEntry)[0]=FirstDWord;       //Copy the first dword
    ((UINT32 *)lpIDTEntry)[1]=SecondDWord;      //copy the second dword
}
void GetIDTEntry(BYTE InterruptNo, UINT32 * FirstDWord, UINT32 * SecondDWord)
{
    UINT32 * lpIDTEntry=(UINT32 *)(mm_IDT+(InterruptNo*8));
    *FirstDWord=((UINT32 *)lpIDTEntry)[0];       //Copy the first dword
    *SecondDWord=((UINT32 *)lpIDTEntry)[1];      //copy the second dword
}

