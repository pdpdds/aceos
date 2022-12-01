/*
  Name: 		Floppy Driver for Ace OS
  Author: 			- Sam (samueldotj@yahoo.co.in)
  Date: 		14-Sep-2004 9:58PM  
*/
#include <Floppy.h>
#include <Processor.h>
#include <GSH.h>
#include <IO.h>
#include <malloc.h>
#include <SysInfo.h>
#include <Timer.h>
#include <printf.h> /* debug*/


#define FDC_MAX_STATUS_BYTES    7

/* drive geometries */
#define FDC_DG144_HEADS       2     /* heads per drive (1.44M) */
#define FDC_DG144_TRACKS     80     /* number of tracks (1.44M) */
#define FDC_DG144_SPT        18     /* sectors per track (1.44M) */
#define FDC_DG144_GAP3FMT  0x54     /* gap3 while formatting (1.44M) */
#define FDC_DG144_GAP3RW   0x1b     /* gap3 while reading/writing (1.44M) */

#define FDC_DG168_HEADS       2     /* heads per drive (1.68M) */
#define FDC_DG168_TRACKS     80     /* number of tracks (1.68M) */
#define FDC_DG168_SPT        21     /* sectors per track (1.68M) */
#define FDC_DG168_GAP3FMT  0x0c     /* gap3 while formatting (1.68M) */
#define FDC_DG168_GAP3RW   0x1c     /* gap3 while reading/writing (1.68M) */

/* command bytes (these are 765 commands + options such as MFM, etc) */
#define FDC_CMD_SPECIFY (0x03)  /* specify drive timings */
#define FDC_CMD_WRITE   (0xc5)  /* write data (+ MT,MFM) */
#define FDC_CMD_READ    (0xe6)  /* read data (+ MT,MFM,SK) */
#define FDC_CMD_RECAL   (0x07)  /* recalibrate */
#define FDC_CMD_SENSEI  (0x08)  /* sense interrupt status */
#define FDC_CMD_FORMAT  (0x4d)  /* format track (+ MFM) */
#define FDC_CMD_SEEK    (0x0f)  /* seek track */
#define FDC_CMD_VERSION (0x10)  /* FDC version */

/* IO Port address */
#define FDC_TOTAL_CONTROLLERS   2
const UINT16 FDD_Con_IOBase[FDC_TOTAL_CONTROLLERS]={ 0x3F0, 0x370 };

#define FDC_STATUS_REGISTER_A                   0
#define FDC_STATUS_REGISTER_B                   0
#define FDC_DIGITAL_OUTPUT_REGISTER             2
#define FDC_MAIN_STATUS_REGISTER                4
#define FDC_DATA_RATE_SELECT_REGISTER           4
#define FDC_DATA_REGISTER                       5
#define FDC_DIGITAL_INPUT_REGISTER              7
#define FDC_CONFIGURATION_CONTROL_REGISTER      7


#define MAX_STATUS_BYTES        7
//this array is used to track the status bytes of a controller
BYTE FDC_StatusBytes[FDC_TOTAL_CONTROLLERS][FDC_MAX_STATUS_BYTES];
BYTE FDC_StatusByteCount[FDC_TOTAL_CONTROLLERS];
BYTE FDC_SR0[FDC_TOTAL_CONTROLLERS];


//these variables are belongs to the status of individual drives
BYTE FDD_MotorStatus[FDC_TOTAL_CONTROLLERS][4];            //indicates whether a motor is on or off
BOOLEAN FDD_DiskChangeStatus[FDC_TOTAL_CONTROLLERS][4];         
INT16 FDD_MotorOffCount[FDC_TOTAL_CONTROLLERS][4];    //this variable is used to switch off motor after 30 ms
BYTE FDD_CurrentTrack[FDC_TOTAL_CONTROLLERS][4];
//BOOLEAN FDD_DiskChangeStatus[FDC_TOTAL_CONTROLLERS][4];

//this variable is set to true when a IRQ 6 is raised 
volatile BOOLEAN FDD_InterruptReceived=FALSE;

struct ListNode * sysFDDs;

//-----------------------------Internal functions-----------------------------

