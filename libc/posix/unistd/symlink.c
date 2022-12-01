/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <process.h>

static char EXE_SUFFIX[] = ".exe";
static char STUBIFY[]	 = "stubify.exe";
static char STUBEDIT[]	 = "stubedit.exe";

/* Return a pointer to the tail of the pathname.  */
static const char *
tail (const char *path)
{
  const char *p = path && path[0] ? path + strlen (path) - 1 : path;

  if (p)
    {
      while (p > path && *p != '/' && *p != '\\' && *p != ':')
	p--;
      if (p > path)
	p++;
    }
  return p;
}

/*
   This returns
              -1, when the file does not exist
               0, when it is not a v2 executable
               1, when it is a v2 executable
*/

static int is_v2_prog(const char *program)
{
  return 0;
}

/* Support the DJGPP ``symlinks'' for .exe files.  */
int
symlink (const char *source, const char *dest)
{
  return -1;
}
