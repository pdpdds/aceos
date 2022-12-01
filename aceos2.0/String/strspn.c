/*---------------------------------------------------------------------------*/
/* strspn() - find the first substring                                       */
/*---------------------------------------------------------------------------*/
#include <string.h>

size_t strspn(const char *string1, const char *string2)
{
  char *p;
  int found = 1;
  size_t slen = 0;
  while(*string1 && (found))
  {
    found = 0;
    p = (char *)string2;
    while(*p)
    {
      if(*string1 == *p)
      {
        slen++;
        found = 1;
        break;
      }
      p++;
    }
    if(found)
    {
      string1++;
    }
  }
  return(slen);
}

