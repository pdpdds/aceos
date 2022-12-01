/*---------------------------------------------------------------------------*/
/* memchr() - find a character in a buffer                                   */
/*---------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

void *memchr(const void *buf, int c, size_t count)
{
  char *p1 = NULL;
  char *p2;
  p2 = (char *)buf;
  while(count)
  {
    if((int)(*p2) == c)
    {
      p1 = p2;
      break;
    }
    p2++;
    count--;
  }
  return(p1);
}

