/*                            C Kernel for Ace OS 
                                                        - Sam (15-Aug-2002) samuelhard@yahoo.com
This is the actual kernel, the asm Kernel will initialize the system and handover the control to this C Kernel
*/
#include <StdIO.h>
#include <VGAText.h>
#include <MemMan.h>
#include <PCI.h>
#include <Collect.h>
#include <DeviceDriver.h>
#include <RTC.h>
#include <HardDisk.h>
#include <Processor.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <Timer.h>
#include <Beep.h>
#include <HMess.h>
#include <Partition.h>
#include <FAT.h>

#define __PCI_MaxClass 0x12

__GSH SystemStorageHandler;
HardDiskHandler * __SysHDDHandler;

void PrintCPUInfo();
BYTE PrintPCIDeviceList(struct PCIConfigurationSpace * ptrPCIDet);
void PrintHDDInfo();
extern "C" void StartCKernel(void)
	{BYTE i;
	Printf("ACE OS V1.0");
	Fill(0, 0, 0 ,79, 0x3a, 'A');
	
	InitDD();
	PrintCPUInfo();
	Printf("\n\rInitializing Keyboard : ");
	Printf("%s", InitKeyboard()?"Sucess":"Failed" );
	Printf("\n\rInitializing Mouse : ");
	if ( InitMouse() )
		{Printf("Sucess. ");
		Printf("\n\r   %s Mouse is connected to %s Port %d",GetMouseType(),GetMousePortType(),GetMousePortNo());
		}
	else
		Printf("Failed" );
	Printf("\n\rDetecting PCI Devices :: ");
	/*UINT16 PCIDevices=InitPCIDevices();
	Printf("%d device(s) found", PCIDevices);
	if ( PCIDevices )
		{Printf("\n\r Device Vendor Class SubClass");
		EnumeratePCIDevices(PrintPCIDeviceList);
		}*/
	
	__SysHDDHandler = (HardDiskHandler *)AllocHeap(sizeof(HardDiskHandler));
	Printf("\n\rDetecting ATA Devices :: " );
	__SysHDDHandler->Initialize();
	Printf(" %d device(s) found",__SysHDDHandler->GetTotalDevices());
	if ( __SysHDDHandler->GetTotalDevices() )
		PrintHDDInfo();
	Printf("\n\rInitializing FAT File System ::\n\r");
	InitFATFileSystem();
	/*UINT16 HandleID=FATFileOpen("C:\\sam\\sam\\386intel.txt",0);
	Printf("\n\r Handle ID %d",HandleID );
	BYTE Buf[500];
	while ( !FATIsEndOfFile(HandleID) )
		{int z=FATReadFile(HandleID,500,Buf) ;
		for ( int i=0;i<z;i++)
			PutCh(Buf[i]);
		if ( z==0  ) 
			break;
		}*/
	Printf("\n\r\n\r %d Device Driver(s) Installed. DD List :: ",GetTotalDDInstalled());
	for(i=0; i<GetTotalDDInstalled();i++)
		Printf("\n\r %s - %s",GetDDInfo(i)->DriverName, GetDDInfo(i)->Company);
	Printf("\n\rKernel :: All done. Listening for Hardware Messages...\n\r ");
	while(1)
		{
		struct HMessage * HM = GetNextHardwareMessage();
		if ( HM )
			{struct DeviceDriverInfo * DDInfo;
			DDInfo = GetDDInfo(HM->HardwareID-1);	//getting Device Driver Information
			if ( DDInfo )	// if Device Driver registered executing Handler
				DDInfo->HardwareHandler(HM->SubSystem,HM->Para1,HM->Para2);
			}
		if ( KeysAvailable() > 0 )
			{BYTE ScanCode=ReadKeyboard();
			Printf("%c", ScanCode<0x49?ConvertScanCode(ScanCode,0):' ');
			}
		}

	}
//----------------------------------------Other functions------------------------------
void PrintCPUInfo()
	{Printf("\n\r%s", _CPU_VendorID);
	BYTE CPUBrand = ( _CPU_Features_EBX << 24 ) >> 24;
	switch ( CPUBrand )
		{case 0x1:
			Printf(" Celeron Processor");
			break;
		case 0x2:
			Printf(" Pentium III Processor");
			break;
		case 0x3:
			Printf(" Pentium III Xeon Processor");
			break;
		case 0x4:
			Printf(" Pentium 4 Processor");
			break;
		}
	Printf(" CPU Family %d Model %d Steping %d.", (_CPU_Signature<<20)>>28,(_CPU_Signature<<24)>>28,(_CPU_Signature<<28)>>28 );
	Printf("\n\rPhysical Memory %d KB RAM installed.", MemoryAmount);

	}
BYTE PrintPCIDeviceList(struct PCIConfigurationSpace * ptrPCIDet)
	{Printf("\n\r %5X %6X ",ptrPCIDet->VendorID ,ptrPCIDet->DeviceID );
	if ( ptrPCIDet->ClassCode < __PCI_MaxClass )
		{Printf(" %s",PCIClassDetails[ptrPCIDet->ClassCode].Description);
		if ( ptrPCIDet->SubClass != 0 )
			{struct PCISubClass * PCISubCl = PCIClassDetails[ptrPCIDet->ClassCode].SubClass;
			
			BYTE SClCode=0;
			while( !(PCISubCl[SClCode].SubClassCode==0 && PCISubCl[SClCode].Description == 0 ) )
				{if ( ptrPCIDet->SubClass == SClCode )
					{if ( PCISubCl != NULL )
						Printf(" - %s",PCISubCl[SClCode].Description);
					break;
					}
				SClCode++;
				}
			}
		}
	}
void PrintHDDInfo()
	{int TotHDD=__SysHDDHandler->GetTotalDevices();
	__HDDInfo * HDDInfo;
	BYTE Key[3]={'H','0',0};

	for (BYTE i=0;i<TotHDD;i++)
		{
		HDDInfo=(struct __HDDInfo *)__SysHDDHandler->GetHDDInfo(Key);
		if ( HDDInfo != NULL )
			{UINT16 i;
			Printf("\n\r %s Device ( %s ) :: ", HDDInfo->DeviceNumber ? "Slave ":"Master", Key);
			if ( HDDInfo->ModelNumber[0]==0 )
				Printf(" N/A ");
			else
				for(i=0;i<20;i++)
					PutCh(HDDInfo->ModelNumber[i]);
			Printf(" - " );
			if ( HDDInfo->SerialNumber[0]==0 )
				Printf(" N/A ");
			else
				for(BYTE i=0;i<20;i++)
					PutCh(HDDInfo->SerialNumber[i]);
			Printf("\n\r Cylinders %d Heads %d Sectors %d. LBA Sectors %ld",HDDInfo->CHSCylinderCount,HDDInfo->CHSHeadCount,HDDInfo->CHSSectorCount,HDDInfo->LBACount);
			}
		Key[1]++;
		}
	}