//This function enables a controller by switching on the motor 
UINT32 Floppy_EnableController(BYTE bDeviceController, BYTE bDevice)
{
    BYTE bValue=0;
    if ( bDeviceController > FDC_TOTAL_CONTROLLERS || bDevice > 3 )
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return ERROR_INVALID_PARAMETER;
    }
    bValue = bDevice & 0x3; //only the first 2 bits are device settings
    bValue |= 0xC;          //0xC - DMA and Interrupt enable, Enable controller
    bValue |= ( 0x10 << bDevice );      // calculate Motor On bit 
    
    //send command to DOR
    OutPortByte( FDD_Con_IOBase[bDeviceController] + FDC_DIGITAL_OUTPUT_REGISTER, bValue );
    
    MDelay(50000);                                // delay for 500 ms, this will enough time to spin up the motor
    
    FDD_MotorStatus[bDeviceController][bDevice] = 1;     //Motor on
    FDD_MotorOffCount[bDeviceController][bDevice] = -1;
    
    return ERROR_SUCCESS;
}
//This function disables a controller by switching off the motor 
UINT32 Floppy_DisableController(BYTE bDeviceController, BYTE bDevice)
{
    if ( bDeviceController > FDC_TOTAL_CONTROLLERS )
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return ERROR_INVALID_PARAMETER;
    }
    
    FDD_MotorStatus[bDeviceController][bDevice] = 0;     //Motor on
    FDD_MotorOffCount[bDeviceController][bDevice] = 36;
    
    return ERROR_SUCCESS;
}
/* sendbyte() routine from intel manual */
UINT32 Floppy_SendByteToController(BYTE bDeviceController, BYTE bValue)
{
    volatile int iMSRValue;
    int tmo;
    if ( bDeviceController > FDC_TOTAL_CONTROLLERS )
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return ERROR_INVALID_PARAMETER;
    }
    
    for (tmo = 0;tmo < 128;tmo++)
    {
        iMSRValue = FDD_Con_IOBase[bDeviceController] + FDC_MAIN_STATUS_REGISTER;       //read MSR
        if ( (iMSRValue & 0xC0) == 0x80)                                                //check whether the controller is ready
        {
            OutPortByte( FDD_Con_IOBase[bDeviceController] + FDC_DATA_REGISTER , bValue);  //send byte
            return ERROR_SUCCESS;
        }
        InPortByte(0x80);                                                               //delay
    }
    SetLastError( WAIT_TIMEOUT );
    return ERROR_SUCCESS;
}
/* getbyte() routine from intel manual */
INT16 Floppy_GetByteFromController(BYTE bDeviceController)
{
    volatile int iMSRValue;
    int tmo;
    if ( bDeviceController > FDC_TOTAL_CONTROLLERS )
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return -1;
    }
    
    for (tmo = 0;tmo < 128;tmo++)
    {
        iMSRValue = FDD_Con_IOBase[bDeviceController] + FDC_MAIN_STATUS_REGISTER;       //read MSR
        if ( (iMSRValue & 0xD0) == 0xD0)                                                //check whether the controller is ready
            return InPortByte( FDD_Con_IOBase[bDeviceController] + FDC_DATA_REGISTER );

        InPortByte(0x80);                                                               //delay
    }
    SetLastError( WAIT_TIMEOUT );
    return -1;
}
BOOLEAN Floppy_Wait(BYTE bDeviceController, BYTE bDevice, BOOLEAN SenseInterrupt)
{
    UINT32 dwTimeOut;
    if ( bDeviceController > FDC_TOTAL_CONTROLLERS || bDevice > 4 )    
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }
    
    dwTimeOut = GetTimerTick() + 18;      //set timeout to 1 sec
    while( !FDD_InterruptReceived && dwTimeOut >= GetTimerTick() );     //wait untill 1 sec or a interrupt occurs
        
    //read controller status bytes
    for(FDC_StatusByteCount[bDeviceController]=0; FDC_StatusByteCount[bDeviceController]<FDC_MAX_STATUS_BYTES && ( InPortByte( FDC_MAIN_STATUS_REGISTER ) & 0x10 ) ; FDC_StatusByteCount[bDeviceController]++ )
        FDC_StatusBytes[bDeviceController][FDC_StatusByteCount[bDeviceController]] = Floppy_GetByteFromController( bDeviceController ) ;
    
    //if sense interrupt required then send command - SENSI
    if ( SenseInterrupt )
    {
        Floppy_SendByteToController( bDeviceController, FDC_CMD_SENSEI );
        FDC_SR0[bDeviceController] = Floppy_GetByteFromController( bDeviceController );
        FDD_CurrentTrack[bDeviceController][bDevice] = Floppy_GetByteFromController( bDeviceController );
    }
    
    //reset interrupt variable
    FDD_InterruptReceived = FALSE;
    
    if ( dwTimeOut < GetTimerTick() ) //timed out?
    {
        if ( InPortByte( FDD_Con_IOBase[bDeviceController] + FDC_DIGITAL_INPUT_REGISTER ) & 0x80 ) //disk change?
            FDD_DiskChangeStatus[bDeviceController][bDevice]=TRUE;
        return FALSE;
    }
    else
        return TRUE;
    
    
    
}
/* recalibrate the drive */
void Floppy_Recalibrate(BYTE bDeviceController, BYTE bDevice)
{
    /* turn the motor on */
    Floppy_EnableController(bDeviceController, bDevice);
    
    /* send actual command bytes */
    Floppy_SendByteToController( bDevice, FDC_CMD_RECAL );
    Floppy_SendByteToController( bDevice, 0 );
    
    /* wait until seek finished */
    Floppy_Wait( bDeviceController, bDevice, TRUE );

    /* turn the motor off */
    Floppy_DisableController( bDeviceController, bDevice );
}
UINT32 Floppy_Reset(BYTE bDeviceController, BYTE bValue)
{
    SetLastError( ERROR_NOT_SUPPORTED );
    return 0;
}

