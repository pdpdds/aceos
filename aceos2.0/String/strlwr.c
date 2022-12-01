/*---------------------------------------------------------------------------*/
/* strlwr() - convert a string to lower case                                 */
/*---------------------------------------------------------------------------*/
#include <ctype.h>

char *strlwr(char *string)
{
  char *p = string;
  while(*p)
  {
    *p = (char)tolower(*p);
    p++;
  }
  return(string);
}

