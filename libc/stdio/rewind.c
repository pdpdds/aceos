/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <libc/file.h>
#include <os/Kernel32.h>

void rewind(FILE *f)
{
    fflush(f);
    #warning *****Check this
    GFS_SetFilePointer(fileno(f), 0L, SEEK_SET);
    f->_fillsize = 512;	/* See comment in filbuf.c */
    f->_cnt = 0;
    f->_ptr = f->_base;
    f->_flag &= ~(_IOERR|_IOEOF);
    if (f->_flag & _IORW)
        f->_flag &= ~(_IOREAD|_IOWRT);
}
