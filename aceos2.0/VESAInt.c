/*
  Name: 		VESA Interrupt Support in PM
  Author: 			- Sam (samuelhard@yahoo.com)
  Date: 		11-Jun-2003 15:33
  
*/
#include <TypeDefs.h>
#include <Thread.h>
#include <TaskMan.h>
#include <GFS.h>
#include <V86.h>
#include <malloc.h>
#include <IO.h>


typedef struct 
{
    char Signature[4];
    short VESAVersion;
    UINT32 OemStringPtr;
    UINT32 Capabilities;
    UINT32 VideoModePtr;
    short TotalMemory;
    //VBE 2.0
    UINT16 OemSoftwareRev;
    UINT32 OemVendorNamePtr;
    UINT32 OemProductNamePtr;
    UINT32 OemProductRevPtr;
    char reserved[222];
    char OemData[256];
}__attribute__ ((packed))VESAINFO;


typedef struct
{
    UINT16 ModeAttributes;
    char WinAAttributes;
    char WinBAttributes;
    UINT16 WinGranularity;
    UINT16 WinSize;
    UINT16 WinASegment;
    UINT16 WinBSegment;
    UINT32 WinFuncPtr;
    short BytesPerScanLine;
    short XRes;
    short YRes;
    char XCharSize;
    char YCharSize;
    char NumberOfPlanes;
    char BitsPerPixel;
    char NumberOfBanks;
    char MemoryModel;
    char BankSize;
    char NumberOfImagePages;
    char res1;
    char RedMaskSize;
    char RedFieldPosition;
    char GreenMaskSize;
    char GreenFieldPosition;
    char BlueMaskSize;
    char BlueFieldPosition;
    char RsvedMaskSize;
    char RsvedFieldPosition;
    char DirectColorModeInfo; //MISSED IN THIS TUTORIAL!! SEE ABOVE
    //VBE 2.0
    UINT32 PhysBasePtr;
    UINT32 OffScreenMemOffset;
    short OffScreenMemSize;
    //VBE 2.1
    short LinbytesPerScanLine;
    char BankNumberOfImagePages;
    char LinNumberOfImagePages;
    char LinRedMaskSize;
    char LinRedFieldPosition;
    char LingreenMaskSize;
    char LinGreenFieldPosition;
    char LinBlueMaskSize;
    char LinBlueFieldPosition;
    char LinRsvdMaskSize;
    char LinRsvdFieldPosition;
    char res2[194];
}__attribute__ ((packed))VESA_MODEINFO;


BYTE * lpLinearFP;




/*
Palette setting only works in 8 bit modes
*/
void set_palette(unsigned char entry, unsigned char r, unsigned char g, unsigned char b)
{
   OutPortByte(0x3c8, entry);
   OutPortByte(0x3c9, r);
   OutPortByte(0x3c9, g);
   OutPortByte(0x3c9, b);
}
void VESAPutPixel(int X, int Y, BYTE Color)
{
    DEBUG();
    lpLinearFP[(Y*640)+X]=Color;
}
void putpixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
    int shorts_per_row=640*3/2+64;   // self explanatory quantities needed for
int chars_per_row=640*3+128;     // the 24 bit putpixel (note the 128 bytes 
                                 // added to the end of the line - this is
                                 // required by this video mode)

    WORD * lpLFP=(WORD *)lpLinearFP;
   if ((x & 1)==0) 
    {		// it's an even pixel
      lpLFP[y*shorts_per_row + (x*3)/2]=blue+(green<<8);
      lpLinearFP[y*chars_per_row + x*3+2]=red;
   }
    else 
    {             // it's an odd pixel
      lpLinearFP[y*chars_per_row + x*3]=blue;
      lpLFP[y*shorts_per_row + (x*3+1)/2]=green+(red<<8);
   }
    
}
void VESASetMode()
{
    const char szV86BinFile[]="F\\E:\\Vesa16";
    
    HANDLE hV86File;
    BYTE * lpV86Image;
    FILEINFO FINFO;
    UINT32 dwFileSize;
    GFS_GetFileAttributes(szV86BinFile, &FINFO);
    dwFileSize = FINFO.nFileSizeLow;

    hV86File = GFS_CreateFile( szV86BinFile, GENERIC_READ, 0, OPEN_EXISTING, 0);
    if ( hV86File == 0 )
    {
        dbgprintf("\n V86 :: Unable to open the file %s", szV86BinFile);
        printf("\n V86 :: Unable to open the file %s", szV86BinFile);
        return;
    }
    lpV86Image = (BYTE *)malloc(dwFileSize);
    if ( lpV86Image == NULL )
    {
        dbgprintf("\n V86 :: Unable to allocate memory");
        return;
    }
    if ( GFS_ReadFile(hV86File, dwFileSize, lpV86Image) == 0 )
    {
        dbgprintf("\n V86 :: Unable to read the file %s", szV86BinFile);
        printf("\n V86 :: Unable to open the file %s", szV86BinFile);
        free(lpV86Image);
        return;
    }
    
    VESAINFO * pVESAInfo = (VESAINFO *) (200 * 1024);
    VESA_MODEINFO * pModeInfo = (VESA_MODEINFO *) (210 * 1024);
    UINT32 lpProgramAddress = 100*1024;
    UINT32 dwPhysBase=0;
    memcpy((void *)lpProgramAddress, lpV86Image, dwFileSize);
    
    HTHREAD tid= (HTHREAD)V86CreateThread( 0x9000, 10000, TP_NORMAL);
    while( GetThreadInfo(GetTaskID(), tid)->dwRunState != TASK_STOPPED );
        
    UINT16 * pVideoMode = (UINT16 *) (( pVESAInfo->VideoModePtr>>16) * 16 ) +  (pVESAInfo->VideoModePtr & 0xFFFF);
    while( *pVideoMode != 0xFFFF )
    {
        dbgprintf("\n Mode:%X %dx%dx%d Phy:%X Attr:%X MemModel:%X DirColorInfo:%X", *pVideoMode, pModeInfo->XRes, pModeInfo->YRes, pModeInfo->BitsPerPixel, pModeInfo->PhysBasePtr, pModeInfo->ModeAttributes, pModeInfo->MemoryModel, pModeInfo->DirectColorModeInfo); 
        printf("\n\t Mode:%X %dx%dx%d Phy:%X Attr:%X MemModel:%X DirColorInfo:%X", *pVideoMode, pModeInfo->XRes, pModeInfo->YRes, pModeInfo->BitsPerPixel, pModeInfo->PhysBasePtr, pModeInfo->ModeAttributes, pModeInfo->MemoryModel, pModeInfo->DirectColorModeInfo); 
        if ( *pVideoMode == 0x101 )
        {
            dwPhysBase = pModeInfo->PhysBasePtr;
            break;
        }
        pVideoMode++;
        pModeInfo++;
    }
    if ( dwPhysBase )
    {
        int i,j;
        lpLinearFP = (BYTE *)KernelMap(0, (640*480)*3, (void *)dwPhysBase /*0xE0000000*/ );
        
        for (i=0;i<255;i++)
            set_palette(i,i+10,i+0,i+20);
        for(i=0;i<640;i++)
            for(j=0;j<480;j++)
                VESAPutPixel(i,j,j>0?i/j:1);
            //putpixel(i,j,200,200,200);
    }
    
}
