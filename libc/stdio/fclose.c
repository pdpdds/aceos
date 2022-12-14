/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <libc/file.h>
#include <os/kernel32.h>

int
fclose(FILE *f)
{
    int r;
    
    r = EOF;
    if (!f)
        return r;
    if (f->_flag & (GENERIC_READ | GENERIC_WRITE) && !(f->_flag&_IOSTRG))
    {
        r = fflush(f);
        if (!GFS_CloseFile(fileno(f)))
            r = EOF;
        if (f->_flag&_IOMYBUF)
            free(f->_base);
    }
    if (f->_flag & _IORMONCL && f->_name_to_remove)
    {
        remove(f->_name_to_remove);
        free(f->_name_to_remove);
        f->_name_to_remove = 0;
    }
    f->_cnt = 0;
    f->_base = 0;
    f->_ptr = 0;
    f->_bufsiz = 0;
    f->_flag = 0;
    f->_file = -1;
    return r;
}
