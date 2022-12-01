/*                            C Kernel for Ace OS 
                                                        - Sam (15-Aug-2002) samueldotj@gmail.com
This is the actual kernel, the asm Kernel will initialize the system and handover the control to this C Kernel
*/
#include <VGAText.h>
#include <Memory.h>
//#include <PCI.h>
#include <DeviceDriver.h>
#include <RTC.h>
#include <HardDisk.h>
#include <Floppy.h>
#include <Processor.h>
#include <Keyboard.h>
#include <Timer.h>
#include <Beep.h>
#include <IPC.h>
#include <Partition.h>
#include <TaskMan.h>
#include <Thread.h>
#include <GSH.h>
#include <GFS.h>
#include <FAT.h>
#include <Timer.h>
#include <VFS.h>
#include <SysInfo.h>
#include <SysCall.h>
#include <Memory.h>
#include <Printer.h>
#include <PE.h>
#include <VESAInt.h>
#include <V86.h>
#include <DMA.h>

#include <syscall.h>
#include <sysinfo.h>

#include <stdio.h>
#include <multiboot.h>

#define PICU1  0x20
#define PICU2  0xA0

#define TIMER_FREQUENCY 18

UINT16 wDDDataSelector;
UINT16 wDDCodeSelector;
void PrintCPUInfo();
void PrintDriveMappingInfo();
//void PrintPCIDeviceList(struct PCIConfigurationSpace * ptrPCIDet);
void PrintHDDInfo();
void InitPIC()
{
    OutPortByte(PICU1+0, 0x11 );  //send 00010001b to ICW1-Master    
    OutPortByte(PICU1+1, 0x20 );  //send 0x20 to ICW2-Master    
    OutPortByte(PICU1+1, 0x4 );   //send 00000100b to ICW3-Master    
    OutPortByte(PICU1+1, 0x1 );   //send 00000001b to ICW4-Master
    
    OutPortByte(PICU1+1, 0 );     //No Mask ; MASK - MASTER (0= Ints ON)
    
    OutPortByte(PICU2+0, 0x11 );  //send 00010001b to ICW1 - SLAVE
    OutPortByte(PICU2+1, 0x28 );  //ICW2 - SLAVE
    OutPortByte(PICU2+1, 0x02 );  //send 00000010b to ICW3 - SLAVE
    OutPortByte(PICU2+1, 0x01 );  //send 00000010b to ICW4 - SLAVE

    OutPortByte(PICU2+1, 0 );     //No Mask ; MASK - SLAVE (0= Ints ON)
}

void ThreadTest1()
{
    while(1)
        printf("!");
}
void ThreadTest2()
{
    while(1)
        printf("@");
}

