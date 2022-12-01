/*
		ACE OS - Source Code
        Printer Port (parallel port)  24-Jun-2003
        
        Created for debug purpose, not yet completed.
        Followed from the printer assembly code of the book "Art of Assembly"  
                    - Sam (samuelhard@yahoo.com)
*/
#ifndef __PRINTER_PORT__H

#define __PRINTER_PORT__H

int InitPrinter(int PrinterPortNo);
int Printer_Print(int PrinterPortNo, BYTE Character);

#endif

