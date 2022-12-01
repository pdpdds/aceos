/*---------------------------------------------------------------------------*/
/* strchr() - find a character in a string                                   */
/*---------------------------------------------------------------------------*/
#include <stdlib.h>

char *strchr(const char *string, int c)
{
  char *p = NULL;
  while(*string)
  {
    if((int)(*string) == c)
    {
      p = (char *)string;
      break;
    }
    string++;
  }
  return(p);
}

