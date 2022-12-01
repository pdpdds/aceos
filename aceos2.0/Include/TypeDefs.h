/*
		ACE OS - Source Code

This file contains some type defintions which may make our code
more readable some time debuggable
*/
#ifndef __TYPE_DEFS__H

#define __TYPE_DEFS__H

extern void DEBUG();

#include "printf.h"

#define ACE_MAJOR 2
#define ACE_MINOR 1
#define ACE_REVISION 1

#define CODE_SELECTOR 0x8
#define DATA_SELECTOR 0x10

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#ifdef KERNELBUILT 
    #define KERNELDLL DLLEXPORT
#else
    #define KERNELDLL DLLIMPORT
#endif

#define CHAR_BIT            8
	
#define CHAR_MAX            127
#define CHAR_MIN            (-128)

#define BYTE_MAX           255
#define BYTE_MIN            0
	
#define INT_MAX             0x7FFF
#define INT_MIN             ((int)0x8000)
#define UINT_MAX            0xFFFFU

#define LONG_MAX            0x7FFFFFFFL
#define LONG_MIN            ((long)0x80000000L)
#define ULONG_MAX           0xFFFFFFFFUL


/* 8 Bit data types*/
typedef unsigned char BYTE;

/* 16 bit data types */
typedef short INT16;
typedef unsigned short UINT16;
typedef unsigned short WORD;

#ifndef _SIZE_T_DEFINED
  typedef unsigned int size_t;
  #define _SIZE_T_DEFINED
#endif

#define CONST const 

#ifndef VOID
    #define VOID void
#endif
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef void *PVOID;
typedef PVOID HANDLE;

typedef long               ISIZE;
typedef unsigned long      LSIZE;
typedef unsigned long      USIZE;


/* 32 bit data types*/
typedef long INT32;
typedef unsigned long UINT32;
typedef unsigned long DWORD;

typedef long long INT64;
typedef unsigned long long UINT64;
typedef BYTE BOOLEAN;

typedef CHAR *PCHAR;
typedef CHAR *LPCH, *PCH;

typedef CONST CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;

typedef LPSTR LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR;
typedef LPCSTR LPCTSTR;

#ifndef NULL
    #define NULL 0
#endif

#define __FLAT__ 1
#define FAR far
#define NEAR near
#define PASCAL pascal
#define CDECL cdecl

#define SUCCESS 1
#define FAILURE -1

#define FALSE 0
#define TRUE  1


#endif
