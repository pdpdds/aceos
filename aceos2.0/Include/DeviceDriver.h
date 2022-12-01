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

enum
{
    DD_R_MEMORY=1,          //Physical Memory
    DD_R_IO,                //IO Port
    DD_R_DMA,               //DMA Channel
};
            
UINT32 DD_LoadDriver(char * ModulePath);
UINT32 DD_AllocateResource(UINT16 wResourceType, void * lpStarting, UINT32 Size);
#ifdef __cplusplus
    }
#endif
#endif
