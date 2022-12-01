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
BYTE InitKeyboard();
void KeyboardHandler();

BYTE  GetLastKey();
BYTE KeysAvailable();
BYTE ReadKeyboard();
void UnreadKeyboard(BYTE KeyCode);
BYTE KeyboardPressStatus();
void SetKeyboardRate(UINT16 Rate,BYTE Delay);
void GetKeyboardRate(UINT16 * Rate, BYTE * Delay);
BYTE ConvertScanCode(BYTE ScanCode, BYTE Modifier);

#ifdef __cplusplus
	}
#endif

#endif


