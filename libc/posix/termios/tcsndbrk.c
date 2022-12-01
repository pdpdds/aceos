/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
#include <errno.h>
#include <io.h>
#include <stddef.h>
#include <termios.h>
#include <libc/ttyprvt.h>

#define _DEV_STDIN  0x0001
#define _DEV_STDOUT 0x0002
#define _DEV_NUL    0x0004
#define _DEV_CLOCK  0x0008
#define _DEV_RAW    0x0020
#define _DEV_CDEV   0x0080
#define _DEV_IOCTRL 0x4000

int
tcsendbreak (int handle, int duration)
{
  return 0;
}
