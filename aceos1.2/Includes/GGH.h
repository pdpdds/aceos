#ifndef __GGH_H
#define __GGH_H
/*  GGH - Generic Graphics Handler 
	Created By Sam on 19-May-2002
		samuelhard@yahoo.com	www.geocities.com/samuelhard
	
	It provides a interface between programs and the Graphics hardware.
	The main purpose of this layer is to eliminate the complexity of
	calculation occured during primilinary drawing functions such as
	SetPixel() for various modes. Through this layer you can write
	different drawing methods for different modes and you link only the
	neccessary method to this layer.
	
	You can use this handler to draw pixels, lines etc. This handler
	supports only basic drawings supported by VGA cards. You can use
	the advanced functions (3D engine) by directly interfacing the 
	Graphics Driver. 
*/
#include <TypeDefs.h>
struct ModeInfo
	{
	UINT16 Width;		//Specifies the total pixel in horizontal
	UINT16 Height;		//Specifies the total pixel in veritical
	BYTE ColorBits;		//4 for 16 colors, 8 for 256 and so on
	};
struct GenericGraphicsHandler
	{
	BYTE (*SetMode)(BYTE ModeNo);	// function pointer to set mode functions
	BYTE (*GetMode)();
	BYTE (*EnumerateModes)( (*)(struct ModeInfo *) );

	BYTE   (*SetPixel)(UINT16 X, UINT16 Y, UINT32 Color);	
	UINT32 (*GetPixel)(UINT16 X, UINT16 Y);	

	BYTE (*Line)(UINT16 StartX, UINT16 StartY, UINT16 EndX, UINT16 EndY, UINT32 Color);	
	BYTE (*Rectangle)(UINT16 StartX, UINT16 StartY, UINT16 EndX, UINT16 EndY, UINT32 Color);	
	BYTE (*FilledRectangle)(UINT16 StartX, UINT16 StartY, UINT16 EndX, UINT16 EndY, UINT32 Color);	
	BYTE (*Circle)(UINT16 X, UINT16 Y, UINT16 Radius, UINT32 Color);	
	BYTE (*Arc)(UINT16 StartX, UINT16 StartY, UINT16 EndX, UINT16 EndY, UINT32 Color);	
	BYTE (*Ellipse)(UINT16 StartX, UINT16 StartY, UINT16 EndX, UINT16 EndY, UINT32 Color);	
	};

/* Using above functions the following classes are constructed to provide easier Text and
	graphics handling */

class GraphicsText
	{
	private:
		UINT16 FontID;
		UINT16 FontSize;
		UINT32 FontColor;
		UINT32 BackColor;
		BYTE FontType;	//if bit set 0-Bold, 1-Italic, 2-Strike, 3- Underline
	public:
		GraphicsText();
		~GraphicsText();

		Write(UINT16 X, UINT16 Y, char * String)
	};
class GraphicsPen
	{private:
		UINT16 PenWidth;
		UINT16 PenHeight;
		UINT32 PenColor;
	public:
		GraphicsPen();
		~GraphicsPen();

		DrawLine();
		DrawArc();
		DrawCircle();
	};
#endif
