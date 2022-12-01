/*---------------------------------------------------------------------------*/
/* strcpy() - copy a string to another                                       */
/*---------------------------------------------------------------------------*/

char *strcpy(char *string1, const char *string2)
{
  char *p = string1;
  while(*string2)
  {
    *p = *string2;
    p++;
    string2++;
  }
  *p = '\0';
  return(string1);
}

