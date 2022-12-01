/* 		Device Driver functions ACE OS kernel		
			-created by Samuel (04-Sep-2002)	
			samuelhard@yahoo.com
*/
#include <PCI.h>
#include <Collect.h>
#include <DeviceDriver.h>
#include <StdIO.h>
#include <FAT.h>
#include <Heap.h>

#define __PCI_MaxClass 0x12
#define TOTAL_GENERIC_DEVICES 10
char GenericDevices[TOTAL_GENERIC_DEVICES][15]=
	{
		"Timer",
		"Keyboard",
		"Serial Port 1",
		"Serial Port 2",
		"Parallel Port",
		"Harddisk",
		"Floppy",
		"FPU",
		"File System",
		"Others"
	};
Collection <struct DeviceDriverInfo *> __SysDeviceDrivers;
void InitDD()
	{__SysDeviceDrivers.Initialize();
	}
//This function loads the dynamic kernel's executive and then returns the LoadAddress
UINT32 LoadDeviceDriver(char * ModulePath)
    {void (*DeviceDriverEntry)();
	struct DirEntry DE;
	if ( GetDirectoryEntry(ModulePath, &DE) == NULL )
		{Printf("\n\rDeviceDriver:: LoadDeviceDriver() failed. Unable to open module %s",ModulePath );
		return 0;
		}
    UINT16 HandleID=FATFileOpen(ModulePath,0);
    if ( HandleID == 0 )
		{Printf("\n\rDeviceDriver:: LoadDeviceDriver() failed. Unable to open module %s",ModulePath );
		return 0;
		}
	BYTE * ptrLoadAddress = (BYTE *)AllocHeap(DE.FileSize+2);
	if ( FATReadFile(HandleID,DE.FileSize,ptrLoadAddress) != DE.FileSize )
		{Printf("\n\rDeviceDriver:: LoadDeviceDriver() failed. Unable to read module %s",ModulePath );
		return 0;
		}

	DeviceDriverEntry=(void (*)())(ptrLoadAddress-0x19000);
	DeviceDriverEntry();
    }
BYTE GetTotalDDInstalled()
	{return __SysDeviceDrivers.Count();
	}
struct DeviceDriverInfo * GetDDInfo(BYTE HardwareID)
	{return __SysDeviceDrivers.Item(HardwareID);
	}
BYTE RegisterDeviceDriver(UINT16 VendorID, UINT16 DeviceID, BYTE GenericDevice, char * DriverName, char * Company, void (*HardwareHandler)(UINT16 SubSystem,UINT32 Para1, UINT32 Para2))
	{
	BYTE i, DeviceExist=False;
	struct DeviceDriverInfo * DDInfo;
	
	//checking existing device drivers

	if ( VendorID == 0 || DeviceID == 0 )
		{if ( GenericDevice > TOTAL_GENERIC_DEVICES )
			return 0;
		}
	else
		{for(i=0;i<__SysDeviceDrivers.Count();i++)
			if ( __SysDeviceDrivers.Item(i)->VendorID == VendorID && __SysDeviceDrivers.Item(i)->DeviceID == DeviceID )
				return 0;
		/*checking whether PCI device is exist
		for( i=0; i<__SysPCIDevices.Count(); i++ )
			{struct PCIConfigurationSpace * ptrPCIDet;
			ptrPCIDet = &__SysPCIDevices.Item(i)->PCIConfDetails;
			if ( ptrPCIDet->VendorID==VendorID && ptrPCIDet->DeviceID==DeviceID )
				{DeviceExist=True;
				break;
				}
			}*/
		if ( !DeviceExist )
			return 0;
		}

	DDInfo = (struct DeviceDriverInfo * )AllocHeap(sizeof(struct DeviceDriverInfo));
	if ( DDInfo == NULL )
		return 0;
	DDInfo->HardwareID=__SysDeviceDrivers.Count()+1;
	DDInfo->HardwareHandler=(void (*)(UINT16, UINT32, UINT32))((UINT32)HardwareHandler-KERNEL_LOAD_ADDRESS);
	DDInfo->VendorID=VendorID;
	DDInfo->DeviceID=DeviceID;
	DDInfo->GenericDevice=GenericDevice;
	StrCpy(DDInfo->DriverName, DriverName);
	StrCpy(DDInfo->Company, Company);	
	__SysDeviceDrivers.Add(DDInfo);
	return DDInfo->HardwareID;
	}
char * GetGenericDeivceName(BYTE GenericDevice)
	{if ( GenericDevice > TOTAL_GENERIC_DEVICES ) 
		return 0;
	return GenericDevices[GenericDevice];
	}
