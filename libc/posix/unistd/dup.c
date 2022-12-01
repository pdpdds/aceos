/* Copyright (C) 2001 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <unistd.h>
#include <errno.h>
#include <io.h>
#include <sys/fsext.h>

int dup(int fd)
{
    #warning write implementation
    return -1;
}
