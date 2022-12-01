/*              Keyboard interface Routines
                      Developed by Sam(samuelhard@yahoo.com) on 19-June-2002
                      
*/
#include "TypeDefs.h"
#include "Processor.h"
#include "Keyboard.h"
#include "Heap.h"
#include "DeviceDriver.h"
#include "IO.h"
#include "Interrupt.h"


#define BUFFERSIZE 500
BYTE Sys_KeyboardHardwareID;
UINT32 LastKeyPress=0;
	/*Format of this variable is		DOWN	UP
	0 - Right Shift  			36 	B6
	1 - Left Shift				2A 	AA
	2 - Right Ctrl				E0-1D	E0-9D
	3 - Left Ctrl				1D	9D
	4 - Right Alt				E0-38	E0-B8
	5 - Left Alt				38	B8
	6 - Scroll Lock Active			46	C6		
	7 - Num Lock Active			45	C5
	8 - Caps Lock Active			3A	BA
	9 - Insert Active			E0-52	E0-D2
	10 - Sys Rq / Print Screen		81
	11 - Hold Active ( Ctrl+NumLock or Pause/Break )	E0-45	E0-C5
	*/
UINT32 KeyPressStatus=0;
	
BYTE KeyBufferStart=0;	//indicates where character starts
BYTE KeyBufferTotal=0;	//total characters in buffer


BYTE * KeyBuffer;
BYTE KeyScanCodeSet[][2] =
	{ 	{0  , 0},
	  	{'1' , '!' },
		{'2' , '@' },
		{'3' , '#' },
		{'4' , '$' },
		{'5' , '%' },
		{'6' , '^' },
		{'7' , '&' },
		{'8' , '*' },
		{'9' , '(' },
		{'0' , ')' },
		{'-' , '_' },
		{'=' , '+' },
		{'\b' , '\b' },
		{'\t' , '\t' },
		{'q' , 'Q' },
		{'w' , 'W' },
		{'e' , 'E' },
		{'r' , 'R' },
		{'t' , 'T' },
		{'y' , 'Y' },
		{'u' , 'U' },
		{'i' , 'I' },
		{'o' , 'O' },
		{'p' , 'P' },
		{'[' , '{' },
		{']' , '}' },
		{ 0 , 0 },
		{ 0 , 0 },
		{'a' , 'A' },
		{'s' , 'S' },
		{'d' , 'D' },
		{'f' , 'F' },
		{'g' , 'G' },
		{'h' , 'H' },
		{'j' , 'J' },
		{'k' , 'K' },
		{'l' , 'L' },
		{';' , ':' },
		{'\'' , '\"' },
		{'`' , '~' },
		{ 0 , 0 },
		{ '\\' , '|' },
		{'z' , 'Z' },
		{'x' , 'X' },
		{'c' , 'C' },
		{'v' , 'V' },
		{'b' , 'B' },
		{'n' , 'N' },
		{'m' , 'M' },
		{',' , '<' },
		{'.' , '>' },
		{'/' , '?' },
		{0 , 0 },
		{0 , 0 },
		{0 , 0 },
		{' ' , ' ' },
	};
