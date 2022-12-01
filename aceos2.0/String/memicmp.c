/*---------------------------------------------------------------------------*/
/* memicmp() - compare characters in two buffers (case insensitive)          */
/*---------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdlib.h>

int memicmp(const void *buf1, const void *buf2, unsigned int count)
{
  char *p1, *p2;
  int result = 0;
  p1 = (char *)buf1;
  p2 = (char *)buf2;
  while(count)
  {
    if(toupper(*p1) != toupper(*p2))
    {
      result = 1;
      if(toupper(*p1) < toupper(*p2))
      {
        result = -1;
      }
      break;
    }
    p1++;
    p2++;
    count--;
  }
  return(result);
}

