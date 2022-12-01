/*---------------------Ace OS source Code-----------------------
		Created by Sam on 10-Aug-2002(samuelhard@yahoo.com)
 This file is a header file for VGAText, VGACursor rountines
--------------------------------------------------------------*/
#ifndef VGATEXT_H
#define VGATEXT_H
#include <TypeDefs.h>

#define VGA_MAX_COL 80
#define VGA_MAX_ROW 25


#define VGA_CRT_ADDRESS 0x3D4
#define VGA_CRT_DATA    0x3D5

#define VGA_CRT_CURSOR_START    0x0A
#define VGA_CRT_CURSOR_END      0x0B
#define VGA_CRT_H_START_ADDRESS 0x0C
#define VGA_CRT_H_END_ADDRESS   0x0D
#define VGA_CRT_CURSOR_H_LOCATION 0x0E
#define VGA_CRT_CURSOR_L_LOCATION 0x0F


#define VGA_TEXT_MEMORY (0xB8000)

#define MAX_COL 80
#define MAX_ROW 25


#ifdef __cplusplus
    extern "C" {
#endif

            
void VGA_SetTextAttribute(BYTE Attribute);
BYTE VGA_GetTextAttribute();
            
BYTE VGA_GetCurCol();
BYTE VGA_GetCurRow();
            
void VGA_PrintCharacter(BYTE Row,BYTE Col, BYTE Character);
void VGA_PutCh(BYTE Character);
//KERNELDLL int  VGA_PrintString(BYTE Row,BYTE Col, char * String);
int  VGA_Print(char * String);
void VGA_Fill(BYTE RowStart, BYTE ColStart,BYTE RowEnd , BYTE ColEnd, BYTE Ch_Att, BYTE Type);
void VGA_InsertRow();
void VGA_SetCursorType(BYTE Start, BYTE End);
void VGA_MoveCursor(BYTE Row,BYTE Col);
void VGA_SetCursorType(BYTE Bottom, BYTE Top);

#ifdef __cplusplus
	}
#endif
#endif
