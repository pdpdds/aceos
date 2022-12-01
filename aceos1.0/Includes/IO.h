/*------------------------Intel IO Functions-----------------------------
				Created by Samuel on 14-June-2002
				samuelhard@yahoo.com
*/
#ifndef __IO_H
#define __IO_H

#include <TypeDefs.h>

#ifdef __cplusplus
	extern "C" {
#endif


BYTE  InPortByte ( WORD Port);
WORD  InPortWord ( WORD Port);
DWORD InPortDWord( WORD Port);

void OutPortByte  ( WORD Port, BYTE Value);
void OutPortWord  ( WORD Port, WORD Value);
void OutPortDWord ( WORD Port, DWORD Value);

#ifdef __cplusplus
	}
#endif

#endif


