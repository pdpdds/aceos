/*
  Name: 		Floppy Driver for Ace OS
  Author: 			- Sam (samueldotj@yahoo.co.in)
  Date: 		14-Sep-2004 9:58PM  
*/
#ifndef __FLOPPY_H
#define __FLOPPY_H

#include <TypeDefs.h>
#include <GSH.H>


#ifdef __cplusplus
    extern "C" {
#endif

#define FLOPPY_INTERRUPT_NUMBER (32+6)
        
extern volatile BOOLEAN FDD_InterruptReceived;
        
        
struct _FDD
{
    BYTE bIORegisterIdx;        /* Index Number of IO Resources and IRQ */
    BYTE bIRQ;

    BYTE bDeviceNumber;		/* 0 - Master, 1 - Slave */
    
    BYTE bMode;			/* 0 - CHS Mode, 1-LBA Mode */

    UINT16 wCHSHeadCount;
    UINT16 wCHSCylinderCount;
    UINT16 wCHSSectorCount;
    UINT32 dwLBACount;   	/* Used only in LBA mode */

    UINT16 wBytesPerSector;
};
typedef struct _FDD FDD;
typedef FDD * LPFDD;

extern struct ListNode * sysFDDs;

extern void (*_FloppyInterruptHandler)();               //0x6 interrupt handler

void Floppy_Initialize();

BYTE FDD_GetNoOfDevices();
UINT32 FDD_GetDeviceParameters(LPCSTR szDevicePath, BYTE * lpBuffer);
UINT32 FDD_Reset(LPCSTR szDevicePath);
UINT32 FDD_Status(LPCSTR szDevicePath);

UINT32 FDD_Read(BYTE bDeviceNo,  LPGSHIOPARA lpIOPara, BYTE * lpBuffer);
UINT32 FDD_Write(BYTE bDeviceNo,  LPGSHIOPARA lpIOPara, BYTE * lpBuffer);
UINT32 FDD_Verify(BYTE bDeviceNo,  LPGSHIOPARA lpIOPara);

#ifdef __cplusplus
    }
#endif
            
#endif

