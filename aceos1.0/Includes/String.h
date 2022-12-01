#ifndef __STRING__H
#define __STRING__H
#include <TypeDefs.h>
/*      1) This file is for only small string handling (32000 characters)
	   with functionality
	2) Use String class instead of this one only if you are programming
	   at high level. If you are programming at low kernel level use
	   this file


	Created By Sam
		samuelhard@yahoo.com
*/
#ifdef __cplusplus
	extern "C" {
#endif
#include <TypeDefs.h>
#include <ChType.h>
/*	2) Use String class instead of this one only if you are programming
	   at high level. If you are programming at low kernel level use
	   this file


	Created By Sam
		samuelhard@yahoo.com
*/
void MoveData(void * Dest, void * Src, UINT32 Len);
void MemCpy(void * Dest, void * Src, UINT32 Len);
void * MemChr(void * Src,BYTE Character, UINT32 Len);
signed char MemCmp(void * S1, void * S2,UINT32 Len);
signed char MemICmp(void * S1, void * S2,UINT32 Len);
void MemSet(char * String, const char Ch, UINT32 Len);



UINT32 StrLen(const char * String);
char *StrCpy(char * Dest, const char * Source);
char *StrNCpy(char * Dest,const char * Source,UINT16 Length);
int StrCmp(const char * String1,const char * String2);
int StrICmp(const char * String1,const char * String2);
int StrNCmp(const char * String1,const char * String2,UINT32 Len);
int StrNICmp(const char * String1,const char * String2,UINT32 Len);
char * StrCat(char * Dest,const char * Source);
const char *StrStr(const char * String1,char * const String2);
const char *StrChr(const char * String,char const Character);
const char *StrRChr(const char * String,char const Character);
const char *StrNChr(const char * String,char const Character, UINT32 Length);
char * RPad(char * Dest, const char * Source , UINT32 Len, char FillChar);
char * LPad(char * Dest, const char * Source , UINT32 Len, char FillChar);
char *StrRev(char *str);
char *StrUpr(char *String);
char *StrLwr(char *String);
UINT32 StrCSpn(const char *s1, const char *s2);
#ifdef __cplusplus
	}
#endif

#endif
