/*---------------------------------------------------------------------------*/
/* strrchr() - find the last occurence of a character in a string            */
/*---------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

char *strrchr(const char *string, int c)
{
  char *p = NULL;
  while(*string)
  {
    if((int)(*string) == c)
    {
      p = (char *)string;
    }
    string++;
  }
  return(p);
}

