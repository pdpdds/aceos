/*
  Name: 		DMA Controller Management
  Author: 			        - Samuel
  Description: 	This file contains neccessary routines to manage the DMA controller
  Date: 20-Sep-04 10:58PM
*/
#include <DMA.h>
#include <Memory.h>
#include <SysInfo.h>
#include <IO.h>

/* Defines for accessing the upper and lower byte of an integer. */
#define LOW_BYTE(x)         (x & 0x00FF)
#define HI_BYTE(x)          ((x & 0xFF00) >> 8)

//Define macros for converting Linear Address to Segment:Offset and vice versa
#define LP_SEG(x)       ( x & 0xFFFF0000 )
#define LP_OFF(x)       ( x & 0x0000FFFF )
#define LP_PAGE(x)      ( x >> 16 )

//this array is used as a spin lock key array ************ Currently it is not implemented -----take care
int sysDMAChannelUsage[TOTAL_DMA_CHANNELS];

//this internal variable is used to track which channel is masked and which one is not
BYTE sysDMAChannelMask;

//this two dimensional array is used to hold the port address for all DMA Channels
//Address Port, Count Port, Page Port, Command Register, Single Mask registers, Write Mode Register, Flip Flop
int sysDMAChannelPort[TOTAL_DMA_CHANNELS][7]=
{
    {0x00,  0x01,   0x87,   0x08,   0x0A,   0x0B,   0x0C},
    {0x02,  0x03,   0x83,   0x08,   0x0A,   0x0B,   0x0C},
    {0x04,  0x05,   0x81,   0x08,   0x0A,   0x0B,   0x0C},
    {0x06,  0x07,   0x82,   0x08,   0x0A,   0x0B,   0x0C},
    {0xC0,  0xC1,   0x8f,   0xD0,   0xD4,   0xD6,   0xD8},
    {0xC2,  0xC3,   0x8B,   0xD0,   0xD4,   0xD6,   0xD8},
    {0xC4,  0xC5,   0x89,   0xD0,   0xD4,   0xD6,   0xD8},
    {0xC6,  0xC7,   0x8A,   0xD0,   0xD4,   0xD6,   0xD8}
};
//initialize the internal variables and reset the DMA controller
void DMA_Init()
{
    int i;
    //set all DMA channels are free
    for(i=0;i<TOTAL_DMA_CHANNELS;i++)
        sysDMAChannelUsage[i]=0;
    sysDMAChannelUsage[4]=1;// Slave DMA Controller
    
    OutPortByte(DMAC1_COMMAND_REGISTER, 0x10 );//enable DMAC1 Controller
    OutPortByte(DMAC2_COMMAND_REGISTER, 0x10 );//enable DMAC2 Controller
    
    //----**** wait here for 25ms
    
    OutPortByte(DMAC1_ALL_MASK_REGISTER, 0xFF); //disable all channels in primary controller
    OutPortByte(DMAC2_ALL_MASK_REGISTER, 0xFF); //disable all channels in secondary controller
    
    sysDMAChannelMask=0xFF;     //all channels are masked
}
LPDMACHANNEL DMA_AllocateChannel(BYTE bChannelNo)
{
    LPDMACHANNEL lpResult;
    
    if( bChannelNo >= TOTAL_DMA_CHANNELS )      //check for correct argument 
        return NULL;
    if( sysDMAChannelUsage[bChannelNo] )     //check whether the channel is free or not
        return NULL;
    
    //allocate memory for the DMACHANNEL -- this memory should be free in the DMAReleaseChannel call
    lpResult = malloc(sizeof(DMACHANNEL));
    if ( lpResult==NULL )                       //memory allocation error
        return NULL;
    //Initialize the structure
    lpResult->bChannelNo = bChannelNo;
    lpResult->bMode = DMA_SINGLEMODE | DMA_READTRANSFER;
    lpResult->lpDMABuffer = NULL;
    lpResult->dwStartTime = 0;
    
    sysDMAChannelUsage[bChannelNo] = 1;         //occupied
    return lpResult;                            //return the result
}
UINT32 DMA_ReleaseChannel(LPDMACHANNEL lpDMAChannel)
{
    if ( lpDMAChannel )
    {
        if ( lpDMAChannel->bChannelNo >= TOTAL_DMA_CHANNELS )
            return ERROR_DEV_NOT_EXIST;
        sysDMAChannelUsage[lpDMAChannel->bChannelNo]=0;     //free
        //issue command here to stop and clear any DMA transfer in the channel
        DMA_DisableChannel(lpDMAChannel);
        
        free(lpDMAChannel);                     //free the structure
        return ERROR_SUCCESS;
    }
    else
        return ERROR_INVALID_HANDLE;
}
/* This function will try to create a DMA buffer below 1MB*/
LPDMABUFFER DMA_AllocateBuffer(UINT32 dwSize)
{
    DWORD dwReqPage;
    LPDMABUFFER lpDMABuffer;
    
    lpDMABuffer = (LPDMABUFFER) malloc (sizeof(DMABUFFER));
    if ( lpDMABuffer )
    {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return 0;
    }
    lpDMABuffer->dwPhysicalAddress=0;
    lpDMABuffer->dwVirtualAddress=0;
    lpDMABuffer->dwSize=dwSize;
    
    //calculate the required page size
    dwReqPage = dwSize/VASM_PAGE_SIZE + (dwSize%VASM_PAGE_SIZE)?1:0;
    //allocate physical memory below 1 mb
    lpDMABuffer->dwPhysicalAddress = VASM_AllocateLowPhysicalPages( 0, (1024*1024)/VASM_PAGE_SIZE, dwReqPage ) * VASM_PAGE_SIZE;
    if ( lpDMABuffer->dwPhysicalAddress == 0 )
    {
        //clean-up and return error
        free(lpDMABuffer);
        return 0;               
    }
    //allocate virtual address
    lpDMABuffer->dwVirtualAddress = VirtualAlloc(GetTaskID(), 0, dwReqPage, MEM_RESERVE, 3);
    if ( lpDMABuffer->dwVirtualAddress == 0 )
    {
        //clean-up and return error
        VASM_FreeLowPhysicalPages( lpDMABuffer->dwPhysicalAddress, dwReqPage );
        free(lpDMABuffer);
        return 0;
    }
    //map the virtual address to point the physical address
    if ( KernelMap( (void *)lpDMABuffer->dwVirtualAddress, dwSize, (void *)lpDMABuffer->dwPhysicalAddress) )
        return lpDMABuffer;        //success
    else
    {
        //clean-up and return error
        VASM_FreeLowPhysicalPages( lpDMABuffer->dwPhysicalAddress, dwReqPage );
        VirtualFree( GetTaskID(), lpDMABuffer->dwVirtualAddress, dwSize);
        free(lpDMABuffer);
        return 0;
    }
}
//destroy the DMA buffer - should be called after finishing DMA transfer
UINT32 DMA_FreeBuffer(LPDMABUFFER lpDMABuffer)
{
    if ( lpDMABuffer )
    {
        //clean-up
        VASM_FreeLowPhysicalPages( lpDMABuffer->dwPhysicalAddress, lpDMABuffer->dwSize/VASM_PAGE_SIZE + (lpDMABuffer->dwSize%VASM_PAGE_SIZE)?1:0 );
        VirtualFree( GetTaskID(), lpDMABuffer->dwVirtualAddress,  lpDMABuffer->dwSize/VASM_PAGE_SIZE + (lpDMABuffer->dwSize%VASM_PAGE_SIZE)?1:0 );
        free(lpDMABuffer);
        return ERROR_SUCCESS;
    }
    else
        return ERROR_INVALID_HANDLE;
}
UINT32 DMA_SetBufferInfo(LPDMACHANNEL lpDMAChannel, LPDMABUFFER lpDMABuffer)
{
    if ( lpDMAChannel )
    {
        //set the DMA buffer
        lpDMAChannel->lpDMABuffer = lpDMABuffer;
        return ERROR_SUCCESS;
    }
    else
        return ERROR_INVALID_HANDLE;
}

