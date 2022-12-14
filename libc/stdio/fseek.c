/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <libc/file.h>
#include <os/Kernel32.h>

int
fseek(FILE *f, long offset, int ptrname)
{
    long p = -1;			/* can't happen? */
    
    /* See comment in filbuf.c */
    f->_fillsize = 512;
    
    f->_flag &= ~_IOEOF;
    if (f->_flag & _IOREAD)
    {
        if (f->_base && !(f->_flag & _IONBF))
        {
            p = ftell(f);
            if (ptrname == SEEK_CUR)
            {
                offset += p;
                ptrname = SEEK_SET;
            }
            /* check if the target position is in the buffer and
            optimize seek by moving inside the buffer */
            if (ptrname == SEEK_SET && (f->_flag & (_IOUNGETC|_IORW)) == 0 && p-offset <= f->_ptr-f->_base && offset-p <= f->_cnt)
            {
                f->_ptr+=offset-p;
                f->_cnt+=p-offset;
                return 0;
            }
        }
    
        if (f->_flag & _IORW)
            f->_flag &= ~_IOREAD;
        
        #warning CHECK THIS CODE
        //UINT32 GFS_SetFilePointer(HANDLE hFile, DWORD dwNewPos, DWORD dwMoveMethod);    
        //p = FsSeek(fileno(f), offset, ptrname);
        p = GFS_SetFilePointer(fileno(f), offset, ptrname);    
        
        f->_cnt = 0;
        f->_ptr = f->_base;
        f->_flag &= ~_IOUNGETC;
    }
    else if (f->_flag & (_IOWRT|_IORW))
    {
        p = fflush(f);
		#warning CHECK THIS CODE
        return GFS_SetFilePointer(fileno(f), offset, ptrname) == -1 || p == EOF ? -1 : 0;
    }
    return p==-1 ? -1 : 0;
}
