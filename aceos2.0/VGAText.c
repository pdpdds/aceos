/*              VGA Text Mode 80x25 Routines
    Ace OS Source Code 
        Created on 10-Aug-2002 by Samuel(samueldotj@gmail.com) as assembly file
        Modifed as C file on 29-July-2003
*/
#include <TypeDefs.h>
#include <IO.h>
#include <VGAText.h>


BYTE VGACurRow=0;
BYTE VGACurCol=0;
BYTE VGATextAttribute=0x07;

BYTE VGACursorBottom=14;
BYTE VGACursorTop=13;

void VGA_SetTextAttribute(BYTE Attribute)
{
    VGATextAttribute = Attribute;
}
BYTE VGA_GetTextAttribute()
{
    return VGATextAttribute;
}
BYTE VGA_GetCurCol()
{
    return VGACurCol;
}
BYTE VGA_GetCurRow()
{
    return VGACurRow;
}
void VGA_PrintCharacter(BYTE Row, BYTE Col, BYTE Character)
{
    
    int VGAOffset=(Row*160)+(Col*2);
    BYTE * VGAMemory=(BYTE *)VGA_TEXT_MEMORY;
    if ( Row >= MAX_ROW || Col >= MAX_COL )
        return;
    VGACurCol=Col; VGACurRow=Row;
    
    VGAMemory[VGAOffset]=Character;
    VGAMemory[VGAOffset+1]=VGATextAttribute;
    
}
void VGA_PutCh(BYTE Character)
{
    unsigned char attribute = (0 << 4) | (15 & 0xF);

    //if you make this VGAOffest at global level it will increase the performance
    if ( Character == 0 ) Character = '*';
    int VGAOffset=(VGACurRow*160)+(VGACurCol*2);
    BYTE * VGAMemory=(BYTE *)VGA_TEXT_MEMORY;
    switch ( Character )
    {
        case  '\n':
            VGACurRow++;
            break;
        case '\r':
            VGACurCol=0;
            break;
        case '\t':
            VGACurCol+=8;
            break;
        default:
            VGACurCol++;
            VGAMemory[VGAOffset+1]=attribute;
            VGAMemory[VGAOffset]=Character;
    }
    
    if ( VGACurCol >= MAX_COL )
    {
        VGACurCol=0;
        VGACurRow++;
    }
    if ( VGACurRow >= MAX_ROW )
    {
        VGACurCol=0;
        VGACurRow=MAX_ROW-1;
        VGA_InsertRow();
    }
    VGA_MoveCursor(VGACurCol, VGACurRow );
}

int VGA_Print(char * String)
{
    int i=0;
    while(String[i])
        VGA_PutCh(String[i++]);
    return i;
}
void VGA_Fill(BYTE RowStart, BYTE ColStart, BYTE RowEnd , BYTE ColEnd, BYTE Ch_Att, BYTE Type)
{
    int VGAOffset;
    BYTE * VGAMemory=(BYTE *)VGA_TEXT_MEMORY;
    BYTE i,j;
    
    for(i=RowStart;i<=RowEnd;i++)
    {
        VGAOffset=(i*160)+(ColStart*2);
        for(j=ColStart;j<=ColEnd;j++)
        {
            if ( Type=='C' )
                VGAMemory[VGAOffset]=Ch_Att; //character
            else
                VGAMemory[VGAOffset+1]=Ch_Att;//attribute
            VGAOffset+=2;
        }
        
    }
}
void VGA_InsertRow()
{
    int VGAOffset=160;
    BYTE * VGAMemory=(BYTE *)VGA_TEXT_MEMORY;
    
    while(VGAOffset<4000)
    {
        VGAMemory[VGAOffset-160]=VGAMemory[VGAOffset];
        VGAOffset++;
    }
    VGAOffset=3840;
    while(VGAOffset<4000)
    {
        VGAMemory[VGAOffset]=0;
        VGAMemory[VGAOffset+1]=VGATextAttribute;
        VGAOffset+=2;
    }
    VGACurRow=MAX_ROW-1;
    VGACurCol=0;
}
void VGA_MoveCursor(BYTE X, BYTE Y)
{
    UINT16 Offset=(Y*MAX_COL)+(X);
    if ( X >= MAX_COL || Y >= MAX_ROW )
        return;
    VGACurCol=X;
    VGACurRow=Y;
    
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
void VGA_SetCursorType(BYTE Bottom, BYTE Top)
{
    OutPortByte(VGA_CRT_ADDRESS,VGA_CRT_CURSOR_START);
    OutPortByte(VGA_CRT_DATA,Top);
    OutPortByte(VGA_CRT_ADDRESS,VGA_CRT_CURSOR_END);
    OutPortByte(VGA_CRT_DATA,Bottom);
}