void KeyboardHand(UINT16 SubSystem,UINT32 Para1, UINT32 Para2)
	{BYTE StoreOffset;
	//Printf(" [%X] ", Para1 );
	switch ( Para1 )
		{
		case B_RIGHTSHIFT_DOWN:
			KeyPressStatus|=B_RIGHTSHIFT_MASK;
			break;
		case B_RIGHTSHIFT_UP:
			KeyPressStatus&= !B_RIGHTSHIFT_MASK;
			break;
		case B_LEFTSHIFT_DOWN:
			KeyPressStatus|=B_LEFTSHIFT_MASK;
			break;
		case B_LEFTSHIFT_UP:
			KeyPressStatus&= !B_LEFTSHIFT_MASK;
			break;
		case B_LEFTCTRL_DOWN:		
			if ( LastKeyPress == 0xE0 )
				KeyPressStatus|=B_RIGHTCTRL_MASK;
			else
				KeyPressStatus|=B_LEFTCTRL_MASK;
			break;
		case B_LEFTCTRL_UP:
			if ( LastKeyPress == 0xE0 )
				KeyPressStatus&=!B_RIGHTCTRL_MASK;
			else
				KeyPressStatus&=!B_LEFTCTRL_MASK;
			break;
		case B_LEFTALT_DOWN:
			if ( LastKeyPress == 0xE0 )
				KeyPressStatus|= B_RIGHTALT_MASK;
			else
				KeyPressStatus|= B_LEFTALT_MASK;
			break;
		case B_LEFTALT_UP:
			if ( LastKeyPress == 0xE0 )
				KeyPressStatus&= !B_RIGHTALT_MASK;
			else
				KeyPressStatus&= !B_LEFTALT_MASK;
			break;
		case B_SCROLL_DOWN:
			KeyPressStatus|=B_SCROLL_MASK;
			break;
		case B_SCROLL_UP:
			KeyPressStatus&= !B_SCROLL_MASK;
			break;
		case B_NUM_DOWN:
			if ( LastKeyPress == 0xE0 )
				KeyPressStatus|=B_HOLD_MASK;
			else
				KeyPressStatus|=B_NUM_MASK;
			break;
		case B_NUM_UP:	
			if ( LastKeyPress == 0xE0 )
				KeyPressStatus&= !B_HOLD_MASK;
			else
				KeyPressStatus&= !B_NUM_MASK;
			break;
		/*case B_CAPS_DOWN:
			KeyPressStatus|=B_CAPS_MASK;
			break;*/
		case B_CAPS_UP:
			if (!( KeyPressStatus & B_CAPS_MASK ) )
				KeyPressStatus|=B_CAPS_MASK;
			else
				KeyPressStatus&= !B_CAPS_MASK;
			break;
		case B_INS_DOWN: //*
			KeyPressStatus|=B_INS_MASK;
			break;
		case B_INS_UP://*
			KeyPressStatus&= !B_INS_MASK;
			break;
		case B_SYSREQ:
			KeyPressStatus|=B_SYSREQ_MASK;
			break;
		}
	//Keyboard LED management
	BYTE KeyLED=0;
	if ( KeyPressStatus & B_SCROLL_MASK )
		KeyLED|=1;
	if ( KeyPressStatus & B_NUM_MASK )
		KeyLED|=2;
	if ( KeyPressStatus & B_CAPS_MASK )
		KeyLED|=4;
	
	if ( (InPortByte(KEY_OnboardController) & 0x1) )//checking whether we can communicate with keyboard
		{OutPortByte(KEY_KeyboardController,0xED); //sending LED command to keyboard microcontroller
		while ( !(InPortByte(KEY_OnboardController) & 0x1) );//checking whether we can communicate with keyboard
		OutPortByte(KEY_KeyboardController,KeyLED); //LED Value
		}		
	
	if ( Para1 >= 0x81 ) //key upcode
		{//post software message 
		return;
		}

	if ( KeyBufferTotal >= BUFFERSIZE ) //checking whether buffer is filled
		{Beep();	
		return;
		}
	if ( KeyBufferStart >= BUFFERSIZE )
		KeyBufferStart=0;
	StoreOffset=KeyBufferStart+KeyBufferTotal;
	if  ( StoreOffset >= BUFFERSIZE )
		StoreOffset=StoreOffset-BUFFERSIZE;
	KeyBuffer[StoreOffset]=(BYTE)Para1;
	KeyBufferTotal++;
	LastKeyPress = Para1;
	}
BYTE InitKeyboard()
	{
	Sys_KeyboardHardwareID = RegisterDeviceDriver(0, 0, 1, "Generic Keyboard V1.0", "Power Team", KeyboardHand);
	if ( Sys_KeyboardHardwareID )
		{SetIDT(32+1,KeyboardHandler);
		KeyBuffer=(BYTE *)AllocHeap(BUFFERSIZE);
		if ( KeyBuffer == NULL )
			Printf("\n\rKeyboard : Memory allocation failled for Buffer.");
		KeyBufferTotal=KeyPressStatus=LastKeyPress=KeyBufferStart=0;
		}
	else
		Printf("\n\rKeyboard : Unable to register driver.");
	return Sys_KeyboardHardwareID;
	}

//returns the last key pressed and donot remove it from the queue
BYTE GetLastKey()
	{if ( KeyBufferTotal > 0 )
		return KeyBuffer[KeyBufferTotal];
	return 0;
	}
//return total keys in buffer
BYTE KeysAvailable()
	{return KeyBufferTotal;
	}
BYTE ReadKeyboard()
	{
	while(!KeyBufferTotal); //waiting for a key press

	KeyBufferTotal--;
	return KeyBuffer[KeyBufferStart++];
	}
void UnReadKeyboard()
	{
	}
BYTE KeyboardPressStatus()
	{return KeyPressStatus;
	}
void SetKeyboardRate(UINT16 Rate, BYTE Delay)
	{
	}
void GetKeyboardRate(UINT16 * Rate, BYTE * Delay)
	{
	}
BYTE ConvertScanCode(BYTE ScanCode, BYTE Modifier)
	{if ( ScanCode > 0 && ScanCode < 0x40 )
		return KeyScanCodeSet[ScanCode-1][0];
	else
		return 0;
	}
