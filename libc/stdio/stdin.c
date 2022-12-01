/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <stdio.h>
#include <libc/file.h>
#include <libc/stdiohk.h>
#include <os/Kernel32.h>


FILE __dj_stdin = {
  0, 0, 0, 0,
  _IOREAD | _IOLBF,
  0
};

FILE *__get_stdin(void)
{
	if (__dj_stdin._file == 0)
		__dj_stdin._file = GetStdHandle(STD_INPUT_HANDLE);
	return &__dj_stdin;
}
