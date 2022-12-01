/*------------------------Mouse Functions-----------------------------
				Created by Samuel on 06-Sep-2002
				samuelhard@yahoo.com
*/
#ifndef __MOUSE_H
#define __MOUSE_H

#include <TypeDefs.h>

#ifdef __cplusplus
	extern "C" {
#endif
void MouseHandler();
BYTE InitMouse();

void RaiseDTR(UINT16 IOBase);
void DropDTR(UINT16 IOBase);
void InitializePIC(BYTE COMPort);
void InitializeSCC(UINT16 IOBase);
char * GetMouseType();
BYTE GetMousePortNo();
char * GetMousePortType();

#ifdef __cplusplus
	}
#endif

#endif


