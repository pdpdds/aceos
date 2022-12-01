/*
	Generates Beep on the PC Speaker
		samuelhard@yahoo.com

	First call the function Sound() with the frequency as the argument then
	use the NoSound() function to turn off the sound
*/
#ifndef __BEEP__H
#define __BEEP__H

#include <TypeDefs.h>
#include <Timer.h>
#include <IO.h>


#ifdef __cplusplus
    extern "C" {
#endif

void Sound(UINT16 Frequency);
void NoSound(); 
void Beep();
#ifdef __cplusplus
    }
#endif

#endif
