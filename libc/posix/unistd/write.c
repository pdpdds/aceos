/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <unistd.h>
#include <fcntl.h>
#include <io.h>
#include <errno.h>
#include <libc/ttyprvt.h>

int (*__libc_write_termios_hook)(int handle, const void *buffer, size_t count,
				 ssize_t *rv) = NULL;

ssize_t
write(int handle, const void* buffer, size_t count)
{
  
    #warning Write the data to disk
    return -1;
}
