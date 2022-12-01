#ifndef __V86_H
#define __V86_H
/* 	   
    V86 Monitor for Ace OS
		Created by Sam ( 06-Jul-2004)
				samuelhard@yahoo.com
*/

#include <TypeDefs.h>
#include <Memory.h>
#include <malloc.h>
#include <stdio.h>
#include <TaskMan.h>
#include <Thread.h>

#define V86_EFLAG_VALUE 0x20202 //0x23202
#define V86_EFLAG_IF	0x00200
#define V86_CODE_SELECTOR    0x1000
#define V86_DATA_SELECTOR    0x0


#ifdef __cplusplus
    extern "C" {
#endif
        
struct V86TaskInfo
{
    UINT32 EAX;
    UINT32 ErrorCode;
    UINT32 EIP;
    UINT32 CS;
    UINT32 EFlags;
    UINT32 ESP;
    UINT32 SS;
    UINT32 ES;
    UINT32 DS;
    UINT32 FS;
    UINT32 GS;
    
}__attribute__ ((packed));

typedef struct V86TaskInfo V86TASKINFO;
typedef V86TASKINFO * LPV86TASKINFO;

#ifdef __cplusplus
    }
#endif
#endif
