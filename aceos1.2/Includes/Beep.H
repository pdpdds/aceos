/*
	Generates Beep on the PC Speaker
		www.geocities.com/samuelhard
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

KERNELDLL void Sound(UINT16 Frequency);
KERNELDLL void NoSound(); 
KERNELDLL void Beep();
#ifdef __cplusplus
	}
#endif

#endif
