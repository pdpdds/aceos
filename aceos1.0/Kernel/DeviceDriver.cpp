/* 		Device Driver functions ACE OS kernel		
			-created by Samuel (04-Sep-2002)	
			samuelhard@yahoo.com
*/
#include <PCI.h>
#include <Collect.h>
#include <DeviceDriver.h>

#define __PCI_MaxClass 0x12
#define TOTAL_GENERIC_DEVICES 7

Collection <struct DeviceDriverInfo *> __SysDeviceDrivers;
void InitDD()
	{__SysDeviceDrivers.Initialize();
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

	if ( GenericDevice != 0 )
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
char GenericDevices[][15]=
	{
		"Timer",
		"Keyboard",
		"Serial Port 1",
		"Serial Port 2",
		"Parallel Port",
		"Harddisk",
		"Floppy",
		"FPU"
	};
char * GetGenericDeivceName(BYTE GenericDevice)
	{if ( GenericDevice > TOTAL_GENERIC_DEVICES ) 
		return 0;
	return GenericDevices[GenericDevice];
	}

