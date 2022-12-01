/* $Id: write.c,v 1.1.1.1 2002/12/21 09:50:19 pavlovskii Exp $ */

#include <errno.h>
#include <os/kernel32.h>
ssize_t	_write(int fd, const void *buf, size_t nbyte)
{
    return GFS_WriteFile(fd, nbyte, buf);
}

