/*---------------------------------------------------------------------------*/
/* strstr() - find a substring                                               */
/*---------------------------------------------------------------------------*/
#include <stdlib.h>

char *strstr(const char *string1, const char *string2)
{
  char *p1, *p2, *p3;
  int found = 0;
  while(*string1 && (!found))
  {
    p1 = (char *)string1;
    p2 = (char *)string2;
    p3 = NULL;
    found = 1;
    while(*p2)
    {
      if(*p1 != *p2)
      {
        found = 0;
        break;
      } else {
        p1++;
        p2++;
      }
    }
    if(!found)
    {
     string1++;
    } else {
      p3 = (char *)string1;
    }
  }
  return(p3);
}