void StartCKernel(multiboot_header_t *header, multiboot_info_t *info)
{
    SYSTEMTIME stLocalTime;
    __asm__ __volatile__ ("cli");
    
    InitMemManPhaseII();
    
    DEBUG();
    DetectProcessor();  	// Detect processor features and preserve result
    SetExceptionHandlers();
    InitPIC();                  // Initializing PIC to generate interrupts at IDT 32

    VGA_Fill(0, 0,24, 79, ' ', 'C');       //Clear the screen
    printf("Ace OS V%X.%X.%X",ACE_MAJOR, ACE_MINOR, ACE_REVISION);
    VGA_Fill(0, 0, 0 ,79, 0x3a, 'A');      //Fill with some color    
    printf("\n\rInitializing Timer(%dHz):: ",TIMER_FREQUENCY);   
    printf("%s", InitTimer(TIMER_FREQUENCY)?"Success":"Failed" );
    InitPrinter(0);     //init printer port 0 which will be used as debug port
    InitTaskManager();
    __asm__ __volatile__ ("sti");
    
    PrintCPUInfo();
    
    GetLocalTime(&stLocalTime);
    
    /*void * x[100];
    int j=0;
    for(j=0;j<100;j++)
        x[j]= malloc( 70*j );
    for(j=0;j<100;j++)
        free(x[j]);*/
    
    printf("\n\rReal Time Clock Settings %X-%X-%X",stLocalTime.wDay,stLocalTime.wMonth,stLocalTime.wYear);
    printf("\n\rInitializing Keyboard :: ");
    printf("%s",InitKeyboard()?"Success":"Failed" );
    //printf("\n\rInitializing DMAC 8237::");
    /*printf("\n\rDetecting PCI Devices :: ");
    UINT16 PCIDevices=InitPCIDevices();
    printf("%d device(s) found", PCIDevices);
    if ( PCIDevices )
    {
        printf("\n\r Device Vendor Class SubClass");
        EnumeratePCIDevices(PrintPCIDeviceList);
    }*/
    
    //SetTimerTickStatus(1);      //let the Timer tick
    
    //printf("\n\rInitializing FDD :: " );
    //Floppy_Initialize();
    printf("\n\rDetecting ATA Devices :: " );
    HardDisk_Initialize();
    printf(" %d device(s) found", HDD_GetNoOfDevices());
    if ( HDD_GetNoOfDevices() )
        PrintHDDInfo();
    FillDevicePathLookupTable();
    printf("\n\rDrive Mappings :: ");
    PrintDriveMappingInfo();
    printf("\n\rInitializing GFS :: ");
    printf("%s",GFS_Init()?"Success":"Failed");
    printf("\n\rInitializing VFS :: ");
    printf("%s", VFS_Init()?"Success":"Failed");
    printf("\n\rInitializing FAT File System :: ");
    printf("%s", FAT_Init()?"Success":"Failure" );
    
    printf("\n\rInitializing System Calls :: ");
    InitSysCallTable();
    printf("done");

     
    
    /*PE_IMAGE_LOAD_INFO PI;
    if ( PE_LoadFile("C:\\","CShell.exe",  &PI) )
    {
        void (*DriverEntry)();
        PI.lpLoadedAddress=PI.lpImage;
        INT32 Delta=(UINT32)PI.lpLoadedAddress - (UINT32) PI.lpPreferedLoadAddress;
        PE_RelocateImage(Delta, PI.lpPEFile, PI.lpImage, PI.lpRelocationSection);
        DriverEntry=PE_GetExportSymbolByName(PI.lpImage, PI.lpExportDirTable,"DriverEntry" );
        printf("\n\r Driver Entry %x",DriverEntry);
        if (DriverEntry)
            CreateThread( DriverEntry,  5000, 0x8, 0x10, 0x202, TASK_RUNNABLE, TP_NORMAL, FALSE, TRUE);
    }*/
   // GFS_DeleteFile("C:\\test.exe");
   // printf("\n\r Creating task :: ");
    CreateTask("C:\\","Shell.exe");
   // CreateTask("C:\\","test.exe");
   // printf("done");
   
   
    //for(;;);
   // while(1);
    //DD_LoadDriver("F\\C:\\StdVGA.dll");
    
    /*FILEINFO FInfo;
    HANDLE hFile = NULL;
    hFile =   GFS_FindFirstFile("C:\\*.*", &FInfo);
    if (  hFile == NULL )
    {
        printf("\n\r Unable to find the file. Find first returns null");
    }
    else
    {
        printf("\n\r First returns [%s]", FInfo.cAlternateFileName);
        while( GFS_FindNextFile(hFile, &FInfo) )
        {
            printf("\n\r Next returns [%s]", FInfo.cAlternateFileName);
        }
        printf("\n\r next finished");
    }*/
    /*hFile = GFS_CreateFile("\\\\Harddisk0\\Partition0\\Test", GENERIC_READ, 0, OPEN_EXISTING, 0);
    hFile = GFS_CreateFile("c:\\Sam\\Sam\\31.ca", GENERIC_READ, 0, OPEN_EXISTING, 0);
    if ( hFile == NULL )
    {
        printf("\n\r Open file failed");
    }
    else
    {
        char buffer[1000];
        int c=GFS_ReadFile(hFile, 1000, buffer);
        while ( c!=0 )
        {
            if ( c )
            {
                int i;
                for(i=0;i<c;i++)
                    printf("%c",buffer[i]);
            }
            c=GFS_ReadFile(hFile, 1000, buffer);
        }
    }*/ 
    
    /*static volatile struct _SYSTEM_INFO SysInfo;
    printf("\n\r %X Para %x",GetSystemInfo, &SysInfo);
    DEBUG();
    __asm__ __volatile__ ("push $1;\
    push %2;\
                movl %0, %%eax;\
                int %1"::"i"(GETSYSTEMINFO),"i"(SYSCALL_INT),"m"(&SysInfo) );
    DEBUG();    
    printf("\n\rSystem Information - CPU %s", SysInfo.CPU_VendorID);
    printf("Memory - Available %ld  Total %ld ", SysInfo.MEM_PhysicalAvailable, SysInfo.MEM_PhysicalTotal);*/
        
    printf("\n\rCreating Timer to delay 1 sec : ");
    LPTIMER lpTim = CreateTimer( 1 * 18.5 , NULL );
    WaitForSingleObject( lpTim->lpObject );
    
    /*printf("\n\rok");
    printf("\n\rLoading CShell.exe... : F\\C:\\CShell.exe");
    //DD_LoadDriver("F\\C:\\CShell.exe");
    printf("done");
    */
    
    
    
    //VESASetMode();
    printf("\n\rKernel :: All done.");
    //sys_CurThread->dwRunState=TASK_STOPPED;
    //RunScheduler();
    
    //CreateThread( ThreadTest1,  1000, 0x8, 0x10, 0x202, TASK_RUNNABLE, TP_TIME_CRITICAL, 0, 1);
    //CreateThread( ThreadTest2,  1000, 0x8, 0x10, 0x202, TASK_RUNNABLE, TP_IDLE, 0, 1);
    int i=0;
    while(1)
    {
        LPMESSAGE x=IPC_ReceiveMessage( sys_CurThread->lpMessageBox );
        if ( x )
        {
            //printf("\n\r %X %X %X", x->dwMessageID, x->dwPara1, x->dwPara2);
            printf("[%X]", GetLastError() );
          
            i++;
            if ( i > 5 )
            {
                DropTimer( lpTim );
            }
        }
    }
}
//----------------------------------------Other functions------------------------------
void PrintCPUInfo()
{
    struct _SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    printf("\n\r%s", SysInfo.CPU_VendorID);
    BYTE CPUBrand = ( SysInfo.CPU_FeatureFlags[0] << 24 ) >> 24;
    switch ( CPUBrand )
    {
        case 0x1:
            printf(" Celeron Processor");
            break;
        case 0x2:
            printf(" Pentium III Processor");
            break;
        case 0x3:
            printf(" Pentium III Xeon Processor");
            break;
        case 0x4:
            printf(" Pentium 4 Processor");
            break;
    }
    printf(" CPU Family %d Model %d Steping %d.", (SysInfo.CPU_Signature<<20)>>28,(SysInfo.CPU_Signature<<24)>>28,(SysInfo.CPU_Signature<<28)>>28 );
    //printf("\n\rProcessor Features %X %X %X", SysInfo.CPU_FeatureFlags[0], SysInfo.CPU_FeatureFlags[1], SysInfo.CPU_FeatureFlags[2]);
    printf("\n\rVirtual Page Size = %ldBytes. Physical Memory - Total %ldMB, Available %ldMB.",SysInfo.MEM_PageSize, mm_PhysicalTotal, mm_PhysicalAvailable )  ;
}
/*	

void PrintPCIDeviceList(struct PCIConfigurationSpace * ptrPCIDet)
{
    //printf("\n\r %5X %6X %X %X ",ptrPCIDet->VendorID ,ptrPCIDet->DeviceID, ptrPCIDet->ClassCode, ptrPCIDet->SubClass );
    //return 0;
    if ( ptrPCIDet->ClassCode < __PCI_MaxClass && ptrPCIDet->ClassCode !=0 )
    {
        printf("\n\r %s",PCIClassDetails[ptrPCIDet->ClassCode].Description);
        if ( ptrPCIDet->SubClass != 0 )
        {
            struct PCISubClass * PCISubCl = PCIClassDetails[ptrPCIDet->ClassCode].SubClass;
                
            BYTE SClCode=0;
            while( !(PCISubCl[SClCode].SubClassCode==0 && PCISubCl[SClCode].Description == 0 ) )
            {
                if ( ptrPCIDet->SubClass == SClCode )
                {
                    if ( PCISubCl != NULL )
                        printf(" - %s",PCISubCl[SClCode].Description);
                    break;
                }
                SClCode++;
            }
        }
    }
}*/
void PrintHDDInfo()
{
    int TotHDD=HDD_GetNoOfDevices();
    LPHDD HDDInfo;
    BYTE i;

    for (i=0;i<TotHDD;i++)
    {
        
        HDDInfo=(LPHDD)sysHDDArray[i];
        if ( HDDInfo != NULL )
        {
            UINT16 j;
            printf("\n\r %s Device ( Harddisk%d ) :: ", HDDInfo->bDeviceNumber ? "Slave ":"Master", i);
            if ( HDDInfo->szModelNumber[0]==0 )
                printf(" N/A ");
            else
                for(j=0;j<20;j++)
                    VGA_PutCh(HDDInfo->szModelNumber[j]);
            printf(" - " );
            if ( HDDInfo->szSerialNumber[0]==0 )
                printf(" N/A ");
            else
                for(j=0;j<20;j++)
                    VGA_PutCh(HDDInfo->szSerialNumber[j]);
            printf("\n\r Cylinders %d Heads %d Sectors %d. LBA Sectors %ld",HDDInfo->wCHSCylinderCount,HDDInfo->wCHSHeadCount,HDDInfo->wCHSSectorCount,HDDInfo->dwLBACount);
            }
    }
}
        
void PrintDriveMappingInfo()
{
    int bTotalDrives = GetTotalDrives();
    int i;
    for(i=0;i<bTotalDrives;i++)
    {
        printf("\n\r %c: (%s) [%s]", 'A'+i, sysDevicePathLookupTable[i].szDevicePath, GetPartititionTypeString(PART_TYPE(&sysDevicePathLookupTable[i].Part)) );
    }
}