//------------------------Kernel and System level functions-------------------
void Floppy_Initialize()
{
    char szKey[3];
    int i,j;

    szKey[0]='F'; //FDD type ID
    szKey[1]='0'; //First FDD
    szKey[2]=0;   //Null Character
    
    sysFDDs = NULL;
    for( i=0; i< FDC_TOTAL_CONTROLLERS; i++)
    {
        for(j=0;i<4;j++)
        {
            FDD_MotorStatus[i][j]=0;       //motor off
            FDD_MotorOffCount[i][j]=-1;
            
            Floppy_Reset(i,j);
        }
    }
        
    if ( szKey[1] != '0' ) //one or more FDD is found and we must register into the GSH
    {
        LPGSH lpNewGSH;
        //remember this structure should not be freed
        lpNewGSH = (LPGSH)malloc(sizeof(GSH));
        if ( lpNewGSH == NULL )
        {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            return;
        }
        lpNewGSH->GetNoOfDevices=FDD_GetNoOfDevices;
        lpNewGSH->Read=FDD_Read;
        lpNewGSH->Write=FDD_Write;
        
        lpNewGSH->GetDeviceParameters=NULL;
        lpNewGSH->Verify=NULL;
        lpNewGSH->Reset=NULL;
        lpNewGSH->Status=NULL;
        
        //newStorageHandler->DeviceHandler=this;
        if ( GSH_Register('F',"Floppy    ", lpNewGSH) )
            printf("\n\rFloppy :: GSH Registration failed.");
    }
}

BYTE FDD_GetNoOfDevices()
{
    SetLastError( ERROR_NOT_SUPPORTED );
    return 0;
}
UINT32 FDD_GetDeviceParameters(LPCSTR szDevicePath, BYTE * lpBuffer)
{
    SetLastError( ERROR_NOT_SUPPORTED );
    return 0;
}
UINT32 FDD_Reset(LPCSTR szDevicePath)
{
    SetLastError( ERROR_NOT_SUPPORTED );
    return 0;
}
UINT32 FDD_Status(LPCSTR szDevicePath)
{
    SetLastError( ERROR_NOT_SUPPORTED );
    return 0;
}
UINT32 FDD_Read(BYTE bDeviceNo,  LPGSHIOPARA lpIOPara, BYTE * lpBuffer)
{
    SetLastError( ERROR_NOT_SUPPORTED );
    return 0;
}
UINT32 FDD_Write(BYTE bDeviceNo,  LPGSHIOPARA lpIOPara, BYTE * lpBuffer)
{
    SetLastError( ERROR_NOT_SUPPORTED );
    return 0;
}
UINT32 FDD_Verify(BYTE bDeviceNo,  LPGSHIOPARA lpIOPara)
{
    SetLastError( ERROR_NOT_SUPPORTED );
    return 0;
}

