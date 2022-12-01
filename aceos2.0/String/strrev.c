/*---------------------------------------------------------------------------*/
/* strrev() - reverse characters in a string                                 */
/*---------------------------------------------------------------------------*/
#include <string.h>

char *strrev(char *string)
{
  char temp;
  size_t slen;
  char *p1, *p2;
  slen = strlen(string);
  if(slen > 1)
  {
    p1 = string;
    p2 = string + (slen - 1);
    while (p1 < p2)
    {
      temp = *p1;
      *p1 = *p2;
      *p2 = temp;
      p1++;
      p2--;
    }
  }
  return(string);
}

