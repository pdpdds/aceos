/*
  Name: 		DMA Controller Driver
  Author: 			- Sam (samueldotj@yahoo.co.in)
  Date: 		16-Sep-2004 8:17AM
*/
#ifndef __DMAC_H
#define __DMAC_H

#include <TypeDefs.h>
#include <GSH.H>
#include <ObjMan.H>

#ifdef __cplusplus
    extern "C" {
#endif

struct _DMABuffer
{
    DWORD dwPhysicalAddress;
    DWORD dwVirtualAddress;
    
    DWORD dwSize;
};
typedef struct _DMABuffer DMABUFFER;
typedef DMABUFFER * LPDMABUFFER;

struct _DMAChannel
{
    LPOBJECT lpObject;
    
    BYTE bChannelNo;
    BYTE bMode;
    LPDMABUFFER lpDMABuffer;
    UINT32 dwStartTime;
};
typedef struct _DMAChannel DMACHANNEL;
typedef DMACHANNEL * LPDMACHANNEL;

#define TOTAL_DMA_CHANNELS 8
//this array is used as a spin lock key array
extern int sysDMAChannelUsage[TOTAL_DMA_CHANNELS];

#define DMA_ADDRESS_PORT_INDEX 0
#define DMA_COUNT_PORT_INDEX 1
#define DMA_PAGE_PORT_INDEX 2
#define DMA_COMMAND_REGISTER_INDEX 3
#define DMA_SINGLE_MASK_REGISTER_INDEX 4
#define DMA_WRITE_MODE_REGISTER_INDEX 5
#define DMA_FLIP_FLOP_REGISTER_INDEX 6
//this two dimensional array is used to hold the port address for all DMA Channels
//Address Port, Count Port, Page Port, Command Register, Single Mask registers, Write Mode Register, Flip Flop
extern int sysDMAChannelPort[TOTAL_DMA_CHANNELS][7];

#define DMAC1_COMMAND_REGISTER 0x08
#define DMAC2_COMMAND_REGISTER 0xD0

#define DMAC1_CLEAR_MASK_REGISTER 0x0E
#define DMAC2_CLEAR_MASK_REGISTER 0xDC

#define DMAC1_ALL_MASK_REGISTER 0x0F
#define DMAC2_ALL_MASK_REGISTER 0xDE

//Control Byte bit definitions
//Mode Selection Bits 7:6
#define DMA_DEMANDMODE  0 //00
#define DMA_SINGLEMODE  64 //01
#define DMA_BLOCKMODE   128 //10
#define DMA_CASCADEMODE 192 //11
//Address Increment/Decrement bit 5
#define DMA_ADDRESSDECREMENT    32 //1
#define DMA_ADDRESSINCREMENT    0 //0
//AutoInitialization enable bit 4
#define DMA_AUTOINIT    16 //1
#define DMA_SINGLECYCLE 0 //0
//Transfer Type bits 3:2
#define DMA_VERIFYTRANSFER      0 //00
#define DMA_WRITETRANSFER       4 //01
#define DMA_READTRANSFER        8 //10

void DMA_Init();
LPDMACHANNEL DMA_AllocateChannel(BYTE bChannelNo);
UINT32 DMA_ReleaseChannel(LPDMACHANNEL lpDMAChannel);

LPDMABUFFER DMA_AllocateBuffer(UINT32 dwSize);
UINT32 DMA_FreeBuffer(LPDMABUFFER lpBuffer);

UINT32 DMA_SetBufferInfo(LPDMACHANNEL lpDMAChannel, LPDMABUFFER lpBuffer);
UINT32 DMA_SetChannelMode(LPDMACHANNEL lpDMAChannel, BYTE bMode);

UINT32 DMA_StartTransfer(LPDMACHANNEL lpDMAChannel);
UINT32 DMA_PauseTransfer(LPDMACHANNEL lpDMAChannel);

UINT32 DMA_EnableChannel(LPDMACHANNEL lpDMAChannel);
UINT32 DMA_DisableChannel(LPDMACHANNEL lpDMAChannel);

#ifdef __cplusplus
    }
#endif

#endif

