/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
/* This is file STAT.C */

#include <libc/stubs.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dir.h>
#include <sys/fsext.h>
#include <os/kernel32.h>

#include "xstat.h"


#define ALL_FILES   (FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_DIREC|FA_ARCH)

#define _STAT_INODE         1   /* should we bother getting inode numbers? */
#define _STAT_EXEC_EXT      2   /* get execute bits from file extension? */
#define _STAT_EXEC_MAGIC    4   /* get execute bits from magic signature? */
#define _STAT_DIRSIZE       8   /* compute directory size? */
#define _STAT_ROOT_TIME  0x10   /* try to get root dir time stamp? */
#define _STAT_WRITEBIT   0x20   /* fstat() needs write bit? */

/* Should we bother about executables at all? */
#define _STAT_EXECBIT       (_STAT_EXEC_EXT | _STAT_EXEC_MAGIC)


/* Main entry point.  This is library stat() function.
 */

int
stat(const char *path, struct stat *statbuf)
{
  int  e = errno;
  int  pathlen, ret;

  if (!path || !statbuf)
    {
      errno = EFAULT;
      return -1;
    }

  pathlen = strlen (path);

  /* Fail if PATH includes wildcard characters supported by FindFirst,
     or if it is empty.  */
  if (memchr(path, '*', pathlen) || memchr(path, '?', pathlen)
      || path[0] == '\0')
    {
      errno = ENOENT;	/* since no such filename is possible */
      return -1;
    }
	FILEINFO fInfo;
	GFS_GetFileAttributes( path, &fInfo );
	statbuf->st_gid = 0;
	statbuf->st_atime = fInfo.ftCreationTime.dwLowDateTime;
	statbuf->st_ctime = fInfo.ftLastAccessTime.dwLowDateTime;
	statbuf->st_dev = 0;
	statbuf->st_ino = 0;
	statbuf->st_mtime = fInfo.ftLastWriteTime.dwLowDateTime;
	statbuf->st_nlink = 1;
	statbuf->st_rdev = 0;
	statbuf->st_size = fInfo.nFileSizeLow;
	statbuf->st_uid = 0;
	
	return 0;
}

