/*              System Speeaker 
                      Developed by Sam(samuelhard@yahoo.com) on 04-Sep-2002
                      
*/
#include "TypeDefs.h"
#include "Beep.h"

#define SOUND_MAGIC 0xB6
void Sound(UINT16 Frequency)
	{//sending magic byte
	OutPortByte(0x43, SOUND_MAGIC);

	//writing frequency
	Frequency=120000L/Frequency;
	OutPortByte(0x42, Frequency>>8);
	OutPortByte(0x42, (Frequency<<8)>>8);

	//Sound on
	OutPortByte(0x61, InPortByte(0x61)|3 );
	}
void Beep()
	{Sound(1500);
	MDelay(100);
	NoSound();
	}

