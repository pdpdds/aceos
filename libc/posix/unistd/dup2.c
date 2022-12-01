/* Copyright (C) 2001 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <io.h>
#include <sys/fsext.h>

int
dup2(int fd, int newfd)
{
    #warning Write implementation
    return -1;
}
