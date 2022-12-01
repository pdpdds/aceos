/*---------------------------------------------------------------------------*/
/* strnset() - set characters in a string                                    */
/*---------------------------------------------------------------------------*/
#include <string.h>

char *strnset(char *string, int c, size_t count)
{
  char *p = string;
  while(*p && count)
  {
    *p = (char)c;
    count--;
    p++;
  }
  return(string);
}