UINT32 DMA_SetChannelMode(LPDMACHANNEL lpDMAChannel, BYTE bMode)
{
    if ( lpDMAChannel )
    {
        lpDMAChannel->bMode = bMode;
        return ERROR_SUCCESS;
    }
    else
        return ERROR_INVALID_HANDLE;
}
/*It will enables the channel before that it sets other registers required for DMA transfer*/
UINT32 DMA_StartTransfer(LPDMACHANNEL lpDMAChannel)
{
    if ( lpDMAChannel )
    {
        if ( lpDMAChannel->bChannelNo < TOTAL_DMA_CHANNELS )
        {
            
            //disable interrupts
            asm volatile("cli");
            
            //disable the channel because we are going to program it
            DMA_DisableChannel(lpDMAChannel);
            
            //set mode
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_WRITE_MODE_REGISTER_INDEX], lpDMAChannel->bMode );
            
            //Send the offset address.  The first byte is the low base offset, the second byte is the high offset.
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_ADDRESS_PORT_INDEX], LOW_BYTE( LP_OFF( lpDMAChannel->lpDMABuffer->dwPhysicalAddress ) ) );
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_ADDRESS_PORT_INDEX], HI_BYTE( LP_OFF(lpDMAChannel->lpDMABuffer->dwPhysicalAddress ) ) );

            //Send the physical page that the data lies on.
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_PAGE_PORT_INDEX], LP_PAGE( lpDMAChannel->lpDMABuffer->dwPhysicalAddress ) );

            //Send the length of the data.  Again, low byte first
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_COUNT_PORT_INDEX], LOW_BYTE( lpDMAChannel->lpDMABuffer->dwSize ));
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_COUNT_PORT_INDEX], HI_BYTE( lpDMAChannel->lpDMABuffer->dwSize ));
                    
            //enable the channel - bits 0:1 - Channel No and bit 2 - (0-Clear 0-Mask)
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_SINGLE_MASK_REGISTER_INDEX], ( lpDMAChannel->bChannelNo % 4 ) );
            
            //enable interrupts
            asm volatile("sti");
            
            return ERROR_SUCCESS;
        }
        else
            return ERROR_INVALID_HANDLE;
    }
    else
        return ERROR_INVALID_HANDLE;
}
/*It will enables the channel and starts any transaction pending*/
UINT32 DMA_EnableChannel(LPDMACHANNEL lpDMAChannel)
{
    if ( lpDMAChannel )
    {
        if ( lpDMAChannel->bChannelNo < TOTAL_DMA_CHANNELS )
        {
            //clear the mask - bits 0:1 - Channel No and bit 2 - (0-Clear 0-Mask)
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_SINGLE_MASK_REGISTER_INDEX], ( lpDMAChannel->bChannelNo % 4 ) );
            return ERROR_SUCCESS;
        }
        else
            return ERROR_INVALID_HANDLE;
    }
    else
        return ERROR_INVALID_HANDLE;
}
/*It will disables the channel and clears any pending transactions*/
UINT32 DMA_DisableChannel(LPDMACHANNEL lpDMAChannel)
{
    if ( lpDMAChannel )
    {
        if ( lpDMAChannel->bChannelNo < TOTAL_DMA_CHANNELS )
        {
            //mask the channel - bits 0:1 - Channel No and bit 2 - (0-Clear 0-Mask)
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_SINGLE_MASK_REGISTER_INDEX], 0x04 | ( lpDMAChannel->bChannelNo % 4 ) );
            //clear any data transfer
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_FLIP_FLOP_REGISTER_INDEX], 0 );
            return ERROR_SUCCESS;
        }
        else
            return ERROR_INVALID_HANDLE;
    }
    else
        return ERROR_INVALID_HANDLE;
}
/*The difference between this and the previous function is that this function will not cancel any data transfer*/
UINT32 DMA_PauseTransfer(LPDMACHANNEL lpDMAChannel)
{
    if ( lpDMAChannel )
    {
        if ( lpDMAChannel->bChannelNo < TOTAL_DMA_CHANNELS )
        {
            //mask the channel - bits 0:1 - Channel No and bit 2 - (0-Clear 0-Mask)
            OutPortByte( sysDMAChannelPort[lpDMAChannel->bChannelNo][DMA_SINGLE_MASK_REGISTER_INDEX], 0x04 | ( lpDMAChannel->bChannelNo % 4 ) );
            return ERROR_SUCCESS;
        }
        else
            return ERROR_INVALID_HANDLE;
    }
    else
        return ERROR_INVALID_HANDLE;
}

