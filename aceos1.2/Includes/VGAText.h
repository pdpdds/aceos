/*---------------------Ace OS source Code-----------------------
		Created by Sam on 10-Aug-2002(samuelhard@yahoo.com)
 This file is a header file for VGAText, VGACursor rountines
--------------------------------------------------------------*/
#ifndef VGATEXT_H
#define VGATEXT_H
#include <TypeDefs.h>

#ifdef __cplusplus
	extern "C" {
#endif

BYTE SetTextAttribute(BYTE Attribute);
BYTE GetTextAttribute();
void PrintCharacter(int Row,int Col, BYTE Character);
void PutCh(BYTE Character);
int PrintString(int Row,int Col, char * String);
int Print(char * String);
void Fill(int RowStart, int ColStart,int RowEnd , int ColEnd, BYTE Ch_Att, BYTE Type);
void InsertRow(void);
void SetCursorType(int Start, int End);
void MoveCursor(int Row,int Col);

#ifdef __cplusplus
	}
#endif
#endif
