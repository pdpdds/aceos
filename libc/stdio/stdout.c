/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <stdio.h>
#include <libc/file.h>
#include <libc/stdiohk.h>
#include <os/Kernel32.h>


FILE __dj_stdout = {
  0, 0, 0, 0,
  _IOWRT | _IOFBF,
  0
};

FILE *__get_stdout(void)
{
    if (__dj_stdout._file == 0)
	__dj_stdout._file = GetStdHandle(STD_OUTPUT_HANDLE);;
    return &__dj_stdout;
}
