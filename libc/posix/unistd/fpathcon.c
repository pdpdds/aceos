/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

long
fpathconf(int fildes, int name)
{
  static char root_path[] = "/";
  struct stat st_buf;
  char fs_root[4];
  char *p = root_path;

  
  return pathconf(p, name);
}
