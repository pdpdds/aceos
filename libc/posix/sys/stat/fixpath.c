/* Copyright (C) 2001 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>		/* For FILENAME_MAX */
#include <stdlib.h>
#include <errno.h>		/* For errno */
#include <string.h>		/* For strlen() */
#include <fcntl.h>		/* For LFN stuff */
#include <crt0.h>		/* For crt0 flags */
//#include <dos.h>		/* For Win NT version check */
#include <sys/stat.h>
//#include <libc/dosio.h>

static unsigned use_lfn;

static char *__get_current_directory(char *out, int drive_number);

static char *
__get_current_directory(char *out, int drive_number)
{
  out[0]=0;
  return out;
}

__inline__ static int
is_slash(int c)
{
  return c == '/' || c == '\\';
}

__inline__ static int
is_term(int c)
{
  return c == '/' || c == '\\' || c == '\0';
}

/* Takes as input an arbitrary path.  Fixes up the path by:
   1. Removing consecutive slashes
   2. Removing trailing slashes
   3. Making the path absolute if it wasn't already
   4. Removing "." in the path
   5. Removing ".." entries in the path (and the directory above them)
   6. Adding a drive specification if one wasn't there
   7. Converting all slashes to '/'
 */
void
_fixpath(const char *in, char *out)
{
  int		drive_number;
  char		in1[FILENAME_MAX];
  char		*ip;
  char		*op = out;
  int		preserve_case = 1;
  char		*name_start;
  int		mbsize;

  use_lfn = 0;

  /* Perform the same magic conversions that _put_path does.  
  _put_path(in);
  dosmemget(__tb, sizeof(in1), in1);
  ip = in1;*/
  ip = in;

  /* Add drive specification to output string */
  if (((*ip >= 'a' && *ip <= 'z') ||
       (*ip >= 'A' && *ip <= 'Z'))
      && (*(ip + 1) == ':'))
  {
    if (*ip >= 'a' && *ip <= 'z')
    {
      drive_number = *ip - 'a';
      *op++ = *ip++;
    }
    else
    {
      drive_number = *ip - 'A';
      if (*ip <= 'Z')
	*op++ = drive_number + 'a';
      else
	*op++ = *ip;
      ++ip;
    }
    *op++ = *ip++;
  }
  else
  {
    *op++ = 'C';
    *op++ = ':';
  }

  /* Convert relative path to absolute */
  if (!is_slash(*ip))
    op = __get_current_directory(op, drive_number);

  /* Step through the input path */
  while (*ip)
  {
    /* Skip input slashes */
    if (is_slash(*ip))
    {
      ip++;
      continue;
    }

    /* Skip "." and output nothing */
    if (*ip == '.' && is_term(*(ip + 1)))
    {
      ip++;
      continue;
    }

    /* Skip ".." and remove previous output directory */
    if (*ip == '.' && *(ip + 1) == '.' && is_term(*(ip + 2)))
    {
      ip += 2;
      if(out[2] == '.' && *(op - 1) == '.') 
      { 				/* relative path not skipped */
        *op++ = '/';
        *op++ = '.';
        *op++ = '.';
      } else
      /* Don't back up over drive spec */
      if (op > out + 2)
	/* This requires "/" to follow drive spec */
	while (!is_slash(*--op));
      continue;
    }

    /* Copy path component from in to out */
    *op++ = '/';
#if 0
    while (!is_term(*ip)) *op++ = *ip++;
#else
    while (!is_term(*ip))
      {
	mbsize = mblen (ip, MB_CUR_MAX);
	if (mbsize > 1)
	  {
	    /* copy multibyte character */
	    while (--mbsize >= 0)
	      *op++ = *ip++;
	  }
	else
	  *op++ = *ip++;
      }
#endif
  }

  /* If root directory, insert trailing slash */
  if (op == out + 2) *op++ = '/';

  /* Null terminate the output */
  *op = '\0';

  /* switch FOO\BAR to foo/bar, downcase where appropriate */
  for (op = out + 3, name_start = op - 1; *name_start; op++)
  {
    char long_name[FILENAME_MAX], short_name[13];

#if 1
    /* skip multibyte character */
    mbsize = mblen (op, MB_CUR_MAX);
    if (mbsize > 1)
      {
	op += mbsize - 1;
	continue;
      }
#endif
    if (*op == '\\')
      *op = '/';
    if (!preserve_case && (*op == '/' || *op == '\0'))
    {
      memcpy(long_name, name_start+1, op - name_start - 1);
      long_name[op - name_start - 1] = '\0';
      //if (!strcmp(_lfn_gen_short_fname(long_name, short_name), long_name))
	  if(1)
      {
#if 0
	while (++name_start < op)
	  if (*name_start >= 'A' && *name_start <= 'Z')
	    *name_start += 'a' - 'A';
#else
	while (++name_start < op)
	  {
	    mbsize = mblen (name_start, MB_CUR_MAX);
	    if (mbsize > 1)
	      {
		/* skip multibyte character */
		name_start += mbsize - 1;
		continue;
	      }
	    else if (*name_start >= 'A' && *name_start <= 'Z')
	      *name_start += 'a' - 'A';
	  }
#endif
      }
      else
	name_start = op;
    }
    else if (*op == '\0')
      break;
  }
}

