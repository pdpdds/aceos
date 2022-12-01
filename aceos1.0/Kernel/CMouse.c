/*		Generic Mouse Driver for Ace OS
			- created Sam (samuelhard@yahoo.com) on 06-Sep-2002
*/

#include <TypeDefs.h> 
#include <String.h>
#include <Heap.h>
#include <IO.h>
#include <Mouse.h>
#include <Processor.h>


char MouseType[3][25]=
		{"Microsoft Compatible",
		 "Logitech",
		 "Mouse Systems"
		};
char PortType[3][20]=
		{"Serial",
		"PS/2",
		"USB"
		};
char MouseProtocol=-1;
BYTE PortNo=0, SysMousePortType=-1, HardwareID;
//---------------------------------------------------------------------
/* These constants are SCC IO Port Addresses or SCC Registers */
const IER=1;   	/* Interrupt Enable Register */
const IIR=2;    /* Interrupt Identification Register (Read - Only)*/
const LCR=3; 	/* Line Control Register */
const MCR=4; 	/* Modem Control Register */
const LSR=5;	/* Line Status Register (Read - Only)*/
const MSR=6;	/* Modem Status Register (Read - Only) */
const SRR=7;    /* Shadow Status Register (Read - Only */

/* These constants are PIC 1 Register*/
const PIC_CMD=0x20; 	// PIC Command Register
const PIC_STS=0x21;	// PIC Status Register

const COMInt[4]={0x0C,0x0B,0x0C,0x0B}; /* How much serial ports you have is not a
	problem, only two interrupts are provided for serail communication and
	these address cannot be modified*/

const COMIOBase[4]={0x3F8,0x2F8,0x3E8,0x2E8};/* Similar to the Interrupts, IO addresses
	are hardwired and cannot be modified. but this will cause some incompatability
	with windows and bios because they may remap the IO address to some other space
	*/

UINT16 SysMouseIOPort; /* This global variable is initialized in main() with the
valid 0x3f8 or any other value*/

/* Programming the SCC - Serial Communication Controller
This will make the SCC to accept/transmit 1200 baud and 1 stop bit*/
void InitializeSCC(UINT16 IOBase)
	{
	OutPortByte( IOBase+LCR,0x80);   /* set the Baud Rate Divisor Latch so that we can access baud rate divisor */
	OutPortByte( IOBase,0x60);       /* set the BRD to 1200 baud   */
	OutPortByte( IOBase+1,0x00);     /* same */
	OutPortByte( IOBase+LCR,0x02);   /* 7 bits, no parity, one stop  0000010*/
	}
/* Before calling this function you must set the IVT Correctly*/
void InitializePIC(BYTE COMPort)
	{OutPortByte( SysMouseIOPort+MCR, 0x0b );   /* enable DTR and OUT2 */
	OutPortByte( SysMouseIOPort+IER,0x1);     /* Activating data available interrupt */
	BYTE Mask = InPortByte( PIC_STS ); /* Reading status of PIC */
	Mask = Mask & ( COMPort==1?0xEF:0xF7);/* Setting the appropriate bit for COM1(11101111) and COM2(11110111) */
	OutPortByte( PIC_STS, Mask );
	}
/* This global function pointer will hold the old ISR - Interrupt Service Routine */

void DropDTR(UINT16 IOBase)
	{int Status = InPortByte ( IOBase + MCR );
	Status &= 0xFE;  /* turn off DTR bit */
	OutPortByte ( IOBase+MCR, (unsigned char) Status );
	}
void RaiseDTR(UINT16 IOBase)
	{int Status = InPortByte ( IOBase+MCR );
	Status |= 0x01;                  /* turn on DTR bit */
	OutPortByte ( IOBase+MCR, (unsigned char) Status );
	}
void MouseHand(UINT16 SubSystem, UINT32 Para1, UINT32 Para2)
	{
	}
char * GetMouseType()
	{if ( MouseProtocol == -1 )
		return 0;
	return MouseType[MouseProtocol];
	}
BYTE GetMousePortNo()
	{if ( MouseProtocol == -1 )
		return 0;
	return PortNo;
	}
char * GetMousePortType()
	{if ( MouseProtocol == -1 )
		return 0;
	return PortType[SysMousePortType];
	}
BYTE InitMouse()
	{BYTE MCRData, MData[3];

	for(PortNo=0;PortNo<4 && MouseProtocol == -1;PortNo++)
		{//----***checking whether the COM port is valid
		BYTE Value=InPortByte(COMIOBase[PortNo]+MCR)>>5;
		if ( Value ) //the last 3 bits in MCR reserved and must be always 0
			continue;
		Value=InPortByte(COMIOBase[PortNo]+LSR);
		if ( Value==0xFF )
			continue;
		OutPortByte(COMIOBase[PortNo]+LCR,0x1b);
		Value = InPortByte( COMIOBase[PortNo]+LCR);
		OutPortByte( COMIOBase[PortNo]+LCR,0x3);
		BYTE Value2 = InPortByte( COMIOBase[PortNo]+LCR);
		if ( Value != 0x1b && Value2 != 0x3 )
			continue;
		Value = InPortByte(COMIOBase[PortNo]+IER)>>4;
		if (  Value != 0 )
			continue;
		//----***COM Port checking is over
		InitializeSCC( COMIOBase[PortNo] );

		InPortByte(COMIOBase[PortNo]+LSR);//clear state bits
		InPortByte(COMIOBase[PortNo]+MCR);//clear state bits
		InPortByte(COMIOBase[PortNo]);//clear receive buffer

		OutPortWord(COMIOBase[PortNo]+MSR,0); //interrupts off, disable FIFO

		OutPortByte(COMIOBase[PortNo]+MCR, 0);
		MDelay(1000);		
		OutPortByte(COMIOBase[PortNo]+MCR, 3);

		BYTE DataAvl=0;
		UINT32 T=GetTimerTick();
		do 	{
			Value=InPortByte(COMIOBase[PortNo]+LSR);//checking whether data is available
			DataAvl = Value & 1 ;
			
			}while(T+3000>GetTimerTick() && !DataAvl);
		if ( !DataAvl )
			continue;
		
		MData[0]=InPortByte(COMIOBase[PortNo]);
		MData[1]=InPortByte(COMIOBase[PortNo]);

		if ( MData[0]=='M' )
			{MouseProtocol=0;
			if ( MData[1]=='3' )
				MouseProtocol=1;
			}
		}
	if ( MouseProtocol == -1 )
		return 0;
	SysMouseIOPort=COMIOBase[PortNo];
	SysMousePortType=0; //serial port

	HardwareID = RegisterDeviceDriver(0, 0, 1, "Generic Mouse Driver V0.9", "Power Team", MouseHand);
	if ( HardwareID )
		{SetIDT(32+COMInt[PortNo],MouseHandler);
		InitializeSCC(SysMouseIOPort);

		InitializePIC(PortNo+1);
		InPortByte(SysMouseIOPort);
		}
	return HardwareID;
	}
