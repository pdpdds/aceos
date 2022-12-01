#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main(int argc, char * argv[])
	{int Drive=0,Result;
	unsigned char BinImage[512];
	FILE * fp;
	char FileName[]="Boot.bin";
	if ( argc > 1 && stricmp(argv[1],"-h")==0 )
           {
           printf("\n\r Ace OS Boot Maker for DOS");
           printf("\n\r MakeBoot [FileName] <drive>");
           return;
           }
	if ( argc < 2 )
			{printf("\n\r Missing Arguments. ");
           return;
           }
        if ( stricmp(argv[1],"A:") == 0  || stricmp(argv[1],"B:") == 0 )
           {strcpy(FileName, argv[2]);
	   Drive = toupper(argv[1][0])-65;	
	   }
	else
	   {
	   strcpy(FileName, argv[1]);
	   Drive = toupper(argv[2][0])-65;	
	   }
           
        fp=fopen(FileName,"rb");
        if ( fp == NULL )
	   {printf("\n\r Cannot open the file - %s", FileName);
	   return;
	   }
        if ( fread((unsigned char *)BinImage,512,1,fp) == 0 )
	   {printf("\n\r Cannot read all contents from %s", FileName);
	   return;
	   }

        Result = biosdisk(_DISK_WRITE,Drive,0,0,1,1,BinImage) & 0xff00;
        if ( Result  == 0)
			printf("\n\r Successfully Written");
        else
			printf("\n\r Cannot Write. Error Code = %X",Result);
		return Result;

	}
