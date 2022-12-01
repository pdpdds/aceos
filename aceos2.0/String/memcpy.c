/*---------------------------------------------------------------------------*/
/* memcpy() - copy characters from a buffer                                  */
/*---------------------------------------------------------------------------*/
#include <string.h>

void *memcpy(void *dest, const void *src, size_t count)
{
  char *p1, *p2;
  p1 = (char *)src;
  p2 = (char *)dest;
  while(count)
  {
    *p2 = *p1;
    p1++;
    p2++;
    count--;
  }
  return(dest);
}

