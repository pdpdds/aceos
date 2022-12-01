/*---------------------------------------------------------------------------*/
/* memmove() - move one buffer to another                                    */
/*---------------------------------------------------------------------------*/
#include <string.h>

void *memmove(void *dest, const void *src, size_t count)
{
  char *p1, *p2;
  if(src != dest)
  {
    p1 = (char *)src;
    p2 = (char *)dest;
    if(dest > src)
    {
      p1 += (count - 1);
      p2 += (count - 1);
      while(count)
      {
        *p2 = *p1;
        p1--;
        p2--;
        count--;
      }
    } else {
      while(count)
      {
        *p2 = *p1;
        p1++;
        p2++;
        count--;
      }
    }
  }
  return(dest);
}

