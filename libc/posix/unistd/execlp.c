/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <unistd.h>
#include <process.h>
#include <libc/unconst.h>

extern char * const *environ;

int execlp(const char *path, const char *argv0, ...)
{
  return spawnvpe(P_OVERLAY, path, unconst(&argv0,char * const *), environ);
}
