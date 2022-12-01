/*------------------------Device Driver Functions-----------------------------
				Created by Samuel on 04-Sep-2002
				samuelhard@yahoo.com
*/
#ifndef __DEVICE_DRIVER_H
#define __DEVICE_DRIVER_H

#include <TypeDefs.h>
#ifdef __cplusplus
	extern "C" {
#endif
struct DeviceDriverInfo
	{BYTE HardwareID;	/* This is allocated by the kernel*/
	UINT16 VendorID;	/* PCI Device Vendor ID */
	UINT16 DeviceID;	/* PCI Device ID that the driver can support*/
	BYTE GenericDevice;	
	char DriverName[30]; 	/* Driver Name with NULL*/
	char Company[30];	/* Company Name with NULL*/
	void (*HardwareHandler)(UINT16 SubSystem,UINT32 Para1, UINT32 Para2); 
	};
void InitDD();
KERNELDLL UINT32 LoadDeviceDriver(char * ModulePath);
/*This function will return number of installed device drivers installed.*/
KERNELDLL BYTE GetTotalDDInstalled();
/*This function register the device driver and allocate hardware id for the DD */
KERNELDLL BYTE RegisterDeviceDriver(UINT16 VendorID, UINT16 DeviceID, BYTE GenericDevice, char * DriverName, char * Company, void (*HardwareHandler)(UINT16 SubSystem,UINT32 Para1, UINT32 Para2));
/*This function will return the DDInfo for a particular device. Only kernel must use this function*/
KERNELDLL struct DeviceDriverInfo * GetDDInfo(BYTE HardwareID);
KERNELDLL char * GetGenericDeivceName(BYTE GenericDevice);

#ifdef __cplusplus
	}
#endif
#endif
