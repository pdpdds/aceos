#include <TypeDefs.h>
#include <ChType.h>
/*	2) Use String class instead of this one only if you are programming
	   at high level. If you are programming at low kernel level use
	   this file


	Created By Sam
		samuelhard@yahoo.com
*/
void MemCpy(void * Dest, void * Src, UINT32 Len)
	{MoveData(Dest,Src,Len);
	}
void * MemChr(void * Src,BYTE Character, UINT32 Len)
	{UINT32 i;
	for(i=0;i<Len;i++)
		if ( *((BYTE *)Src+i) == Character )
			return (BYTE *)Src+i;
	}
signed char MemCmp(void * S1, void * S2,UINT32 Len)
	{return StrCmp(S1,S2,Len);
	}
signed char MemICmp(void * S1, void * S2,UINT32 Len)
	{return StrNICmp(S1,S2,Len);
	}
void MemSet(char * String, const char Ch, UINT32 Len)
	{UINT32 i;
	for(i=0;i<=Len;i++)
		String[i]=Ch;
	}

UINT32 StrLen(const char * String)
	{UINT32 i=0;
	while(String[i++]);
	return i-1;
	}
char *StrCpy(char * Dest, const char * Source)
	{UINT32 Len=StrLen(Source),i;
	for(i=0;i<Len+1;i++)
		Dest[i]=Source[i];
	return Dest;
	}
char *StrNCpy(char * Dest,const char * Source,UINT16 Length)
	{UINT32 Len=StrLen(Source),i;
	for(i=0;i<Len && i<Length;i++)
		Dest[i]=Source[i];
	return Dest;
	}
int StrCmp(const char * String1,const char * String2)
	{UINT32 i=0;
	for(i=0;String1[i] && String2[i];i++)
		{if ( String1[i] != String2[i]  )
			return String1[i]-String2[i];
		}
	return String1[i]-String2[i];
	}
int StrICmp(const char * String1,const char * String2)
	{UINT32 i=0;
	for(i=0;String1[i] && String2[i];i++)
		{if ( ToUpper(String1[i]) != ToUpper(String2[i])  )
			return String1[i]-String2[i];
		}
	return String1[i]-String2[i];
	}
int StrNCmp(const char * String1,const char * String2,UINT32 Len)
	{UINT32 i;
	for(i=0;i<Len;i++)
		if ( String1[i] != String2[i]  )
			return String1[i]-String2[i];
	return 0;
	}
int StrNICmp(const char * String1,const char * String2,UINT32 Len)
	{UINT32 i;
	for(i=0;i<Len;i++)
		if ( ToUpper(String1[i]) != ToUpper(String2[i])  )
			return String1[i]-String2[i];
	return 0;
	}
char * StrCat(char * Dest,const char * Source)
	{StrCpy(&Dest[StrLen(Dest)],Source);
	return Dest;
	}
const char *StrStr(const char * String1,char * const String2)
	{UINT32 Len1=StrLen(String1),Len2=StrLen(String2),i,j;
	for(i=0;i<Len1;i++)
		if( String1[i] == String2[0] )
			{for(j=0;j<Len2;j++)
				if ( String1[i+j] != String2[j] )
					{i+=j;
					break;
					}
			if ( j >= Len2 )
				return &String1[i];
			}
	return 0;
	}
const char *StrChr(const char * String,char const Character)
	{UINT32 Len=StrLen(String),i;
	for(i=0;i<Len;i++)
		if( String[i] == Character )
		  return &String[i]; 
	return 0;
	}
const char *StrRChr(const char * String,char const Character)
	{UINT32 Len=StrLen(String),i;
	for(i=Len;i>0;i--)
		if( String[i] == Character )
		  return &String[i]; 
	return 0;
	}
const char *StrNChr(const char * String,char const Character, UINT32 Length)
    {UINT32 Len=StrLen(String),i;
	for(i=0;i<Len&i<=Length;i++)
		if( String[i] == Character )
		  return &String[i]; 
	return 0;
    }

char * RPad(char * Dest, const char * Source , UINT32 Len, char FillChar)
	{UINT32 i;
	StrCpy(Dest , Source );
	if ( StrLen(Source) > Len )
		return Dest;
	for(i=StrLen(Source); i<Len ;i++)
		Dest[i]=FillChar;
	Dest[i]=NULL;
	return Dest;
	}
char * LPad(char * Dest, const char * Source , UINT32 Len, char FillChar)
	{UINT32 i;
	for(i=0;i<Len-StrLen(Source);i++);
		Dest[i]=FillChar;
	if ( StrLen(Source)-Len  > 0)
		StrNCpy(&Dest[i],Source,StrLen(Source)-Len);
	else
		StrCpy(&Dest[i],Source);
	return Dest;
	}
char *StrRev(char *str)
	{char *p1, *p2;
	if (! str || ! *str)
		return str;
	for (p1 = str, p2 = str + StrLen(str) - 1; p2 > p1; ++p1, --p2)
		{*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
		}
	return str;
	}
char *StrUpr(char *String)
	{char *s;
	if (String)
		{for (s = String; *s; ++s)
			*s = ToUpper(*s);
		}
	return String;
	}

char *StrLwr(char *String)
	{char *s;
	if (String)
		{for (s = String; *s; ++s)
			*s = ToLower(*s);
		}
	return String;
	}

UINT32 StrCSpn(const char *s1, const char *s2)
	{const char *p, *spanp;
	char c, sc;

	for (p = s1;;)
		{c = *p++;
		spanp = s2;
		do 	{
			if ((sc = *spanp++) == c)
				return p - 1 - s1;
			} while (sc != 0);
		}
	}

