/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

static char pc_n[]= "pc";

int
gethostname (char *buf, int size)
{
	buf[0]=0;
  return 0;
}
