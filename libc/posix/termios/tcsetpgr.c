/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */

#include <libc/stubs.h>
#include <unistd.h>
#include <errno.h>

int
tcsetpgrp (int fd, pid_t pgid)
{
  return -1;
}
