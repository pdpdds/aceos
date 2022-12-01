/*---------------------------------------------------------------------------*/
/* memccpy() - copy characters from a buffer                                 */
/*---------------------------------------------------------------------------*/
#include <string.h>

void *memccpy(void *dest, const void *src, int c, unsigned int count)
{
  char *p1, *p2;
  p1 = (char *)src;
  p2 = (char *)dest;
  while(count)
  {
    *p2 = *p1;
    if(*p2 == (char)c)
    {
      dest = (p2 + 1);
      break;
    }
    p2++;
    p1++;
    count--;
  }
  return(dest);
}

