/*
		ACE OS - Source Code

This file contains some type defintions which may make our code
more readable some time debuggable
*/
#ifndef __TYPE_DEFS__H

#define __TYPE_DEFS__H

#define ACE_MAJOR 1
#define ACE_MINOR 1
#define ACE_REVISION 0

#define KERNEL_LOAD_ADDRESS 0x19000

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
typedef char CHAR;

/* 16 bit data types */
typedef int INT16;
typedef unsigned short UINT16;
typedef unsigned short WORD;


/* 32 bit data types*/
typedef long INT32;
typedef unsigned long UINT32;
typedef unsigned long DWORD;

typedef BYTE BOOLEAN;

#define NULL 0

#define __FLAT__ 1
#define FAR far
#define NEAR near
#define PASCAL pascal
#define CDECL cdecl

#define SUCCESS 1
#define FAILURE -1

#define False 0
#define True  1
#define FALSE 0
#define TRUE  1


#endif
