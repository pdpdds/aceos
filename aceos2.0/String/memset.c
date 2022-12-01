/*---------------------------------------------------------------------------*/
/* memset() - set a buffer to a specified character                          */
/*---------------------------------------------------------------------------*/
#include <string.h>

void *memset(void *dest, int c, size_t count)
{
  char *p = dest;
  while(count)
  {
    *p = (char)c;
    p++;
    count--;
  }
  return(dest);
}

