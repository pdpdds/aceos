/*                            Printer Port Functions  - Ace OS
                                                        - Sam (24-Jun-2003) samueldotj@gmail.com
*/

#include <TypeDefs.h>
#include <IO.h>

#define CONTROL_REG 2
#define STATUS_REG  1

#define MAX_PRINTER_PORTS 4
#define PRINTER_TIME_OUT  10

#define PRINTER_PORT_NOT_EXISTS 1
#define PRINTER_PORT_NO_ERROR 0

int sysPrinterPorts[MAX_PRINTER_PORTS]={0x378,0x278,0,0};

int InitPrinter(int PrinterPortNo)
{
    if ( PrinterPortNo<0 || PrinterPortNo>MAX_PRINTER_PORTS )
        return PRINTER_PORT_NOT_EXISTS;
    
    int PrinterPort=sysPrinterPorts[PrinterPortNo],i;
    BYTE CtrlReg=InPortByte(PrinterPort+CONTROL_REG);
    CtrlReg &= 0xDB; //Clear INIT/BIDIR bits.
    OutPortByte(PrinterPort,CtrlReg );
    for(i=0;i<5;i++); //This will produce at least a 50 usec delay.
    CtrlReg |= 0x4; //Stop resetting printer.
    OutPortByte(PrinterPort,CtrlReg );
    return 0;
}
int Printer_Print(int PrinterPortNo, BYTE Character)
{
    BOOLEAN Busy=TRUE;
    if ( PrinterPortNo<0 || PrinterPortNo>MAX_PRINTER_PORTS )
        return PRINTER_PORT_NOT_EXISTS;

    int PrinterPort=sysPrinterPorts[PrinterPortNo],i,j;
    
    for(i=0;i<PRINTER_TIME_OUT ;i++)
        for(j=0;j<100;j++)
        {
            BYTE StatusReg=InPortByte(PrinterPort+STATUS_REG);
            if ( StatusReg & 0x80 )
            {
                Busy=FALSE;
                break;
            }
        }
    OutPortByte(PrinterPort, Character); 
        
    BYTE CtrlReg=InPortByte(PrinterPort+CONTROL_REG);
    CtrlReg &= 0x1E;
    OutPortByte(PrinterPort+CONTROL_REG,CtrlReg);
        
    for(i=0;i<16;i++);
    
    CtrlReg |= 1;
    OutPortByte(PrinterPort+CONTROL_REG,CtrlReg); //strobe
    for(i=0;i<16;i++);
        
    CtrlReg &= 0xFE;
    OutPortByte(PrinterPort+CONTROL_REG,CtrlReg); //clear strobe
        
    return PRINTER_PORT_NO_ERROR;
}

