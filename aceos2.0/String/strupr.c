/*---------------------------------------------------------------------------*/
/* strupr() - convert a string to upper case                                 */
/*---------------------------------------------------------------------------*/
#include <ctype.h>

char *strupr(char *string)
{
  char *p = string;
  while(*p)
  {
    *p = (char)toupper(*p);
    p++;
  }
  return(string);
}

