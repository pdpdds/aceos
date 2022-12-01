/*              Keyboard interface Routines
                      Developed by Sam(samuelhard@yahoo.com) on 19-June-2002
                      
*/
#include "TypeDefs.h"
#include "Processor.h"
#include "Keyboard.h"
#include "DeviceDriver.h"

#define BUFFERSIZE 100

UINT32 KeyPressStatus=0;
	/*Format of this variable is 
	B_RIGHTSHIFT 	EQU 0				;0 - Right Shift
	B_LEFTSHIFT		EQU 1				;1 - Left Shift
	B_CTRL			EQU 2				;2 - Ctrl
	B_ALT			EQU 3				;3 - Alt
	B_SCROLL		EQU 4				;4 - Scroll Lock Active
	B_NUM			EQU 5				;5 - Num Lock Active
	B_CAPS			EQU 6				;6 - Caps Lock Active
	B_INS			EQU 7				;7 - Insert Active
										;8 - 0
										;9 - 0
	B_SYSREQ		EQU 10				;10 - Sys Rq / Print Screen
	B_HOLD			EQU 11				;11 - Hold Active ( Ctrl+NumLock or Pause/Break )
	*/
BYTE KeyBufferStart=0;	//indicates where character starts
BYTE KeyBufferTotal=0;	//total characters in buffer
BYTE KeyBuffer[BUFFERSIZE];

static char KeyScanCodeSet[][2] =
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
	}
BYTE InitKeyboard()
	{BYTE HardwareID;
	
	HardwareID = RegisterDeviceDriver(0, 0, 1, "Generic Keyboard V1.0", "Power Team", KeyboardHand);
	if ( HardwareID )
		SetIDT(33,KeyboardHandler);
	return HardwareID;
	}

BYTE GetKeyboardLEDStatus()
	{
	}
BYTE SetKeyboardLEDStatus(BYTE LED)	
	{
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

