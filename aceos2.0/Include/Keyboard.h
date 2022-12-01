/*------------------------KEYBOARD Functions-----------------------------
				Created by Samuel on 01-Sep-2002
				samuelhard@yahoo.com
*/
#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <TypeDefs.h>

#ifdef __cplusplus
	extern "C" {
#endif

#define KB_ONBOARDCONTROLLER	0x64
#define KB_KEYBOARDCONTROLLER 	0x60

#define B_RIGHTSHIFT_DOWN 0x36
#define B_RIGHTSHIFT_UP   0xB6
#define	B_LEFTSHIFT_DOWN  0x2A
#define	B_LEFTSHIFT_UP 	  0xAA
#define	B_RIGHTCTRL_DOWN  0x1D	//*
#define	B_RIGHTCTRL_UP    0x9D	//*
#define	B_LEFTCTRL_DOWN	  0x1D	
#define	B_LEFTCTRL_UP	  0x9D	
#define	B_RIGHTALT_DOWN	  0x38 //*
#define	B_RIGHTALT_UP	  0xb8 //*
#define	B_LEFTALT_DOWN	  0x38
#define	B_LEFTALT_UP	  0xB8
#define	B_SCROLL_DOWN	  0x46
#define	B_SCROLL_UP 	  0xC6		
#define	B_NUM_DOWN	  0x45
#define	B_NUM_UP	  0xC5
#define	B_CAPS_DOWN	  0x3A	
#define	B_CAPS_UP	  0xBA	
#define	B_INS_DOWN	  0x52 //*
#define	B_INS_UP	  0xD2 //*
#define	B_SYSREQ	  0x81
#define B_HOLD_DOWN	  0x45 //*
#define B_HOLD_UP	  0xC5 //*

BYTE InitKeyboard();
void KeyboardHandler();

BYTE KB_GetLastKey();
BYTE KB_KeysAvailable();
BYTE KB_Read();
void KB_Unread(BYTE KeyCode);
BYTE KB_PressStatus();
void KB_SetRate(UINT16 Rate,BYTE Delay);
void KB_GetRate(UINT16 * Rate, BYTE * Delay);

BYTE KB_ConvertScanCode(BYTE ScanCode, BYTE Modifier);

#ifdef __cplusplus
	}
#endif

#endif


