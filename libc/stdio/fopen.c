/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <libc/file.h>
#include <libc/local.h>
#include <os/Kernel32.h>

FILE * fopen(const char *file, const char *mode)
{
    FILE *f;
    int fd, rw, oflags = 0;
    
    DWORD dwOpenMode, dwCreationDisposition;
    /*char tbchar;*/
    wchar_t *wc;

    if (file == 0)
        return 0;
    if (mode == 0)
        return 0;

    rw = (mode[1] == '+') || (mode[1] && (mode[2] == '+'));
    dwOpenMode = rw ? GENERIC_READ | GENERIC_WRITE : GENERIC_WRITE;

    switch (*mode)
    {
    case 'a':
        oflags = CREATE_NEW | OPEN_ALWAYS | (rw ? GENERIC_READ | GENERIC_WRITE : GENERIC_WRITE);
        dwCreationDisposition = CREATE_NEW | OPEN_ALWAYS;
        break;
    case 'r':
        oflags = OPEN_EXISTING | (rw ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ);
        dwCreationDisposition = OPEN_EXISTING;
        break;
    case 'w':
        oflags = CREATE_ALWAYS | (rw ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ);
        dwCreationDisposition = CREATE_ALWAYS;
        break;
    default:
        return (NULL);
    }
    
    /* xxx - text/binary distinction ignored for Mobius */
    /*if (mode[1] == '+')
        tbchar = mode[2];
    else
        tbchar = mode[1];

    if (tbchar == 't')
        oflags |= O_TEXT;
    else if (tbchar == 'b')
        oflags |= O_BINARY;
    else
        oflags |= (_fmode & (O_TEXT|O_BINARY));*/

	#warning CHECK THIS
	wc = file;
    //wc = _towc(file);
    if (wc == NULL)
        return NULL;

    #warning ******Check this
    //HANDLE GFS_CreateFile(LPCTSTR lpFilePath,DWORD dwOpenMode, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlags );
    //fd = FsOpen(wc, oflags);
    fd = GFS_CreateFile( wc, dwOpenMode, 0, dwCreationDisposition, 0 );
    if (fd == NULL)
        return NULL;

    free(wc);
    f = __alloc_file();
    if (f == NULL)
        return NULL;

    if (*mode == 'a')
        GFS_SetFilePointer(fd, 0, SEEK_END);

    f->_cnt = 0;
    f->_file = fd;
    f->_bufsiz = 0;
    f->_flag = oflags;

    f->_base = f->_ptr = NULL;
    return f;
}
