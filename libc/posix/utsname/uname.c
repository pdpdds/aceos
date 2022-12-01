/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <errno.h>

int uname(struct utsname *u)
{
  if (!u)
  {
    errno = EFAULT;
    return -1;
  }

  strcpy(u->sysname, "Ace");
  sprintf(u->release, "%d", 1);
  sprintf(u->version, "%d", 2);
  strcpy(u->machine, "PC");
  strcpy(u->nodename, "PC");
  return 0;
}
