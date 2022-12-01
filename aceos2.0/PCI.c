/*  	Created on 14-June-2002 by Sam - samuelhard@yahoo.com


	This file provide interface to detect the PCI devices present
	in the system and also provide the IO address, IRQ information.

	Supports PCI 2.2
*/

#include <IO.h>
#include <PCI.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

/* This is the 256 byte PCI Configuration Space.
The first 16 bytes are PreDefined Header and others are depends on
the Header type 0 - NonBridge 1- Bridge 2 - PCCard
*/
/* This function fills the PCICfg structure using the configuration method 1
*/
BYTE GetPCIConfigurationSpace(BYTE Bus, BYTE Device, BYTE Function, struct PCIConfigurationSpace * PCICfg)
{
    int Reg;
    int First=1;
    for ( Reg=0;Reg < sizeof(struct PCIConfigurationSpace) / sizeof(DWORD); Reg++ )
    {
        DWORD Addr,Value;
        Addr = 0x80000000L |
               ( ((DWORD) Bus) << 16 ) |
               ( ((DWORD)( Device << 3 ) | Function ) << 8) |
               ( Reg << 2 );
        OutPortDWord( PCI_CONFIG_ADDRESS, Addr);
        Value = InPortDWord(PCI_CONFIG_DATA);
        
        Addr = InPortDWord(PCI_CONFIG_ADDRESS);
        ((DWORD *)PCICfg)[Reg]=Value;
        OutPortDWord( PCI_CONFIG_ADDRESS, 0);
        if ( First )
        {
            if ( Value == 0xFFFFFFFF )
                return 0;
            First=0;
        }
    }
    return Reg;
}
/* this function will initialize the _SysPCIList
Note you have to deallocate all the nodes before refreshing.
returns Total PCI Devices found on the system or 0 on error*/
struct ListNode * __SysPCIDevices=NULL;
UINT16 InitPCIDevices()
{
    UINT16 TotDev=0; 
    BYTE Bus, Device,Function;
    struct PCIConfigurationSpace PCICfg;
    struct PCIDeviceDetails *newPCIDev;	

    for (Bus=0;Bus<0xFF;Bus++)
        for(Device=0;Device<32;Device++)
            for(Function=0;Function<8;Function++)
            {
                BYTE Ret = GetPCIConfigurationSpace(Bus, Device, Function, &PCICfg);
                if ( !Ret )
                    continue;
                if (!( PCICfg.VendorID==0xFFFF || PCICfg.DeviceID==0xFFFFFFFF ))
                {
                    newPCIDev = ( struct PCIDeviceDetails * ) malloc(sizeof( struct PCIDeviceDetails ));
                    if ( newPCIDev == NULL )
                    {
                        printf("\n\r PCI Init ::  Memory Allocation Error");
                        return 0;
                    }
                    newPCIDev->Bus = Bus;
                    newPCIDev->Device = Device;
                    newPCIDev->Function = Function;
                    memcpy( &newPCIDev->PCIConfDetails , &PCICfg, sizeof( struct PCIConfigurationSpace ) );
                    List_Add(&__SysPCIDevices,"",newPCIDev); //adding to the linked list
                    TotDev++;
                }
            }
    return TotDev;
}
void EnumeratePCIDevices( BYTE (*CallBackFn) (struct PCIConfigurationSpace * )  )
{
    UINT16 Count=List_Count(__SysPCIDevices),i;
    for(i=0; i<Count; i++ )
    {
        struct PCIConfigurationSpace * ptrPCIDet;
        ptrPCIDet = &( ( struct PCIDeviceDetails * )List_GetData(__SysPCIDevices,"",i))->PCIConfDetails;
        if ( ptrPCIDet != NULL )
            CallBackFn(ptrPCIDet);
    }
}
