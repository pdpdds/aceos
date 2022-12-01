/*---------------------------------------------------------------------------*/
/* strpbrk() - point to any one of a set of characters within a string       */
/*---------------------------------------------------------------------------*/
#include <stdlib.h>

char *strpbrk(const char *string1, const char *string2)
{
  char *p;
  char *p1 = NULL;
  int found = 0;
  while(*string1 && (!found))
  {
    p = (char *)string2;
    while(*p)
    {
      if(*string1 == *p)
      {
        p1 = (char *)string1;
        found = 1;
        break;
      }
      p++;
    }
    if(!found)
    {
      string1++;
    }
  }
  return(p1);
}

