/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <libc/file.h>
#include <libc/stdiohk.h>
#include <os/Kernel32.h>

/* Note: We set _fillsize to 512, and use that for reading instead of
   _bufsize, for performance reasons.  We double _fillsize each time
   we read here, and reset it to 512 each time we call fseek.  That
   way, we don't waste time reading data we won't use, or doing lots
   of small reads we could optimize.  If we do lots of seeking, we'll
   end up maintaining small read sizes, but if we don't seek, we'll
   eventually read blocks as large as the transfer buffer. */

int _filbuf(FILE *f)
{
    int size, fillsize;
    char c;
    
    if (f->_flag & _IORW)
        f->_flag |= _IOREAD;
    
    if ((f->_flag&_IOREAD) == 0)
        return EOF;
    if (f->_flag&(_IOSTRG|_IOEOF))
        return EOF;
    f->_flag &= ~_IOUNGETC;
    
    if (f->_base==NULL && (f->_flag&_IONBF)==0) 
    {
        size = 512/*_go32_info_block.size_of_transfer_buffer*/;
        if ((f->_base = malloc(size)) == NULL)
        {
            f->_flag |= _IONBF;
            f->_flag &= ~(_IOFBF|_IOLBF);
        }
        else
        {
            f->_flag |= _IOMYBUF;
            f->_bufsiz = size;
            f->_fillsize = 512;
        }
    }
    
    if (f->_flag&_IONBF)
        f->_base = &c;
    
    if (f == stdin) 
    {
        /*if (stdout->_flag&_IOLBF)
        fflush(stdout);
        if (stderr->_flag&_IOLBF)
        fflush(stderr);*/
    }
    
    /* don't read too much! */
    if (f->_fillsize > f->_bufsiz)
        f->_fillsize = f->_bufsiz;
    
    /* This next bit makes it so that the cumulative amount read always
    aligns with file cluster boundaries; i.e. 512, then 2048
    (512+1536), then 4096 (2048+2048) etc. */
    fillsize = f->_fillsize;
    if (fillsize == 1024 && f->_bufsiz >= 1536)
        fillsize = 1536;
    
    size = f->_flag & _IONBF ? 1 : fillsize;
    /* If termios hooked this handle, let it process the request.
    Note that we only call the termios hook for the handles which
    are marked by getc or fread to be hooked by termios.  This is
    because termios converts CR+LF to NL for text-mode handles,
    and thus getc and fread must know to treat the hooked handles
    as if they were binary, i.e. not to strip away CR characters.  */
    /*if ((f->_flag & _IOTERM) == 0
    || __libc_read_termios_hook == NULL
    || __libc_read_termios_hook(fileno(f), f->_base, size, &f->_cnt) == 0)*/
    {
        #warning **** check this
        //UINT32 GFS_ReadFile(HANDLE hFile, UINT32 dwNoOfBytes, void * lpBuffer);
        //if (!FsRead(fileno(f), f->_base, size, &f->_cnt))
        f->_cnt = GFS_ReadFile(fileno(f), size, f->_base);
        if ( f->_cnt <= 0)
            f->_cnt = 0;
        
        #if 0
        if(__is_text_file(f) && f->_cnt>0)
        {
        /* truncate text file at Ctrl-Z */
            char *cz=memchr(f->_base, 0x1A, f->_cnt);
            if(cz)
            {
                int newcnt = cz - f->_base;
                GFS_SetFilePointer(fileno(f), -(f->_cnt - newcnt), SEEK_CUR);
                f->_cnt = newcnt;
            }
        }
        #endif
    }
    
    /* Read more next time, if we don't seek */
    if (f->_fillsize < f->_bufsiz)
        f->_fillsize *= 2;
    f->_ptr = f->_base;
    if (f->_flag & _IONBF)
        f->_base = NULL;
    if (--f->_cnt < 0) 
    {
        if (f->_cnt == -1)
        {
            f->_flag |= _IOEOF;
            if (f->_flag & _IORW)
                f->_flag &= ~_IOREAD;
        }
        else
            f->_flag |= _IOERR;
        f->_cnt = 0;
        return EOF;
    }
    return *f->_ptr++ & 0377;
}
