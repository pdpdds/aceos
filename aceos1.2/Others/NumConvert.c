#include <TypeDefs.h>
char * UIntToString(unsigned int Val, char * Str, char Base)
	{char Result, strTmp[8], Pos, i;
	 unsigned int Value=Val;
         Pos=0;
         do
        	{
                 Result = Value % Base;
                 Value = Value / Base;
                 strTmp[Pos++] = Result>9?'A'+(Result-10):'0'+Result;
                }while ( Value );
         strTmp[Pos--]=0;
         for(i=0;Pos>=0;Pos--,i++)
         	Str[i]=strTmp[Pos];
         Str[i]=0;
         return Str;
        }
char * ULongToString(UINT32 Val, char * Str, char Base)
	{char Result, strTmp[16], Pos, i;
	 UINT32 Value=Val;
         Pos=0;
         do
        	{
                 Result = Value % Base;
                 Value = Value / Base;
                 strTmp[Pos++] = Result>9?'A'+(Result-10):'0'+Result;
                }while ( Value );
         strTmp[Pos--]=0;
         for(i=0;Pos>=0;Pos--,i++)
         	Str[i]=strTmp[Pos];
         Str[i]=0;
         return Str;
        }
