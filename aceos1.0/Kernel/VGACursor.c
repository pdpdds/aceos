/*------------------------------------------------------
               Ace Os Kernel source Code
                         Created by Sam 
	   Cursor position and size management
------------------------------------------------------ */

#include "TypeDefs.h"
#include "IO.h"
#define VGA_CRT_ADDRESS 0x3D4
#define VGA_CRT_DATA    0x3D5

#define VGA_CRT_CURSOR_START 0x0A
#define VGA_CRT_CURSOR_END 0x0B
#define VGA_CRT_H_START_ADDRESS 0x0C
#define VGA_CRT_H_END_ADDRESS 0x0D
#define VGA_CRT_CURSOR_H_LOCATION 0x0E
#define VGA_CRT_CURSOR_L_LOCATION 0x0F
void MoveCursor(BYTE X,BYTE Y)
	{UINT16 Offset=(X*80)+(Y-1);
	OutPortByte(VGA_CRT_ADDRESS,VGA_CRT_CURSOR_H_LOCATION);
	OutPortByte(VGA_CRT_DATA,Offset>>8);
	OutPortByte(VGA_CRT_ADDRESS,VGA_CRT_CURSOR_L_LOCATION);
	OutPortByte(VGA_CRT_DATA,(Offset<<8)>>8);
	}
/* Sets the Cursor Type
	0 to 15 is possible value to pass
	Returns - none.
	Example : Normal Cursor - (13,14)
		  Solid Cursor - (0,15)
		  No Cursor - (25,25) - beyond the cursor limit so it is invisible
*/
void SetCursorType(BYTE Bottom, BYTE Top)
	{OutPortByte(VGA_CRT_ADDRESS,VGA_CRT_CURSOR_START);
	OutPortByte(VGA_CRT_DATA,Top);
	OutPortByte(VGA_CRT_ADDRESS,VGA_CRT_CURSOR_END);
	OutPortByte(VGA_CRT_DATA,Bottom);
	}

