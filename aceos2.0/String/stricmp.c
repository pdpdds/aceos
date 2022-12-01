/*---------------------------------------------------------------------------*/
/* stricmp() - compare strings (case insensitive)                            */
/*---------------------------------------------------------------------------*/
#include <ctype.h>

int stricmp(const char *string1, const char *string2)
{
  int result = 0;
  while((toupper(*string1) == toupper(*string2)) && (!result))
  {
    if(!(*string1) || !(*string2))
    {
      result = 1;
    } else {
      string1++;
      string2++;
    }
  }
  result = 0;
  if(*string1 != *string2)
  {
    result = 1;
    if(toupper(*string1) < toupper(*string2))
    {
      result = -1;
    }
  }
  return(result);
}

