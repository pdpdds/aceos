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


DLLEXPORT BYTE  InPortByte ( WORD Port);
DLLEXPORT WORD  InPortWord ( WORD Port);
DLLEXPORT DWORD InPortDWord( WORD Port);

DLLEXPORT void OutPortByte  ( WORD Port, BYTE Value);
DLLEXPORT void OutPortWord  ( WORD Port, WORD Value);
DLLEXPORT void OutPortDWord ( WORD Port, DWORD Value);

#ifdef __cplusplus
	}
#endif

#endif


